/*

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

// Whole trace files as written by the export (BusTrace::save -> TraceFileWriter).
//
// Expected structure comes from the format definitions and the input frames:
//   candump -L       "(<sec>.<usec>) <iface> <id>#<data>"
//   Vector ASC       header block, relative times, 1-based channels, footer
//   raw CSV          header row plus one row per frame
// Line encoders are covered by trace_line_format.

#include <span>

#include <QtTest>
#include <QBuffer>
#include <QRegularExpression>

#include "core/BusMessage.h"
#include "core/TraceFileWriter.h"

namespace
{

// No digit separators in this file: qmake's moc scanner reads ' as a character
// literal, misses Q_OBJECT and generates no TraceFileWriterTest.moc.
constexpr qint64 base_us = 1757000000000000LL;

QByteArray hex(const char *spaced)
{
    return QByteArray::fromHex(QByteArray(spaced).replace(' ', ""));
}

QString interfaceName(BusInterfaceId id)
{
    if (id == 7) { return QStringLiteral("vcan0"); }
    if (id == 3) { return QStringLiteral("vcan1"); }
    return QStringLiteral("if%1").arg(id);
}

BusMessage frame(qint64 timestampUs, int iface, uint32_t id, bool extended, bool rx, const QByteArray &data)
{
    BusMessage msg;
    msg.setTimestamp_us(timestampUs);
    msg.setInterfaceId(static_cast<BusInterfaceId>(iface));
    msg.setId(id);
    msg.setExtended(extended);
    msg.setRX(rx);
    msg.setLength(static_cast<uint8_t>(data.size()));
    for (int i = 0; i < data.size(); ++i)
    {
        msg.setByte(static_cast<uint8_t>(i), static_cast<uint8_t>(data[i]));
    }
    return msg;
}

QByteArray written(TraceFileFormat format, const QVector<BusMessage> &messages)
{
    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);
    TraceFileWriter::write(buffer, format,
                           std::span<const BusMessage>(messages.constData(), static_cast<std::size_t>(messages.size())),
                           interfaceName);
    return buffer.data();
}

QStringList lines(const QByteArray &content)
{
    return QString::fromUtf8(content).split(QLatin1Char('\n'), Qt::SkipEmptyParts);
}

QStringList tokens(const QString &line)
{
    static const QRegularExpression whitespace(QStringLiteral("\\s+"));
    return line.split(whitespace, Qt::SkipEmptyParts);
}

} // namespace

class TraceFileWriterTest : public QObject
{
    Q_OBJECT

private slots:
    void canDumpFile();
    void ascFile();
    void ascEmptyTraceIsValid();
};

void TraceFileWriterTest::canDumpFile()
{
    const QVector<BusMessage> messages = {
        frame(base_us, 7, 0x123, false, true, hex("112233")),
        frame(base_us + 1500, 3, 0x456, false, false, hex("AA")),
    };
    QCOMPARE(lines(written(TraceFileFormat::CanDump, messages)),
             QStringList({ "(1757000000.000000) vcan0 123#112233", "(1757000000.001500) vcan1 456#AA" }));
}

void TraceFileWriterTest::ascFile()
{
    const QVector<BusMessage> messages = {
        frame(base_us, 7, 0x123, false, true, hex("112233")),
        frame(base_us + 10000, 3, 0x456, false, false, hex("AA")),
        frame(base_us + 25000, 7, 0x124, false, true, hex("BB")),
    };
    const QStringList content = lines(written(TraceFileFormat::VectorAsc, messages));

    QCOMPARE(content.size(), 6 + 3 + 1);
    QVERIFY(content[0].startsWith(QLatin1String("date ")));
    QCOMPARE(content[1], QStringLiteral("base hex  timestamps absolute"));
    QVERIFY(content[4].startsWith(QLatin1String("Begin Triggerblock ")));
    QCOMPARE(content[5].trimmed(), QStringLiteral("0.000000 Start of measurement"));
    QCOMPARE(content.last(), QStringLiteral("End TriggerBlock"));

    // Times relative to the first frame; channels numbered by first appearance.
    QCOMPARE(tokens(content[6]).mid(0, 4), QStringList({ "0.000000", "1", "123", "Rx" }));
    QCOMPARE(tokens(content[7]).mid(0, 4), QStringList({ "0.010000", "2", "456", "Tx" }));
    QCOMPARE(tokens(content[8]).mid(0, 4), QStringList({ "0.025000", "1", "124", "Rx" }));
}

void TraceFileWriterTest::ascEmptyTraceIsValid()
{
    const QStringList content = lines(written(TraceFileFormat::VectorAsc, {}));
    QCOMPARE(content.size(), 7);
    QVERIFY(content[0].startsWith(QLatin1String("date ")));
    QCOMPARE(content.last(), QStringLiteral("End TriggerBlock"));
}

QTEST_APPLESS_MAIN(TraceFileWriterTest)

#include "TraceFileWriterTest.moc"
