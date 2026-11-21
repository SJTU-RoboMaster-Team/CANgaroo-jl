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

#include "TraceFileWriter.h"

#include <QDateTime>
#include <QIODevice>
#include <QMap>
#include <QTextStream>

#include "core/TraceLineFormat.h"

namespace TraceFileWriter
{

void write(QIODevice &out, TraceFileFormat format, std::span<const BusMessage> messages,
           const InterfaceNameFn &interfaceName)
{
    switch (format)
    {
        case TraceFileFormat::CanDump:   writeCanDump(out, messages, interfaceName); return;
        case TraceFileFormat::VectorAsc: writeVectorAsc(out, messages); return;
        case TraceFileFormat::Csv:       writeCsv(out, messages, interfaceName); return;
    }
}

void writeCsv(QIODevice &out, std::span<const BusMessage> messages, const InterfaceNameFn &interfaceName)
{
    QTextStream stream(&out);
    stream << TraceLineFormat::csvHeader() << '\n';
    const double tStart = messages.empty() ? 0.0 : messages.front().getFloatTimestamp();
    for (const BusMessage &msg : messages)
    {
        stream << TraceLineFormat::csvLine(msg, tStart, interfaceName(msg.getInterfaceId())) << '\n';
    }
}

void writeCanDump(QIODevice &out, std::span<const BusMessage> messages, const InterfaceNameFn &interfaceName)
{
    QTextStream stream(&out);
    for (const BusMessage &msg : messages)
    {
        stream << TraceLineFormat::canDumpLine(msg, interfaceName(msg.getInterfaceId())) << '\n';
    }
}

void writeVectorAsc(QIODevice &out, std::span<const BusMessage> messages)
{
    QTextStream stream(&out);

    const QDateTime start = messages.empty() ? QDateTime::currentDateTime() : messages.front().getDateTime();
    const double tStart = messages.empty() ? 0.0 : messages.front().getFloatTimestamp();
    stream << TraceLineFormat::ascHeader(start);

    QMap<BusInterfaceId, int> channels;
    for (const BusMessage &msg : messages)
    {
        const BusInterfaceId id = msg.getInterfaceId();
        auto it = channels.constFind(id);
        if (it == channels.cend())
        {
            it = channels.insert(id, static_cast<int>(channels.size()) + 1);
        }
        stream << TraceLineFormat::ascLine(msg, tStart, it.value()) << '\n';
    }

    stream << TraceLineFormat::ascFooter() << '\n';
}

}
