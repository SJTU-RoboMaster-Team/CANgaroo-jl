/*
  Copyright (c) 2015, 2016 Hubert Denkmair <hubert@denkmair.de>
  Copyright (c) 2026 Schildkroet

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

#include "TraceLineFormat.h"

#include <QLocale>

#include "core/BusMessage.h"
#include "core/SocketCan.h"

namespace
{

QString hexByte(uint8_t value)
{
    return QString::number(value, 16).toUpper().rightJustified(2, QLatin1Char('0'));
}

}

namespace TraceLineFormat
{

QString ascHeader(const QDateTime &start)
{
    const QString dt = QLocale(QLocale::C).toString(start, QStringLiteral("ddd MMM dd hh:mm:ss.zzz ap yyyy"));
    return QStringLiteral("date %1\n"
                          "base hex  timestamps absolute\n"
                          "internal events logged\n"
                          "// version 8.5.0\n"
                          "Begin Triggerblock %1\n"
                          "   0.000000 Start of measurement\n").arg(dt);
}

QString ascLine(const BusMessage &msg, double tStart, int channel)
{
    const double t = msg.getFloatTimestamp() - tStart;
    const QString dir = msg.isRX() ? QStringLiteral("Rx") : QStringLiteral("Tx");

    if (msg.isErrorFrame())
    {
        return QStringLiteral("%1 %2  ErrorFrame")
            .arg(t, 11, 'f', 6)
            .arg(channel);
    }

    QString idHexStr = QString::number(msg.getId(), 16);
    QString idDecStr = QString::number(msg.getId());
    if (msg.isExtended())
    {
        idHexStr.append(QLatin1Char('x'));
        idDecStr.append(QLatin1Char('x'));
    }

    if (msg.isFD())
    {
        // Vector ASC CAN FD event:
        // <time> CANFD <channel> <Rx|Tx> <id> <BRS> <ESI> <DLC hex> <data length> <data>
        return QStringLiteral("%1 CANFD %2 %3 %4 %5 %6 %7 %8 %9")
            .arg(t, 11, 'f', 6)
            .arg(channel, 3)
            .arg(dir)
            .arg(idHexStr, 15)
            .arg(msg.isBRS() ? 1 : 0)
            .arg(msg.isESI() ? 1 : 0)
            .arg(canFdLengthToDlc(msg.getLength()), 0, 16)
            .arg(msg.getLength())
            .arg(msg.getDataHexString());
    }

    return QStringLiteral("%1 %2  %3 %4   %5 %6 %7  Length = 0 BitCount = 0 ID = %8")
        .arg(t, 11, 'f', 6)
        .arg(channel)
        .arg(idHexStr, -15)
        .arg(dir)
        .arg(QLatin1Char(msg.isRTR() ? 'r' : 'd'))
        .arg(msg.getLength())
        .arg(msg.getDataHexString())
        .arg(idDecStr);
}

QString ascFooter()
{
    return QStringLiteral("End TriggerBlock");
}

QString canDumpLine(const BusMessage &msg, const QString &interfaceName)
{
    QString line = QStringLiteral("(%1) ").arg(msg.getFloatTimestamp(), 0, 'f', 6);
    line.append(interfaceName);

    const int idWidth = msg.isExtended() ? 8 : 3;
    const QString idHex = QString::number(msg.getId(), 16).toUpper().rightJustified(idWidth, QLatin1Char('0'));

    if (msg.isErrorFrame())
    {
        // Error flag and error classes in the id, 8-byte error payload (<linux/can/error.h>).
        const SocketCan::ErrorFrame error = SocketCan::errorFrame(msg);
        const QString errId = QString::number(SocketCan::err_flag | error.classes, 16).toUpper().rightJustified(8, QLatin1Char('0'));
        line.append(QStringLiteral(" %1#").arg(errId));
        for (const std::uint8_t byte : error.data)
        {
            line.append(hexByte(byte));
        }
    }
    else if (msg.isFD())
    {
        // CANFD: use ## separator with flags byte (bit 0 = BRS, bit 1 = ESI)
        const uint8_t flags = (msg.isBRS() ? 1 : 0) | (msg.isESI() ? 2 : 0);
        line.append(QStringLiteral(" %1##%2").arg(idHex).arg(flags));
        for (int i = 0; i < msg.getLength(); i++)
        {
            line.append(hexByte(msg.getByte(i)));
        }
    }
    else if (msg.isRTR())
    {
        // RTR: #R followed by DLC
        line.append(QStringLiteral(" %1#R%2").arg(idHex).arg(msg.getLength()));
    }
    else
    {
        line.append(QStringLiteral(" %1#").arg(idHex));
        for (int i = 0; i < msg.getLength(); i++)
        {
            line.append(hexByte(msg.getByte(i)));
        }
    }
    return line;
}

QString csvHeader()
{
    return QStringLiteral("Timestamp_s,Epoch_us,Channel,Direction,BusType,ID_Hex,Extended,FD,BRS,ESI,RTR,ErrorFlags,DLC,Data_Hex");
}

QString csvLine(const BusMessage &msg, double tStart, const QString &interfaceName)
{
    const double relTime = msg.getFloatTimestamp() - tStart;
    const QString dir = msg.isRX() ? QStringLiteral("Rx") : QStringLiteral("Tx");
    const int idWidth = msg.isExtended() ? 8 : 3;
    const QString idHex = QString::number(msg.getId(), 16).toUpper().rightJustified(idWidth, QLatin1Char('0'));

    QString safeChannel = interfaceName;
    if (safeChannel.contains(QLatin1Char(',')) || safeChannel.contains(QLatin1Char('"')))
    {
        safeChannel.replace(QLatin1Char('"'), QStringLiteral("\"\""));
        safeChannel = QLatin1Char('"') + safeChannel + QLatin1Char('"');
    }

    QString dataHex;
    if (!msg.isRTR() && msg.getLength() > 0)
    {
        dataHex.reserve(msg.getLength() * 3);
        for (int i = 0; i < msg.getLength(); ++i)
        {
            if (i > 0)
            {
                dataHex.append(QLatin1Char(' '));
            }
            dataHex.append(hexByte(msg.getByte(static_cast<uint8_t>(i))));
        }
    }

    const uint32_t errFlags = msg.errorFlags().toInt();
    const QString errStr = (errFlags != 0)
        ? QStringLiteral("0x%1").arg(errFlags, 0, 16)
        : QStringLiteral("0");

    return QStringLiteral("%1,%2,%3,%4,%5,%6,%7,%8,%9,%10,%11,%12,%13,%14")
        .arg(relTime, 0, 'f', 6)
        .arg(msg.getTimestamp_us())
        .arg(safeChannel, dir, QStringLiteral("CAN"), idHex)
        .arg(msg.isExtended() ? 1 : 0)
        .arg(msg.isFD() ? 1 : 0)
        .arg(msg.isBRS() ? 1 : 0)
        .arg(msg.isESI() ? 1 : 0)
        .arg(msg.isRTR() ? 1 : 0)
        .arg(errStr)
        .arg(msg.getLength())
        .arg(dataHex);
}

bool parseAscCanFdLine(const QStringList &parts, BusMessage &msg)
{
    if (parts.size() < 9 || parts[1].compare(QLatin1String("CANFD"), Qt::CaseInsensitive) != 0)
    {
        return false;
    }

    bool ok = false;
    const int channel = parts[2].toInt(&ok);
    if (!ok)
    {
        return false;
    }

    QString idStr = parts[4];
    const bool extended = idStr.endsWith(QLatin1Char('x'), Qt::CaseInsensitive);
    if (extended)
    {
        idStr.chop(1);
    }
    const uint32_t id = idStr.toUInt(&ok, 16);
    if (!ok)
    {
        return false;
    }

    // Optional symbolic frame name between the id and BRS.
    qsizetype idx = 5;
    [[maybe_unused]] const int probe = parts[idx].toInt(&ok);
    if (!ok)
    {
        ++idx;
    }
    if (parts.size() < idx + 4)
    {
        return false;
    }

    const int brsOrFlags = parts[idx].toInt(&ok);
    if (!ok)
    {
        return false;
    }

    int dataLength = -1;
    qsizetype dataIdx = 0;
    bool esi = false;

    // Vector layout: the DLC code must agree with the data length.
    bool dlcOk = false;
    bool lenOk = false;
    const int dlc = parts[idx + 2].toInt(&dlcOk, 16);
    const int len = parts[idx + 3].toInt(&lenOk);
    if (dlcOk && lenOk && canFdDlcToLength(dlc) == len)
    {
        dataLength = len;
        dataIdx = idx + 4;
        esi = parts[idx + 1] == QLatin1String("1");
    }
    // Legacy cangaroo layout: flags, two reserved zeros, then the byte count twice.
    else if (parts.size() >= idx + 5
             && parts[idx + 1] == QLatin1String("0") && parts[idx + 2] == QLatin1String("0")
             && lenOk && parts[idx + 4].toInt(&ok) == len && ok)
    {
        dataLength = len;
        dataIdx = idx + 5;
    }
    else
    {
        return false;
    }

    if (dataLength < 0 || dataLength > 64 || parts.size() < dataIdx + dataLength)
    {
        return false;
    }

    msg.setFD(true);
    msg.setBRS((brsOrFlags & 0x1) != 0);
    msg.setESI(esi);
    msg.setExtended(extended);
    msg.setId(id);
    msg.setRX(parts[3].compare(QLatin1String("Rx"), Qt::CaseInsensitive) == 0);
    msg.setInterfaceId(static_cast<BusInterfaceId>(channel));
    msg.setLength(static_cast<uint8_t>(dataLength));

    for (int i = 0; i < dataLength; ++i)
    {
        const uint value = parts[dataIdx + i].toUInt(&ok, 16);
        if (!ok || value > 0xFF)
        {
            return false;
        }
        msg.setByte(static_cast<uint8_t>(i), static_cast<uint8_t>(value));
    }
    return true;
}

}
