/*
  Copyright (c) 2026 Schildkroet
  Copyright (c) 2026 SJTU RoboMaster Team

  This file is part of cangaroo.

  cangaroo is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 2 of the License, or
  (at your option) any later version.

  cangaroo is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with cangaroo.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "TraceFileReader.h"

#include <cmath>
#include <cstring>
#include <span>

#include <QCoreApplication>
#include <QDataStream>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QIODevice>
#include <QLocale>
#include <QMap>
#include <QRegularExpression>
#include <QTextStream>
#include <QTimeZone>
#include <QtEndian>

#include "core/SocketCan.h"
#include "core/TraceFileWriter.h"
#include "core/TraceLineFormat.h"

namespace
{

constexpr quint32 k_linktypeCanSocketCan = 227;

bool reportProgress(const TraceFileReader::ProgressFn &progress, qint64 pos, qint64 total, int &counter)
{
    if (!progress)
    {
        return true;
    }
    if ((++counter & 0x3FF) == 0)
    {
        return progress(pos, total);
    }
    return true;
}

void decodeSocketCanError(BusMessage &msg, uint32_t errClasses, const uint8_t *data, int dataLen)
{
    msg.setErrorFrame(true);
    msg.setId(0);
    msg.setExtended(false);
    msg.setLength(0);

    if (errClasses & SocketCan::err_tx_timeout) msg.setErrorFlag(BusError::TxTimeout);
    if (errClasses & SocketCan::err_ack)        msg.setErrorFlag(BusError::Ack);
    if (errClasses & SocketCan::err_busoff)     msg.setErrorFlag(BusError::BusOff);
    if (errClasses & SocketCan::err_restarted)  msg.setErrorFlag(BusError::Restarted);

    if ((errClasses & SocketCan::err_crtl) && dataLen >= 2)
    {
        if (data[1] & 0x03u) msg.setErrorFlag(BusError::Overrun);
        if (data[1] & 0x0Cu) msg.setErrorFlag(BusError::ErrorWarning);
        if (data[1] & 0x30u) msg.setErrorFlag(BusError::ErrorPassive);
        if (data[1] & 0x40u) msg.setErrorFlag(BusError::ErrorActive);
    }
    if ((errClasses & SocketCan::err_prot) && dataLen >= 4)
    {
        if (data[2] & 0x19u) msg.setErrorFlag(BusError::Bit);
        if (data[2] & 0x02u) msg.setErrorFlag(BusError::Form);
        if (data[2] & 0x04u) msg.setErrorFlag(BusError::Stuff);
        if (data[3] == SocketCan::err_prot_loc_crc_seq) msg.setErrorFlag(BusError::Crc);
    }
    if (msg.errorFlags() == BusErrors{})
    {
        msg.setErrorFlag(BusError::Generic);
    }
}

QStringList splitCsvRow(const QString &line)
{
    QStringList fields;
    QString current;
    bool inQuotes = false;
    for (int i = 0; i < line.size(); ++i)
    {
        const QChar ch = line.at(i);
        if (inQuotes)
        {
            if (ch == QLatin1Char('"'))
            {
                if (i + 1 < line.size() && line.at(i + 1) == QLatin1Char('"'))
                {
                    current.append(QLatin1Char('"'));
                    ++i;
                }
                else
                {
                    inQuotes = false;
                }
            }
            else
            {
                current.append(ch);
            }
        }
        else
        {
            if (ch == QLatin1Char('"'))
            {
                inQuotes = true;
            }
            else if (ch == QLatin1Char(','))
            {
                fields.append(current);
                current.clear();
            }
            else
            {
                current.append(ch);
            }
        }
    }
    fields.append(current);
    return fields;
}

TraceReadResult readCanDump(QIODevice &in, qint64 totalBytes, const TraceFileReader::ProgressFn &progress)
{
    TraceReadResult result;
    QTextStream stream(&in);
    static const QRegularExpression re(QStringLiteral(R"(\((\d+\.\d+)\)\s+(\S+)\s+([0-9A-Fa-f]+)(##?)(.*)$)"));

    int progressCounter = 0;
    int lineNum = 0;
    while (!stream.atEnd())
    {
        if (!reportProgress(progress, in.pos(), totalBytes, progressCounter))
        {
            result.cancelled = true;
            result.errorMessage = QCoreApplication::translate("TraceFileReader", "Loading cancelled by user.");
            return result;
        }
        ++lineNum;
        const QString line = stream.readLine().trimmed();
        if (line.isEmpty())
        {
            continue;
        }

        const QRegularExpressionMatch match = re.match(line);
        if (!match.hasMatch())
        {
            if (result.warnings.size() < 20)
            {
                result.warnings.append(QCoreApplication::translate("TraceFileReader", "Line %1: unrecognized candump syntax").arg(lineNum));
            }
            continue;
        }

        const double timestamp = match.captured(1).toDouble();
        const QString iface = match.captured(2);
        const QString idStr = match.captured(3);
        const uint32_t rawId = idStr.toUInt(nullptr, 16);
        const QString separator = match.captured(4);
        const QString payload = match.captured(5);

        BusMessage msg;
        msg.setTimestamp(timestamp);
        msg.setRX(true);

        if (rawId & SocketCan::err_flag)
        {
            const QByteArray bytes = QByteArray::fromHex(payload.toLatin1());
            decodeSocketCanError(msg, rawId & ~SocketCan::err_flag,
                                 reinterpret_cast<const uint8_t *>(bytes.constData()),
                                 static_cast<int>(bytes.size()));
            result.messages.append(msg);
            result.interfaceNames.append(iface);
            continue;
        }

        msg.setExtended(idStr.length() > 3 || rawId > 0x7FFu);
        msg.setId(rawId & 0x1FFFFFFFu);

        if (separator == QLatin1String("##"))
        {
            msg.setFD(true);
            if (!payload.isEmpty())
            {
                const uint8_t flags = static_cast<uint8_t>(QString(payload[0]).toUInt(nullptr, 16));
                msg.setBRS((flags & 0x1u) != 0);
                msg.setESI((flags & 0x2u) != 0);
                const QString dataStr = payload.mid(1);
                const int len = qMin<int>(dataStr.length() / 2, BusMessage::k_maxDataBytes);
                msg.setLength(static_cast<uint8_t>(len));
                for (int i = 0; i < len; ++i)
                {
                    msg.setByte(static_cast<uint8_t>(i), static_cast<uint8_t>(dataStr.mid(i * 2, 2).toUInt(nullptr, 16)));
                }
            }
        }
        else if (payload.startsWith(QLatin1Char('R'), Qt::CaseInsensitive))
        {
            msg.setRTR(true);
            const QString dlcStr = payload.mid(1);
            const int dlc = dlcStr.isEmpty() ? 0 : qBound(0, dlcStr.toInt(), 8);
            msg.setLength(static_cast<uint8_t>(dlc));
        }
        else
        {
            const int len = qMin<int>(payload.length() / 2, 8);
            msg.setLength(static_cast<uint8_t>(len));
            for (int i = 0; i < len; ++i)
            {
                msg.setByte(static_cast<uint8_t>(i), static_cast<uint8_t>(payload.mid(i * 2, 2).toUInt(nullptr, 16)));
            }
        }

        result.messages.append(msg);
        result.interfaceNames.append(iface);
    }

    if (result.messages.isEmpty())
    {
        result.errorMessage = QCoreApplication::translate("TraceFileReader", "No valid candump frames found in file.");
        return result;
    }

    result.ok = true;
    return result;
}

TraceReadResult readVectorAsc(QIODevice &in, qint64 totalBytes, const TraceFileReader::ProgressFn &progress)
{
    TraceReadResult result;
    QTextStream stream(&in);
    static const QRegularExpression wsRe(QStringLiteral("\\s+"));

    int progressCounter = 0;
    double baseEpochSec = 0.0;
    bool hasHeaderEpoch = false;

    while (!stream.atEnd())
    {
        if (!reportProgress(progress, in.pos(), totalBytes, progressCounter))
        {
            result.cancelled = true;
            result.errorMessage = QCoreApplication::translate("TraceFileReader", "Loading cancelled by user.");
            return result;
        }
        const QString line = stream.readLine().trimmed();
        if (line.isEmpty())
        {
            continue;
        }

        if (!hasHeaderEpoch && line.startsWith(QLatin1String("date "), Qt::CaseInsensitive))
        {
            const QString dtStr = line.mid(5).trimmed();
            QDateTime dt = QLocale(QLocale::C).toDateTime(dtStr, QStringLiteral("ddd MMM dd hh:mm:ss.zzz ap yyyy"));
            if (!dt.isValid())
            {
                dt = QLocale(QLocale::C).toDateTime(dtStr, QStringLiteral("ddd MMM d hh:mm:ss.zzz ap yyyy"));
            }
            if (!dt.isValid())
            {
                dt = QLocale(QLocale::C).toDateTime(dtStr, QStringLiteral("ddd MMM dd hh:mm:ss ap yyyy"));
            }
            if (dt.isValid())
            {
                result.headerStartTime = dt;
                baseEpochSec = static_cast<double>(dt.toMSecsSinceEpoch()) / 1000.0;
                hasHeaderEpoch = true;
            }
            continue;
        }

        if (!line[0].isDigit())
        {
            continue;
        }

        const QStringList parts = line.split(wsRe, Qt::SkipEmptyParts);
        if (parts.size() < 3)
        {
            continue;
        }

        bool ok = false;
        const double relTs = parts[0].toDouble(&ok);
        if (!ok)
        {
            continue;
        }
        const double finalTs = hasHeaderEpoch ? (baseEpochSec + relTs) : relTs;

        // Ignore header marker line like "0.000000 Start of measurement"
        if (parts[1].compare(QLatin1String("Start"), Qt::CaseInsensitive) == 0)
        {
            continue;
        }

        // CAN FD line: timestamp CANFD channel Rx/Tx ID ...
        if (parts[1].compare(QLatin1String("CANFD"), Qt::CaseInsensitive) == 0)
        {
            BusMessage msg;
            if (!TraceLineFormat::parseAscCanFdLine(parts, msg))
            {
                continue;
            }
            msg.setTimestamp(finalTs);
            result.messages.append(msg);
            result.interfaceNames.append(QStringLiteral("CH %1").arg(parts[2]));
            continue;
        }

        const QString channel = parts[1];

        // Error frame: timestamp channel ErrorFrame
        if (parts[2].compare(QLatin1String("ErrorFrame"), Qt::CaseInsensitive) == 0)
        {
            BusMessage msg;
            msg.setTimestamp(finalTs);
            msg.setErrorFrame(true);
            msg.setLength(0);
            msg.setId(0);
            msg.setInterfaceId(static_cast<BusInterfaceId>(channel.toInt()));
            result.messages.append(msg);
            result.interfaceNames.append(QStringLiteral("CH %1").arg(channel));
            continue;
        }

        // LIN frame
        if (parts.size() >= 6 && parts[2].compare(QLatin1String("LIN"), Qt::CaseInsensitive) == 0)
        {
            const uint8_t linId = static_cast<uint8_t>(parts[3].toUInt(nullptr, 16) & 0x3Fu);
            const QString linDir = parts[4];

            BusMessage msg;
            msg.setTimestamp(finalTs);
            msg.setBusType(BusType::LIN);
            msg.setId(linId);
            msg.setRX(linDir.compare(QLatin1String("Rx"), Qt::CaseInsensitive) == 0);

            if (parts[5].startsWith(QLatin1String("LIN_"), Qt::CaseInsensitive))
            {
                msg.setErrorFrame(true);
                msg.setLength(0);
            }
            else if (parts[5].compare(QLatin1String("d"), Qt::CaseInsensitive) == 0 && parts.size() >= 8)
            {
                const int dlc = parts[6].toInt(&ok);
                if (ok && dlc >= 0 && dlc <= 8)
                {
                    msg.setLength(static_cast<uint8_t>(dlc));
                    for (int i = 0; i < dlc && (7 + i) < parts.size(); ++i)
                    {
                        if (parts[7 + i].compare(QLatin1String("checksum"), Qt::CaseInsensitive) == 0)
                            break;
                        msg.setByte(static_cast<uint8_t>(i), static_cast<uint8_t>(parts[7 + i].toUInt(nullptr, 16)));
                    }
                }
            }
            result.messages.append(msg);
            result.interfaceNames.append(QStringLiteral("CH %1").arg(channel));
            continue;
        }

        if (parts.size() < 6)
        {
            continue;
        }

        QString idStr = parts[2];
        const bool extended = idStr.endsWith(QLatin1Char('x'), Qt::CaseInsensitive);
        if (extended)
        {
            idStr.chop(1);
        }

        const uint32_t canId = idStr.toUInt(&ok, 16);
        if (!ok)
        {
            continue;
        }

        const QString dir = parts[3];
        const bool isRtr = (parts[4].compare(QLatin1String("r"), Qt::CaseInsensitive) == 0);
        const int dlc = parts[5].toInt(&ok);
        if (!ok || dlc < 0 || dlc > 8)
        {
            continue;
        }

        BusMessage msg;
        msg.setTimestamp(finalTs);
        msg.setExtended(extended);
        msg.setId(canId);
        msg.setRX(dir.compare(QLatin1String("Rx"), Qt::CaseInsensitive) == 0);
        msg.setRTR(isRtr);
        msg.setLength(static_cast<uint8_t>(dlc));
        msg.setInterfaceId(static_cast<BusInterfaceId>(channel.toInt()));

        if (!isRtr)
        {
            for (int i = 0; i < dlc && (6 + i) < parts.size(); ++i)
            {
                msg.setByte(static_cast<uint8_t>(i), static_cast<uint8_t>(parts[6 + i].toUInt(nullptr, 16)));
            }
        }

        result.messages.append(msg);
        result.interfaceNames.append(QStringLiteral("CH %1").arg(channel));
    }

    if (result.messages.isEmpty())
    {
        result.errorMessage = QCoreApplication::translate("TraceFileReader", "No valid Vector ASC frames found in file.");
        return result;
    }

    result.ok = true;
    return result;
}

TraceReadResult readCsv(QIODevice &in, qint64 totalBytes, const TraceFileReader::ProgressFn &progress)
{
    TraceReadResult result;
    QTextStream stream(&in);

    QString headerLine;
    while (!stream.atEnd())
    {
        headerLine = stream.readLine().trimmed();
        if (!headerLine.isEmpty())
        {
            break;
        }
    }

    if (headerLine.isEmpty())
    {
        result.errorMessage = QCoreApplication::translate("TraceFileReader", "CSV trace file is empty.");
        return result;
    }

    const QStringList headers = splitCsvRow(headerLine);
    QMap<QString, int> col;
    for (int i = 0; i < headers.size(); ++i)
    {
        col.insert(headers[i].trimmed().toLower(), i);
    }

    const int colTs     = col.value(QStringLiteral("timestamp_s"), -1);
    const int colEpoch  = col.value(QStringLiteral("epoch_us"), -1);
    const int colChan   = col.value(QStringLiteral("channel"), -1);
    const int colDir    = col.value(QStringLiteral("direction"), -1);
    const int colBus    = col.value(QStringLiteral("bustype"), -1);
    const int colId     = col.value(QStringLiteral("id_hex"), -1);
    const int colExt    = col.value(QStringLiteral("extended"), -1);
    const int colFd     = col.value(QStringLiteral("fd"), -1);
    const int colBrs    = col.value(QStringLiteral("brs"), -1);
    const int colEsi    = col.value(QStringLiteral("esi"), -1);
    const int colRtr    = col.value(QStringLiteral("rtr"), -1);
    const int colErr    = col.value(QStringLiteral("errorflags"), -1);
    const int colDlc    = col.value(QStringLiteral("dlc"), -1);
    const int colData   = col.value(QStringLiteral("data_hex"), -1);

    if ((colTs < 0 && colEpoch < 0) || colId < 0 || colDlc < 0 || colData < 0)
    {
        result.errorMessage = QCoreApplication::translate(
            "TraceFileReader",
            "Invalid CSV trace header: missing required CAN columns (Timestamp_s/Epoch_us, ID_Hex, DLC, Data_Hex).");
        return result;
    }

    auto field = [](const QStringList &row, int idx) -> QString {
        return (idx >= 0 && idx < row.size()) ? row[idx].trimmed() : QString();
    };

    int progressCounter = 0;
    int lineNum = 1;
    while (!stream.atEnd())
    {
        if (!reportProgress(progress, in.pos(), totalBytes, progressCounter))
        {
            result.cancelled = true;
            result.errorMessage = QCoreApplication::translate("TraceFileReader", "Loading cancelled by user.");
            return result;
        }
        ++lineNum;
        const QString line = stream.readLine().trimmed();
        if (line.isEmpty())
        {
            continue;
        }

        const QStringList row = splitCsvRow(line);
        BusMessage msg;

        bool tsOk = false;
        if (colEpoch >= 0 && !field(row, colEpoch).isEmpty())
        {
            const qint64 us = field(row, colEpoch).toLongLong(&tsOk);
            if (tsOk)
            {
                msg.setTimestamp_us(us);
            }
        }
        if (!tsOk && colTs >= 0)
        {
            const double sec = field(row, colTs).toDouble(&tsOk);
            if (tsOk)
            {
                msg.setTimestamp(sec);
            }
        }
        if (!tsOk)
        {
            result.errorMessage = QCoreApplication::translate("TraceFileReader", "CSV line %1: invalid timestamp.").arg(lineNum);
            return result;
        }

        const QString chan = field(row, colChan).isEmpty() ? QStringLiteral("can0") : field(row, colChan);
        const QString dirStr = field(row, colDir);
        msg.setRX(dirStr.isEmpty() || dirStr.compare(QLatin1String("Rx"), Qt::CaseInsensitive) == 0);

        const QString busStr = field(row, colBus);
        msg.setBusType(busStr.compare(QLatin1String("LIN"), Qt::CaseInsensitive) == 0 ? BusType::LIN : BusType::CAN);

        bool idOk = false;
        const uint32_t id = field(row, colId).toUInt(&idOk, 16);
        if (!idOk)
        {
            result.errorMessage = QCoreApplication::translate("TraceFileReader", "CSV line %1: invalid hex ID '%2'.").arg(lineNum).arg(field(row, colId));
            return result;
        }
        msg.setId(id);

        if (colExt >= 0) msg.setExtended(field(row, colExt).toInt() != 0);
        else msg.setExtended(id > 0x7FFu);

        if (colFd >= 0)  msg.setFD(field(row, colFd).toInt() != 0);
        if (colBrs >= 0) msg.setBRS(field(row, colBrs).toInt() != 0);
        if (colEsi >= 0) msg.setESI(field(row, colEsi).toInt() != 0);
        if (colRtr >= 0) msg.setRTR(field(row, colRtr).toInt() != 0);

        if (colErr >= 0)
        {
            const QString errStr = field(row, colErr);
            if (!errStr.isEmpty() && errStr != QLatin1String("0"))
            {
                bool errOk = false;
                const uint32_t errVal = errStr.startsWith(QLatin1String("0x"), Qt::CaseInsensitive)
                    ? errStr.mid(2).toUInt(&errOk, 16)
                    : errStr.toUInt(&errOk, 10);
                if (errOk && errVal != 0)
                {
                    msg.setErrorFlags(BusErrors::fromInt(static_cast<int>(errVal)));
                }
            }
        }

        bool dlcOk = false;
        const int dlc = field(row, colDlc).toInt(&dlcOk);
        if (!dlcOk || dlc < 0 || dlc > BusMessage::k_maxDataBytes)
        {
            result.errorMessage = QCoreApplication::translate("TraceFileReader", "CSV line %1: invalid DLC '%2'.").arg(lineNum).arg(field(row, colDlc));
            return result;
        }
        msg.setLength(static_cast<uint8_t>(dlc));

        const QString dataStr = field(row, colData);
        if (!msg.isRTR() && dlc > 0)
        {
            const QByteArray rawBytes = QByteArray::fromHex(dataStr.toLatin1().simplified().replace(' ', ""));
            if (rawBytes.size() < dlc)
            {
                result.errorMessage = QCoreApplication::translate("TraceFileReader", "CSV line %1: data payload shorter than DLC (%2 < %3).")
                                          .arg(lineNum).arg(rawBytes.size()).arg(dlc);
                return result;
            }
            for (int i = 0; i < dlc; ++i)
            {
                msg.setByte(static_cast<uint8_t>(i), static_cast<uint8_t>(rawBytes[i]));
            }
        }

        result.messages.append(msg);
        result.interfaceNames.append(chan);
    }

    if (result.messages.isEmpty())
    {
        result.errorMessage = QCoreApplication::translate("TraceFileReader", "No valid CAN frames found in CSV file.");
        return result;
    }

    result.ok = true;
    return result;
}

bool parseSocketCanFrame(QDataStream &ds, quint32 capturedLen, BusMessage &msg)
{
    if (capturedLen < 16)
    {
        return false;
    }

    quint32 can_id = 0;
    quint8 dlc_or_len = 0, pad1 = 0, pad2 = 0, pad3 = 0;
    ds >> can_id >> dlc_or_len >> pad1 >> pad2 >> pad3;
    if (ds.status() != QDataStream::Ok)
    {
        return false;
    }

    const bool isFd = (capturedLen == 72);
    const int dataCap = isFd ? 64 : 8;
    uint8_t data[64] = {};
    const int toRead = qMin(static_cast<int>(capturedLen) - 8, dataCap);
    for (int i = 0; i < toRead; ++i)
    {
        quint8 b = 0;
        ds >> b;
        data[i] = b;
    }
    const int remaining = static_cast<int>(capturedLen) - 8 - toRead;
    for (int i = 0; i < remaining; ++i)
    {
        quint8 b = 0;
        ds >> b;
    }
    if (ds.status() != QDataStream::Ok)
    {
        return false;
    }

    // SocketCAN CAN_ID in PCAP/PCAPng is network byte order (big-endian) when written per LINKTYPE_CAN_SOCKETCAN
    // Note: PcapNgFormat::socketCanFrame writes big-endian can_id, whereas QDataStream follows file byte order.
    // To stay 100% consistent with existing PcapNgFormat & ReplayWindow, check flags in host-read order:
    const bool isErr = (can_id & SocketCan::err_flag) != 0;
    msg.setRX(true);
    if (isErr)
    {
        decodeSocketCanError(msg, can_id & ~SocketCan::err_flag, data, toRead);
        return true;
    }

    msg.setId(can_id & 0x1FFFFFFFu);
    msg.setExtended((can_id & SocketCan::eff_flag) != 0);
    msg.setRTR((can_id & SocketCan::rtr_flag) != 0);

    if (isFd)
    {
        msg.setFD(true);
        msg.setBRS((pad1 & 0x01u) != 0);
        msg.setESI((pad1 & 0x02u) != 0);
        msg.setLength(qMin<uint8_t>(dlc_or_len, 64));
    }
    else
    {
        msg.setFD(false);
        msg.setLength(qMin<uint8_t>(dlc_or_len, 8));
    }

    for (int i = 0; i < msg.getLength(); ++i)
    {
        msg.setByte(static_cast<uint8_t>(i), data[i]);
    }
    return true;
}

TraceReadResult readPcap(QIODevice &in, qint64 totalBytes, const TraceFileReader::ProgressFn &progress)
{
    TraceReadResult result;
    constexpr quint32 k_magicLe   = 0xA1B2C3D4u;
    constexpr quint32 k_magicBe   = 0xD4C3B2A1u;
    constexpr quint32 k_magicNsLe = 0xA1B23C4Du;
    constexpr quint32 k_magicNsBe = 0x4D3CB2A1u;

    QDataStream ds(&in);
    ds.setByteOrder(QDataStream::LittleEndian);
    quint32 magic = 0;
    ds >> magic;
    if (ds.status() != QDataStream::Ok)
    {
        result.errorMessage = QCoreApplication::translate("TraceFileReader", "PCAP file is truncated (missing global header).");
        return result;
    }

    bool nanoSecond = false;
    if (magic == k_magicLe || magic == k_magicNsLe)
    {
        ds.setByteOrder(QDataStream::LittleEndian);
        nanoSecond = (magic == k_magicNsLe);
    }
    else if (magic == k_magicBe || magic == k_magicNsBe)
    {
        ds.setByteOrder(QDataStream::BigEndian);
        nanoSecond = (magic == k_magicNsBe);
    }
    else
    {
        result.errorMessage = QCoreApplication::translate("TraceFileReader", "Invalid PCAP magic number (0x%1).").arg(magic, 8, 16, QLatin1Char('0'));
        return result;
    }

    quint16 verMaj = 0, verMin = 0;
    qint32 thiszone = 0;
    quint32 sigfigs = 0, snaplen = 0, linktype = 0;
    ds >> verMaj >> verMin >> thiszone >> sigfigs >> snaplen >> linktype;
    if (ds.status() != QDataStream::Ok)
    {
        result.errorMessage = QCoreApplication::translate("TraceFileReader", "PCAP global header is truncated.");
        return result;
    }
    if (linktype != k_linktypeCanSocketCan)
    {
        result.errorMessage = QCoreApplication::translate("TraceFileReader", "Unsupported PCAP linktype %1 (expected 227 / LINKTYPE_CAN_SOCKETCAN).").arg(linktype);
        return result;
    }

    int progressCounter = 0;
    while (!ds.atEnd())
    {
        if (!reportProgress(progress, in.pos(), totalBytes, progressCounter))
        {
            result.cancelled = true;
            result.errorMessage = QCoreApplication::translate("TraceFileReader", "Loading cancelled by user.");
            return result;
        }
        quint32 tsSec = 0, tsFrac = 0, capLen = 0, origLen = 0;
        ds >> tsSec >> tsFrac >> capLen >> origLen;
        if (ds.status() != QDataStream::Ok)
        {
            break;
        }

        const int64_t tsUs = nanoSecond
            ? (static_cast<int64_t>(tsSec) * 1000000LL + static_cast<int64_t>(tsFrac) / 1000LL)
            : (static_cast<int64_t>(tsSec) * 1000000LL + static_cast<int64_t>(tsFrac));

        // LINKTYPE_CAN_SOCKETCAN stores can_id in big-endian; temporarily read payload via big-endian stream
        const QByteArray payload = in.read(capLen);
        if (static_cast<quint32>(payload.size()) < capLen)
        {
            result.errorMessage = QCoreApplication::translate("TraceFileReader", "Truncated PCAP packet payload.");
            return result;
        }
        QDataStream frameDs(payload);
        frameDs.setByteOrder(QDataStream::BigEndian);
        BusMessage msg;
        if (parseSocketCanFrame(frameDs, capLen, msg))
        {
            msg.setTimestamp_us(tsUs);
            result.messages.append(msg);
            result.interfaceNames.append(QStringLiteral("pcap0"));
        }
    }

    if (result.messages.isEmpty())
    {
        result.errorMessage = QCoreApplication::translate("TraceFileReader", "No valid CAN frames found in PCAP file.");
        return result;
    }
    result.ok = true;
    return result;
}

TraceReadResult readPcapNg(QIODevice &in, qint64 totalBytes, const TraceFileReader::ProgressFn &progress)
{
    TraceReadResult result;
    constexpr quint32 k_btShb = 0x0A0D0D0Au;
    constexpr quint32 k_btIdb = 0x00000001u;
    constexpr quint32 k_btEpb = 0x00000006u;
    constexpr quint32 k_bomLe = 0x1A2B3C4Du;
    constexpr quint32 k_bomBe = 0x4D3C2B1Au;

    QDataStream ds(&in);
    ds.setByteOrder(QDataStream::LittleEndian);

    struct IfaceInfo
    {
        QString name;
        quint32 linkType = 0;
        quint8 tsResol = 6;
    };
    QVector<IfaceInfo> interfaces;
    bool seenShb = false;
    int progressCounter = 0;

    while (!ds.atEnd())
    {
        if (!reportProgress(progress, in.pos(), totalBytes, progressCounter))
        {
            result.cancelled = true;
            result.errorMessage = QCoreApplication::translate("TraceFileReader", "Loading cancelled by user.");
            return result;
        }
        const qint64 blockStart = in.pos();
        quint32 blockType = 0, blockTotalLen = 0;
        ds >> blockType >> blockTotalLen;
        if (ds.status() != QDataStream::Ok)
        {
            break;
        }

        if (blockType == k_btShb)
        {
            quint32 bom = 0;
            ds >> bom;
            if (bom == k_bomLe)
            {
                ds.setByteOrder(QDataStream::LittleEndian);
            }
            else if (bom == k_bomBe)
            {
                ds.setByteOrder(QDataStream::BigEndian);
            }
            else
            {
                result.errorMessage = QCoreApplication::translate("TraceFileReader", "Invalid PCAPng byte-order magic (0x%1).").arg(bom, 8, 16, QLatin1Char('0'));
                return result;
            }
            in.seek(blockStart + 4);
            ds >> blockTotalLen;
            if (blockTotalLen < 28)
            {
                result.errorMessage = QCoreApplication::translate("TraceFileReader", "Invalid PCAPng SHB block length (%1).").arg(blockTotalLen);
                return result;
            }
            seenShb = true;
            interfaces.clear();
        }
        else if (!seenShb)
        {
            result.errorMessage = QCoreApplication::translate("TraceFileReader", "PCAPng file does not begin with a Section Header Block.");
            return result;
        }
        else if (blockTotalLen < 12)
        {
            result.errorMessage = QCoreApplication::translate("TraceFileReader", "Corrupted PCAPng block length (%1).").arg(blockTotalLen);
            return result;
        }
        else if (blockType == k_btIdb)
        {
            quint16 linkType = 0, reserved = 0;
            quint32 snaplen = 0;
            ds >> linkType >> reserved >> snaplen;

            IfaceInfo info;
            info.linkType = linkType;
            const qint64 optEnd = blockStart + blockTotalLen - 4;
            while (in.pos() + 4 <= optEnd)
            {
                quint16 optCode = 0, optLen = 0;
                ds >> optCode >> optLen;
                if (optCode == 0) break;
                const qint64 valStart = in.pos();
                if (optCode == 2 && optLen > 0)
                {
                    QByteArray nameBytes = in.read(optLen);
                    while (nameBytes.endsWith('\0')) nameBytes.chop(1);
                    info.name = QString::fromUtf8(nameBytes);
                }
                else if (optCode == 9 && optLen == 1)
                {
                    quint8 tsresol = 6;
                    ds >> tsresol;
                    info.tsResol = tsresol;
                }
                const quint32 paddedLen = (optLen + 3u) & ~3u;
                in.seek(valStart + paddedLen);
            }
            if (info.name.isEmpty())
            {
                info.name = QStringLiteral("if%1").arg(interfaces.size());
            }
            interfaces.append(info);
        }
        else if (blockType == k_btEpb)
        {
            quint32 ifaceIdx = 0, tsHigh = 0, tsLow = 0, capLen = 0, origLen = 0;
            ds >> ifaceIdx >> tsHigh >> tsLow >> capLen >> origLen;

            if (ifaceIdx < static_cast<quint32>(interfaces.size())
                && interfaces[ifaceIdx].linkType == k_linktypeCanSocketCan)
            {
                const IfaceInfo &iface = interfaces[ifaceIdx];
                const uint64_t tsRaw = (static_cast<uint64_t>(tsHigh) << 32) | tsLow;
                int64_t tsUs = 0;
                if (iface.tsResol == 6)      tsUs = static_cast<int64_t>(tsRaw);
                else if (iface.tsResol == 9) tsUs = static_cast<int64_t>(tsRaw / 1000ULL);
                else if (iface.tsResol == 3) tsUs = static_cast<int64_t>(tsRaw * 1000ULL);
                else
                {
                    double scale = 1e6;
                    for (quint8 e = 0; e < iface.tsResol; ++e) scale /= 10.0;
                    tsUs = static_cast<int64_t>(std::llround(static_cast<double>(tsRaw) * scale));
                }

                const QByteArray payload = in.read(capLen);
                if (static_cast<quint32>(payload.size()) < capLen)
                {
                    result.errorMessage = QCoreApplication::translate("TraceFileReader", "Truncated PCAPng packet payload.");
                    return result;
                }
                QDataStream frameDs(payload);
                frameDs.setByteOrder(QDataStream::BigEndian);
                BusMessage msg;
                if (parseSocketCanFrame(frameDs, capLen, msg))
                {
                    msg.setTimestamp_us(tsUs);
                    result.messages.append(msg);
                    result.interfaceNames.append(iface.name);
                }
            }
        }

        if (!in.seek(blockStart + blockTotalLen))
        {
            break;
        }
    }

    if (!seenShb)
    {
        result.errorMessage = QCoreApplication::translate("TraceFileReader", "Missing PCAPng Section Header Block.");
        return result;
    }
    if (result.messages.isEmpty())
    {
        result.errorMessage = QCoreApplication::translate("TraceFileReader", "No valid CAN frames found in PCAPng file.");
        return result;
    }
    result.ok = true;
    return result;
}

QString readMdfTextBlock(QIODevice &in, quint64 offset)
{
    if (offset == 0 || !in.seek(static_cast<qint64>(offset)))
    {
        return {};
    }
    const QByteArray hdr = in.read(24);
    if (hdr.size() < 24 || (std::memcmp(hdr.constData(), "##TX", 4) != 0 && std::memcmp(hdr.constData(), "##MD", 4) != 0))
    {
        return {};
    }
    const quint64 len = qFromLittleEndian<quint64>(hdr.constData() + 8);
    if (len < 24)
    {
        return {};
    }
    QByteArray text = in.read(static_cast<qint64>(len - 24));
    const int nul = text.indexOf('\0');
    if (nul >= 0)
    {
        text.truncate(nul);
    }
    return QString::fromUtf8(text);
}

TraceReadResult readVectorMdf(QIODevice &in, qint64 totalBytes, const TraceFileReader::ProgressFn &progress)
{
    TraceReadResult result;

    // 64-byte IDBLOCK
    const QByteArray idBlock = in.read(64);
    if (idBlock.size() < 64 || std::memcmp(idBlock.constData(), "MDF     ", 8) != 0)
    {
        result.errorMessage = QCoreApplication::translate("TraceFileReader", "Invalid MDF4 header (missing 'MDF     ' magic).");
        return result;
    }
    const quint16 mdfVer = qFromLittleEndian<quint16>(idBlock.constData() + 28);
    if (mdfVer < 400)
    {
        result.errorMessage = QCoreApplication::translate("TraceFileReader", "Unsupported MDF version %1 (requires MDF 4.x).").arg(mdfVer);
        return result;
    }

    // HDBLOCK at offset 64
    if (!in.seek(64))
    {
        result.errorMessage = QCoreApplication::translate("TraceFileReader", "MDF4 file truncated before HDBLOCK.");
        return result;
    }
    const QByteArray hdBlock = in.read(104);
    if (hdBlock.size() < 104 || std::memcmp(hdBlock.constData(), "##HD", 4) != 0)
    {
        result.errorMessage = QCoreApplication::translate("TraceFileReader", "Invalid MDF4 HDBLOCK.");
        return result;
    }
    const quint64 dgFirst = qFromLittleEndian<quint64>(hdBlock.constData() + 24);
    const quint64 startTimeNs = qFromLittleEndian<quint64>(hdBlock.constData() + 72);
    const int64_t startTimeUs = static_cast<int64_t>(startTimeNs / 1000ULL);
    if (startTimeUs > 0)
    {
        result.headerStartTime = QDateTime::fromMSecsSinceEpoch(startTimeUs / 1000LL, QTimeZone(0));
    }

    if (dgFirst == 0 || !in.seek(static_cast<qint64>(dgFirst)))
    {
        result.errorMessage = QCoreApplication::translate("TraceFileReader", "MDF4 file has no Data Group (##DG) block.");
        return result;
    }
    const QByteArray dgBlock = in.read(64);
    if (dgBlock.size() < 64 || std::memcmp(dgBlock.constData(), "##DG", 4) != 0)
    {
        result.errorMessage = QCoreApplication::translate("TraceFileReader", "Invalid MDF4 DGBLOCK.");
        return result;
    }
    const quint64 cgFirst = qFromLittleEndian<quint64>(dgBlock.constData() + 32);
    const quint64 dtOffset = qFromLittleEndian<quint64>(dgBlock.constData() + 40);
    const quint8 recIdSize = static_cast<quint8>(dgBlock[56]);

    if (cgFirst == 0 || dtOffset == 0 || !in.seek(static_cast<qint64>(cgFirst)))
    {
        result.errorMessage = QCoreApplication::translate("TraceFileReader", "MDF4 file has missing Channel Group or Data block link.");
        return result;
    }
    const QByteArray cgBlock = in.read(104);
    if (cgBlock.size() < 104 || std::memcmp(cgBlock.constData(), "##CG", 4) != 0)
    {
        result.errorMessage = QCoreApplication::translate("TraceFileReader", "Invalid MDF4 CGBLOCK.");
        return result;
    }
    const quint64 cnFirst = qFromLittleEndian<quint64>(cgBlock.constData() + 32);
    const quint64 cycleCount = qFromLittleEndian<quint64>(cgBlock.constData() + 80);
    const quint32 dataBytes = qFromLittleEndian<quint32>(cgBlock.constData() + 96);
    const quint32 invalBytes = qFromLittleEndian<quint32>(cgBlock.constData() + 100);
    const quint32 recordStride = recIdSize + dataBytes + invalBytes;

    // Walk CNBLOCK chain to verify channel byte offsets
    int offT = -1, offId = -1, offDlc = -1, offDir = -1, offData = -1;
    quint32 dataBitCount = 0;
    quint64 cnOff = cnFirst;
    int cnGuard = 0;
    while (cnOff != 0 && ++cnGuard < 128)
    {
        if (!in.seek(static_cast<qint64>(cnOff)))
        {
            break;
        }
        const QByteArray cn = in.read(160);
        if (cn.size() < 160 || std::memcmp(cn.constData(), "##CN", 4) != 0)
        {
            result.errorMessage = QCoreApplication::translate("TraceFileReader", "Invalid MDF4 CNBLOCK.");
            return result;
        }
        const quint64 cnNext = qFromLittleEndian<quint64>(cn.constData() + 24);
        const quint64 txNameOff = qFromLittleEndian<quint64>(cn.constData() + 40);
        const quint32 byteOff = qFromLittleEndian<quint32>(cn.constData() + 92);
        const quint32 bitCnt = qFromLittleEndian<quint32>(cn.constData() + 96);

        const QString name = readMdfTextBlock(in, txNameOff);
        if (name == QLatin1String("t"))              offT = static_cast<int>(byteOff);
        else if (name == QLatin1String("CAN_ID"))    offId = static_cast<int>(byteOff);
        else if (name == QLatin1String("DLC"))       offDlc = static_cast<int>(byteOff);
        else if (name == QLatin1String("Dir"))       offDir = static_cast<int>(byteOff);
        else if (name == QLatin1String("DataBytes")) { offData = static_cast<int>(byteOff); dataBitCount = bitCnt; }

        cnOff = cnNext;
    }

    if (offT < 0 || offId < 0 || offDlc < 0 || offData < 0 || recordStride == 0)
    {
        result.errorMessage = QCoreApplication::translate(
            "TraceFileReader",
            "Unsupported MDF4 channel layout: expected standard CANgaroo CAN channels (t, CAN_ID, DLC, DataBytes).");
        return result;
    }

    if (!in.seek(static_cast<qint64>(dtOffset)))
    {
        result.errorMessage = QCoreApplication::translate("TraceFileReader", "MDF4 DTBLOCK offset out of range.");
        return result;
    }
    const QByteArray dtHdr = in.read(24);
    if (dtHdr.size() < 24)
    {
        result.errorMessage = QCoreApplication::translate("TraceFileReader", "MDF4 DTBLOCK header truncated.");
        return result;
    }
    if (std::memcmp(dtHdr.constData(), "##DZ", 4) == 0)
    {
        result.errorMessage = QCoreApplication::translate("TraceFileReader", "Compressed MDF4 (##DZ) blocks are not supported.");
        return result;
    }
    if (std::memcmp(dtHdr.constData(), "##DT", 4) != 0)
    {
        result.errorMessage = QCoreApplication::translate("TraceFileReader", "Unsupported MDF4 data block type (expected ##DT).");
        return result;
    }

    const quint64 dtLen = qFromLittleEndian<quint64>(dtHdr.constData() + 8);
    if (dtLen < 24 + cycleCount * recordStride)
    {
        result.errorMessage = QCoreApplication::translate("TraceFileReader", "MDF4 DTBLOCK is shorter than declared record count.");
        return result;
    }

    const int maxPayloadBytes = qMin<int>(static_cast<int>(dataBitCount / 8), BusMessage::k_maxDataBytes);
    result.messages.reserve(static_cast<qsizetype>(cycleCount));
    result.interfaceNames.reserve(static_cast<qsizetype>(cycleCount));

    int progressCounter = 0;
    for (quint64 r = 0; r < cycleCount; ++r)
    {
        if (!reportProgress(progress, in.pos(), totalBytes, progressCounter))
        {
            result.cancelled = true;
            result.errorMessage = QCoreApplication::translate("TraceFileReader", "Loading cancelled by user.");
            return result;
        }
        const QByteArray rec = in.read(recordStride);
        if (static_cast<quint32>(rec.size()) < recordStride)
        {
            result.errorMessage = QCoreApplication::translate("TraceFileReader", "Truncated MDF4 record at index %1.").arg(r);
            return result;
        }

        const char *base = rec.constData() + recIdSize;
        double relSec = 0.0;
        const quint64 tBits = qFromLittleEndian<quint64>(base + offT);
        std::memcpy(&relSec, &tBits, sizeof(relSec));

        const quint32 canId = qFromLittleEndian<quint32>(base + offId);
        const quint8 dlc = static_cast<quint8>(base[offDlc]);
        const quint8 dir = (offDir >= 0) ? static_cast<quint8>(base[offDir]) : 0;
        const uint8_t *payload = reinterpret_cast<const uint8_t *>(base + offData);

        BusMessage msg;
        msg.setTimestamp_us(startTimeUs + static_cast<int64_t>(std::llround(relSec * 1e6)));
        msg.setRX(dir == 0);

        if (canId & SocketCan::err_flag)
        {
            decodeSocketCanError(msg, canId & ~SocketCan::err_flag, payload, qMin<int>(dlc, maxPayloadBytes));
        }
        else
        {
            msg.setId(canId & 0x1FFFFFFFu);
            msg.setExtended((canId & SocketCan::eff_flag) != 0);
            msg.setRTR((canId & SocketCan::rtr_flag) != 0);
            const uint8_t len = static_cast<uint8_t>(qMin<int>(dlc, maxPayloadBytes));
            if (len > 8)
            {
                msg.setFD(true);
            }
            msg.setLength(len);
            for (int i = 0; i < len; ++i)
            {
                msg.setByte(static_cast<uint8_t>(i), payload[i]);
            }
        }

        result.messages.append(msg);
        result.interfaceNames.append(QStringLiteral("mdf0"));
    }

    if (result.messages.isEmpty())
    {
        result.errorMessage = QCoreApplication::translate("TraceFileReader", "MDF4 file contains no CAN records.");
        return result;
    }

    result.ok = true;
    return result;
}

} // namespace

namespace TraceFileReader
{

TraceReadResult read(QIODevice &in, TraceFileFormat format, qint64 totalBytes, const ProgressFn &progress)
{
    switch (format)
    {
        case TraceFileFormat::CanDump:   return readCanDump(in, totalBytes, progress);
        case TraceFileFormat::VectorAsc: return readVectorAsc(in, totalBytes, progress);
        case TraceFileFormat::VectorMdf: return readVectorMdf(in, totalBytes, progress);
        case TraceFileFormat::Pcap:      return readPcap(in, totalBytes, progress);
        case TraceFileFormat::PcapNg:    return readPcapNg(in, totalBytes, progress);
        case TraceFileFormat::Csv:       return readCsv(in, totalBytes, progress);
    }
    TraceReadResult err;
    err.errorMessage = QCoreApplication::translate("TraceFileReader", "Unsupported trace file format.");
    return err;
}

TraceReadResult readFile(const QString &filePath, std::optional<TraceFileFormat> explicitFormat, const ProgressFn &progress)
{
    TraceReadResult err;
    const std::optional<TraceFileFormat> fmt = explicitFormat.has_value() ? explicitFormat : traceFormatFromPath(filePath);
    if (!fmt.has_value())
    {
        err.errorMessage = QCoreApplication::translate("TraceFileReader", "Unknown trace file extension for '%1'.").arg(filePath);
        return err;
    }

    const bool binary = (*fmt == TraceFileFormat::Pcap || *fmt == TraceFileFormat::PcapNg || *fmt == TraceFileFormat::VectorMdf);
    QFile file(filePath);
    if (!file.open(binary ? QIODevice::ReadOnly : (QIODevice::ReadOnly | QIODevice::Text)))
    {
        err.errorMessage = QCoreApplication::translate("TraceFileReader", "Cannot open '%1': %2").arg(filePath, file.errorString());
        return err;
    }

    return read(file, *fmt, file.size(), progress);
}

bool writeSplitFiles(const TraceReadResult &data,
                     const QString &outputDir,
                     const QString &baseName,
                     TraceFileFormat format,
                     int maxFramesPerPart,
                     QStringList *writtenFiles,
                     QString *errorMessage)
{
    if (maxFramesPerPart <= 0)
    {
        if (errorMessage) *errorMessage = QCoreApplication::translate("TraceFileReader", "Invalid split frame count (%1).").arg(maxFramesPerPart);
        return false;
    }
    if (data.messages.isEmpty())
    {
        if (errorMessage) *errorMessage = QCoreApplication::translate("TraceFileReader", "No messages to split.");
        return false;
    }
    if (outputDir.isEmpty() || !QDir().mkpath(outputDir))
    {
        if (errorMessage) *errorMessage = QCoreApplication::translate("TraceFileReader", "Cannot create output directory '%1'.").arg(outputDir);
        return false;
    }

    // Build stable interface IDs for each unique interface name in data
    QMap<QString, BusInterfaceId> nameToId;
    QMap<BusInterfaceId, QString> idToName;
    QVector<BusMessage> normalized = data.messages;
    for (int i = 0; i < normalized.size(); ++i)
    {
        const QString iface = (i < data.interfaceNames.size() && !data.interfaceNames[i].isEmpty())
            ? data.interfaceNames[i]
            : QStringLiteral("can0");
        auto it = nameToId.constFind(iface);
        if (it == nameToId.cend())
        {
            const auto newId = static_cast<BusInterfaceId>(nameToId.size());
            it = nameToId.insert(iface, newId);
            idToName.insert(newId, iface);
        }
        normalized[i].setInterfaceId(it.value());
    }

    const QString ext = traceFormatExtension(format);
    const QString safeBase = baseName.trimmed().isEmpty() ? QStringLiteral("trace_part") : baseName.trimmed();
    const bool binary = (format == TraceFileFormat::Pcap || format == TraceFileFormat::PcapNg || format == TraceFileFormat::VectorMdf);

    int partIdx = 0;
    for (qsizetype offset = 0; offset < normalized.size(); offset += maxFramesPerPart)
    {
        ++partIdx;
        const qsizetype count = qMin<qsizetype>(maxFramesPerPart, normalized.size() - offset);
        const QString filePath = QDir(outputDir).filePath(
            QStringLiteral("%1_%2.%3").arg(safeBase).arg(partIdx, 3, 10, QLatin1Char('0')).arg(ext));

        QFile out(filePath);
        if (!out.open(binary ? (QIODevice::WriteOnly | QIODevice::Truncate)
                             : (QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)))
        {
            if (errorMessage)
            {
                *errorMessage = QCoreApplication::translate("TraceFileReader", "Cannot write '%1': %2")
                                    .arg(filePath, out.errorString());
            }
            return false;
        }

        const std::span<const BusMessage> chunk(normalized.constData() + offset, static_cast<std::size_t>(count));
        TraceFileWriter::write(out, format, chunk, [&idToName](BusInterfaceId id) {
            return idToName.value(id, QStringLiteral("can%1").arg(id));
        });
        out.close();
        if (out.error() != QFileDevice::NoError)
        {
            if (errorMessage)
            {
                *errorMessage = QCoreApplication::translate("TraceFileReader", "Write error on '%1': %2")
                                    .arg(filePath, out.errorString());
            }
            return false;
        }
        if (writtenFiles)
        {
            writtenFiles->append(filePath);
        }
    }

    return true;
}

} // namespace TraceFileReader
