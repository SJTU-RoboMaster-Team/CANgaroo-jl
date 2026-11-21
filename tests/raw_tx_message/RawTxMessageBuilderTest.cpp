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

#include <QtTest>

#include "window/RawTxWindow/RawTxMessageBuilder.h"

class RawTxMessageBuilderTest : public QObject
{
    Q_OBJECT

private slots:
    void classicFrame();
    void extendedId_data();
    void extendedId();
    void remoteFrame();
    void fdLength_data();
    void fdLength();
    void emptyPayload();
    void invalidFrame_data();
    void invalidFrame();
};

void RawTxMessageBuilderTest::classicFrame()
{
    RawTxMessageInput input;
    input.id = 0x7FF;
    input.dlc = 2;
    input.interfaceId = 0x123;
    input.data = {0x12, 0x34, 0xFF};
    QString error = QStringLiteral("stale error");

    const auto message = buildRawTxMessage(input, &error);
    QVERIFY(message.has_value());
    QVERIFY(error.isEmpty());
    QCOMPARE(message->getId(), input.id);
    QCOMPARE(message->getLength(), 2);
    QCOMPARE(message->getInterfaceId(), input.interfaceId);
    QCOMPARE(message->getByte(0), 0x12);
    QCOMPARE(message->getByte(1), 0x34);
    QCOMPARE(message->getByte(2), 0);
    QVERIFY(!message->isRX());
    QVERIFY(message->isShow());
    QVERIFY(!message->isExtended());
    QVERIFY(!message->isFD());
    QVERIFY(!message->isRTR());
    QVERIFY(!message->isBRS());
    QVERIFY(!message->isErrorFrame());
}

void RawTxMessageBuilderTest::extendedId_data()
{
    QTest::addColumn<quint32>("id");
    QTest::addColumn<quint32>("expected");
    QTest::newRow("max") << quint32{0x1FFFFFFF} << quint32{0x1FFFFFFF};
    QTest::newRow("clamped") << quint32{0x20000000} << quint32{0x1FFFFFFF};
}

void RawTxMessageBuilderTest::extendedId()
{
    QFETCH(quint32, id);
    QFETCH(quint32, expected);

    RawTxMessageInput input;
    input.id = id;
    input.extended = true;
    const auto message = buildRawTxMessage(input);
    QVERIFY(message.has_value());
    QVERIFY(message->isExtended());
    QCOMPARE(message->getId(), expected);
    QCOMPARE(message->getLength(), 8);
}

void RawTxMessageBuilderTest::remoteFrame()
{
    RawTxMessageInput input;
    input.rtr = true;
    input.data.fill(0xFF);

    const auto message = buildRawTxMessage(input);
    QVERIFY(message.has_value());
    QVERIFY(message->isRTR());
    QCOMPARE(message->getLength(), 8);
    for (int i = 0; i < BusMessage::k_maxDataBytes; ++i) {
        QCOMPARE(message->getByte(i), 0);
    }
}

void RawTxMessageBuilderTest::fdLength_data()
{
    QTest::addColumn<int>("length");
    for (int length : {0, 8, 12, 16, 20, 24, 32, 48, 64}) {
        QTest::addRow("%d-bytes", length) << length;
    }
}

void RawTxMessageBuilderTest::fdLength()
{
    QFETCH(int, length);

    RawTxMessageInput input;
    input.fd = true;
    input.brs = true;
    input.dlc = length;
    input.data.fill(0xA7);

    const auto message = buildRawTxMessage(input);
    QVERIFY(message.has_value());
    QVERIFY(message->isFD());
    QVERIFY(message->isBRS());
    QCOMPARE(message->getLength(), length);
    for (int i = 0; i < BusMessage::k_maxDataBytes; ++i) {
        QCOMPARE(message->getByte(i), i < length ? 0xA7 : 0);
    }
}

void RawTxMessageBuilderTest::emptyPayload()
{
    RawTxMessageInput input;
    input.dlc = 0;
    input.data.fill(0xFF);
    const auto message = buildRawTxMessage(input);
    QVERIFY(message.has_value());
    QCOMPARE(message->getLength(), 0);
    QCOMPARE(message->getByte(0), 0);
}

void RawTxMessageBuilderTest::invalidFrame_data()
{
    QTest::addColumn<int>("length");
    QTest::addColumn<bool>("fd");
    QTest::addColumn<bool>("rtr");
    QTest::addColumn<bool>("brs");
    QTest::newRow("negative") << -1 << false << false << false;
    QTest::newRow("classic-too-long") << 12 << false << false << false;
    QTest::newRow("fd-9") << 9 << true << false << false;
    QTest::newRow("fd-10") << 10 << true << false << false;
    QTest::newRow("fd-11") << 11 << true << false << false;
    QTest::newRow("fd-63") << 63 << true << false << false;
    QTest::newRow("fd-too-long") << 65 << true << false << false;
    QTest::newRow("fd-rtr") << 8 << true << true << false;
    QTest::newRow("classic-brs") << 8 << false << false << true;
}

void RawTxMessageBuilderTest::invalidFrame()
{
    QFETCH(int, length);
    QFETCH(bool, fd);
    QFETCH(bool, rtr);
    QFETCH(bool, brs);

    RawTxMessageInput input;
    input.dlc = length;
    input.fd = fd;
    input.rtr = rtr;
    input.brs = brs;
    QString error;
    QVERIFY(!buildRawTxMessage(input, &error).has_value());
    QVERIFY(!error.isEmpty());
    QVERIFY(!buildRawTxMessage(input).has_value());
}

QTEST_GUILESS_MAIN(RawTxMessageBuilderTest)
#include "RawTxMessageBuilderTest.moc"
