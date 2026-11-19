/*
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

#include <span>

#include <QtTest>
#include <QBuffer>
#include <QDir>
#include <QFile>
#include <QTemporaryDir>

#include "core/BusMessage.h"
#include "core/TraceFileReader.h"
#include "core/TraceFileWriter.h"

namespace
{

constexpr qint64 k_baseUs = 1757000000000000LL;

QString ifaceName(BusInterfaceId id)
{
    return QStringLiteral("can%1").arg(id);
}

QVector<BusMessage> makeSamples()
{
    QVector<BusMessage> msgs;

    BusMessage stdMsg;
    stdMsg.setTimestamp_us(k_baseUs);
    stdMsg.setInterfaceId(0);
    stdMsg.setId(0x201);
    stdMsg.setRX(true);
    stdMsg.setLength(8);
    for (uint8_t i = 0; i < 8; ++i) stdMsg.setByte(i, static_cast<uint8_t>(0x10 + i));
    msgs.append(stdMsg);

    BusMessage extMsg;
    extMsg.setTimestamp_us(k_baseUs + 2500);
    extMsg.setInterfaceId(1);
    extMsg.setId(0x18DAF110);
    extMsg.setExtended(true);
    extMsg.setRX(false);
    extMsg.setLength(3);
    extMsg.setByte(0, 0xAA);
    extMsg.setByte(1, 0xBB);
    extMsg.setByte(2, 0xCC);
    msgs.append(extMsg);

    BusMessage fdMsg;
    fdMsg.setTimestamp_us(k_baseUs + 5000);
    fdMsg.setInterfaceId(0);
    fdMsg.setId(0x301);
    fdMsg.setFD(true);
    fdMsg.setBRS(true);
    fdMsg.setESI(true);
    fdMsg.setRX(true);
    fdMsg.setLength(12);
    for (uint8_t i = 0; i < 12; ++i) fdMsg.setByte(i, static_cast<uint8_t>(i * 3));
    msgs.append(fdMsg);

    BusMessage rtrMsg;
    rtrMsg.setTimestamp_us(k_baseUs + 8000);
    rtrMsg.setInterfaceId(1);
    rtrMsg.setId(0x7DF);
    rtrMsg.setRTR(true);
    rtrMsg.setRX(true);
    rtrMsg.setLength(4);
    msgs.append(rtrMsg);

    BusMessage errMsg;
    errMsg.setTimestamp_us(k_baseUs + 12000);
    errMsg.setInterfaceId(0);
    errMsg.setErrorFlag(BusError::BusOff);
    errMsg.setErrorFlag(BusError::Overrun);
    errMsg.setRX(true);
    msgs.append(errMsg);

    return msgs;
}

} // namespace

class TraceFileReaderTest : public QObject
{
    Q_OBJECT

private slots:
    void roundTripCsv();
    void roundTripAsc();
    void roundTripCanDump();
    void roundTripPcap();
    void roundTripPcapNg();
    void roundTripMdf4();
    void rejectsMalformedFiles();
    void writeSplitFilesWorks();
};

void TraceFileReaderTest::roundTripCsv()
{
    const QVector<BusMessage> samples = makeSamples();
    QBuffer buf;
    QVERIFY(buf.open(QIODevice::ReadWrite));
    TraceFileWriter::write(buf, TraceFileFormat::Csv,
                           std::span<const BusMessage>(samples.constData(), static_cast<std::size_t>(samples.size())),
                           ifaceName);
    buf.seek(0);

    const TraceReadResult res = TraceFileReader::read(buf, TraceFileFormat::Csv, buf.size());
    QVERIFY2(res.ok, qPrintable(res.errorMessage));
    QCOMPARE(res.messages.size(), samples.size());
    QCOMPARE(res.interfaceNames.size(), samples.size());

    for (int i = 0; i < samples.size(); ++i)
    {
        const BusMessage &exp = samples[i];
        const BusMessage &got = res.messages[i];
        QCOMPARE(got.getTimestamp_us(), exp.getTimestamp_us());
        QCOMPARE(got.isRX(), exp.isRX());
        QCOMPARE(got.isErrorFrame(), exp.isErrorFrame());
        QCOMPARE(got.errorFlags(), exp.errorFlags());
        QCOMPARE(got.getId(), exp.getId());
        QCOMPARE(got.isExtended(), exp.isExtended());
        QCOMPARE(got.isFD(), exp.isFD());
        QCOMPARE(got.isBRS(), exp.isBRS());
        QCOMPARE(got.isESI(), exp.isESI());
        QCOMPARE(got.isRTR(), exp.isRTR());
        QCOMPARE(got.getLength(), exp.getLength());
        if (!exp.isRTR())
        {
            for (uint8_t b = 0; b < exp.getLength(); ++b)
            {
                QCOMPARE(got.getByte(b), exp.getByte(b));
            }
        }
    }
    QCOMPARE(res.interfaceNames[0], QStringLiteral("can0"));
    QCOMPARE(res.interfaceNames[1], QStringLiteral("can1"));
}

void TraceFileReaderTest::roundTripAsc()
{
    const QVector<BusMessage> samples = makeSamples();
    QBuffer buf;
    QVERIFY(buf.open(QIODevice::ReadWrite));
    TraceFileWriter::write(buf, TraceFileFormat::VectorAsc,
                           std::span<const BusMessage>(samples.constData(), static_cast<std::size_t>(samples.size())),
                           ifaceName);
    buf.seek(0);

    const TraceReadResult res = TraceFileReader::read(buf, TraceFileFormat::VectorAsc, buf.size());
    QVERIFY2(res.ok, qPrintable(res.errorMessage));
    QCOMPARE(res.messages.size(), samples.size());
    QCOMPARE(res.messages[0].getId(), uint32_t(0x201));
    QCOMPARE(res.messages[1].getId(), uint32_t(0x18DAF110));
    QVERIFY(res.messages[1].isExtended());
    QVERIFY(!res.messages[1].isRX());
    QVERIFY(res.messages[2].isFD());
    QVERIFY(res.messages[2].isBRS());
    QVERIFY(res.messages[2].isESI());
    QCOMPARE(res.messages[2].getLength(), uint8_t(12));
    QVERIFY(res.messages[3].isRTR());
    QVERIFY(res.messages[4].isErrorFrame());
}

void TraceFileReaderTest::roundTripCanDump()
{
    const QVector<BusMessage> samples = makeSamples();
    QBuffer buf;
    QVERIFY(buf.open(QIODevice::ReadWrite));
    TraceFileWriter::write(buf, TraceFileFormat::CanDump,
                           std::span<const BusMessage>(samples.constData(), static_cast<std::size_t>(samples.size())),
                           ifaceName);
    buf.seek(0);

    const TraceReadResult res = TraceFileReader::read(buf, TraceFileFormat::CanDump, buf.size());
    QVERIFY2(res.ok, qPrintable(res.errorMessage));
    QCOMPARE(res.messages.size(), samples.size());
    QCOMPARE(res.messages[0].getId(), uint32_t(0x201));
    QVERIFY(res.messages[2].isFD());
    QVERIFY(res.messages[2].isBRS());
    QVERIFY(res.messages[2].isESI());
    QVERIFY(res.messages[4].isErrorFrame());
    QVERIFY(res.messages[4].errorFlags().testFlag(BusError::BusOff));
    QVERIFY(res.messages[4].errorFlags().testFlag(BusError::Overrun));
}

void TraceFileReaderTest::roundTripPcap()
{
    const QVector<BusMessage> samples = makeSamples();
    QBuffer buf;
    QVERIFY(buf.open(QIODevice::ReadWrite));
    TraceFileWriter::write(buf, TraceFileFormat::Pcap,
                           std::span<const BusMessage>(samples.constData(), static_cast<std::size_t>(samples.size())),
                           ifaceName);
    buf.seek(0);

    const TraceReadResult res = TraceFileReader::read(buf, TraceFileFormat::Pcap, buf.size());
    QVERIFY2(res.ok, qPrintable(res.errorMessage));
    QCOMPARE(res.messages.size(), samples.size());
    QCOMPARE(res.messages[0].getId(), uint32_t(0x201));
    QCOMPARE(res.messages[1].getId(), uint32_t(0x18DAF110));
    QVERIFY(res.messages[1].isExtended());
    QVERIFY(res.messages[2].isFD());
    QVERIFY(res.messages[2].isBRS());
    QVERIFY(res.messages[4].isErrorFrame());
    QVERIFY(res.messages[4].errorFlags().testFlag(BusError::BusOff));
}

void TraceFileReaderTest::roundTripPcapNg()
{
    const QVector<BusMessage> samples = makeSamples();
    QBuffer buf;
    QVERIFY(buf.open(QIODevice::ReadWrite));
    TraceFileWriter::write(buf, TraceFileFormat::PcapNg,
                           std::span<const BusMessage>(samples.constData(), static_cast<std::size_t>(samples.size())),
                           ifaceName);
    buf.seek(0);

    const TraceReadResult res = TraceFileReader::read(buf, TraceFileFormat::PcapNg, buf.size());
    QVERIFY2(res.ok, qPrintable(res.errorMessage));
    QCOMPARE(res.messages.size(), samples.size());
    QCOMPARE(res.interfaceNames[0], QStringLiteral("can0"));
    QCOMPARE(res.interfaceNames[1], QStringLiteral("can1"));
    QCOMPARE(res.messages[2].getLength(), uint8_t(12));
    QVERIFY(res.messages[4].errorFlags().testFlag(BusError::BusOff));
}

void TraceFileReaderTest::roundTripMdf4()
{
    const QVector<BusMessage> samples = makeSamples();
    QBuffer buf;
    QVERIFY(buf.open(QIODevice::ReadWrite));
    TraceFileWriter::write(buf, TraceFileFormat::VectorMdf,
                           std::span<const BusMessage>(samples.constData(), static_cast<std::size_t>(samples.size())),
                           ifaceName);
    buf.seek(0);

    const TraceReadResult res = TraceFileReader::read(buf, TraceFileFormat::VectorMdf, buf.size());
    QVERIFY2(res.ok, qPrintable(res.errorMessage));
    QCOMPARE(res.messages.size(), samples.size());
    QCOMPARE(res.messages[0].getId(), uint32_t(0x201));
    QCOMPARE(res.messages[0].getTimestamp_us(), samples[0].getTimestamp_us());
    QCOMPARE(res.messages[1].getId(), uint32_t(0x18DAF110));
    QVERIFY(res.messages[1].isExtended());
    QVERIFY(!res.messages[1].isRX());
    QCOMPARE(res.messages[2].getLength(), uint8_t(12));
    QVERIFY(res.messages[2].isFD());
    QCOMPARE(res.messages[2].getByte(11), uint8_t(33));
    QVERIFY(res.messages[3].isRTR());
    QVERIFY(res.messages[4].isErrorFrame());
    QVERIFY(res.messages[4].errorFlags().testFlag(BusError::BusOff));
}

void TraceFileReaderTest::rejectsMalformedFiles()
{
    QByteArray badPcap("not-a-pcap");
    QBuffer buf(&badPcap);
    QVERIFY(buf.open(QIODevice::ReadOnly));
    const TraceReadResult r1 = TraceFileReader::read(buf, TraceFileFormat::Pcap);
    QVERIFY(!r1.ok);
    QVERIFY(!r1.errorMessage.isEmpty());

    QByteArray badMdf("MDF     3.00    ");
    QBuffer buf2(&badMdf);
    QVERIFY(buf2.open(QIODevice::ReadOnly));
    const TraceReadResult r2 = TraceFileReader::read(buf2, TraceFileFormat::VectorMdf);
    QVERIFY(!r2.ok);
    QVERIFY(!r2.errorMessage.isEmpty());

    QByteArray badCsv("ColA,ColB\n1,2\n");
    QBuffer buf3(&badCsv);
    QVERIFY(buf3.open(QIODevice::ReadOnly));
    const TraceReadResult r3 = TraceFileReader::read(buf3, TraceFileFormat::Csv);
    QVERIFY(!r3.ok);
    QVERIFY(!r3.errorMessage.isEmpty());

    const TraceReadResult r4 = TraceFileReader::readFile(QStringLiteral("/nonexistent/file.blf"));
    QVERIFY(!r4.ok);
    QVERIFY(!r4.errorMessage.isEmpty());
}

void TraceFileReaderTest::writeSplitFilesWorks()
{
    TraceReadResult src;
    src.messages = makeSamples();
    src.interfaceNames = { "can0", "can1", "can0", "can1", "can0" };
    src.ok = true;

    QTemporaryDir dir;
    QVERIFY(dir.isValid());

    QStringList files;
    QString err;
    QVERIFY2(TraceFileReader::writeSplitFiles(src, dir.path(), QStringLiteral("seg"),
                                              TraceFileFormat::Csv, 2, &files, &err),
             qPrintable(err));
    QCOMPARE(files.size(), 3);

    int totalRead = 0;
    for (const QString &f : files)
    {
        const TraceReadResult part = TraceFileReader::readFile(f);
        QVERIFY2(part.ok, qPrintable(part.errorMessage));
        totalRead += part.messages.size();
    }
    QCOMPARE(totalRead, src.messages.size());
}

QTEST_MAIN(TraceFileReaderTest)
#include "TraceFileReaderTest.moc"
