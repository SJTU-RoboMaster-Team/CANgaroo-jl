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

#include <span>

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QIODevice>
#include <QLocale>
#include <QMap>
#include <QRegularExpression>
#include <QTextStream>

#include "core/SocketCan.h"
#include "core/TraceFileWriter.h"
#include "core/TraceLineFormat.h"

namespace
{

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
        if (!busStr.isEmpty() && busStr.compare(QLatin1String("CAN"), Qt::CaseInsensitive) != 0)
        {
            result.errorMessage = QCoreApplication::translate("TraceFileReader", "CSV line %1: unsupported BusType '%2' (only CAN is supported).")
                                      .arg(lineNum).arg(busStr);
            return result;
        }

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

} // namespace

namespace TraceFileReader
{

TraceReadResult read(QIODevice &in, TraceFileFormat format, qint64 totalBytes, const ProgressFn &progress)
{
    switch (format)
    {
        case TraceFileFormat::CanDump:   return readCanDump(in, totalBytes, progress);
        case TraceFileFormat::VectorAsc: return readVectorAsc(in, totalBytes, progress);
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

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
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

    int partIdx = 0;
    for (qsizetype offset = 0; offset < normalized.size(); offset += maxFramesPerPart)
    {
        ++partIdx;
        const qsizetype count = qMin<qsizetype>(maxFramesPerPart, normalized.size() - offset);
        const QString filePath = QDir(outputDir).filePath(
            QStringLiteral("%1_%2.%3").arg(safeBase).arg(partIdx, 3, 10, QLatin1Char('0')).arg(ext));

        QFile out(filePath);
        if (!out.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
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
