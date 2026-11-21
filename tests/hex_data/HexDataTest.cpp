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

#include "core/HexData.h"

class HexDataTest : public QObject
{
    Q_OBJECT

private slots:
    void parsesSpaceSeparatedBytes();
    void parsesCompactByteRuns();
    void parsesPrefixedAndShortTokens();
    void rejectsInvalidInput();
    void acceptsFullCanFdPayload();
};

void HexDataTest::parsesSpaceSeparatedBytes()
{
    const auto bytes = HexData::parseByteString(QStringLiteral("00 11 22 AB"));
    QVERIFY(bytes.has_value());
    QCOMPARE(*bytes, QByteArray::fromHex("001122AB"));
}

void HexDataTest::parsesCompactByteRuns()
{
    const auto bytes = HexData::parseByteString(QStringLiteral("00112233"));
    QVERIFY(bytes.has_value());
    QCOMPARE(*bytes, QByteArray::fromHex("00112233"));
}

void HexDataTest::parsesPrefixedAndShortTokens()
{
    const auto prefixed = HexData::parseByteString(QStringLiteral("0x01, 0x0A; 0xFF"));
    QVERIFY(prefixed.has_value());
    QCOMPARE(*prefixed, QByteArray::fromHex("010AFF"));

    const auto shortTokens = HexData::parseByteString(QStringLiteral("1 2 3"));
    QVERIFY(shortTokens.has_value());
    QCOMPARE(*shortTokens, QByteArray::fromHex("010203"));

    const auto mixed = HexData::parseByteString(QStringLiteral("0011 22"));
    QVERIFY(mixed.has_value());
    QCOMPARE(*mixed, QByteArray::fromHex("001122"));
}

void HexDataTest::rejectsInvalidInput()
{
    QVERIFY(!HexData::parseByteString(QString()).has_value());
    QVERIFY(!HexData::parseByteString(QStringLiteral("   ")).has_value());
    QVERIFY(!HexData::parseByteString(QStringLiteral(" ,  ; ")).has_value());
    QVERIFY(!HexData::parseByteString(QStringLiteral("11 2Z")).has_value());
    QVERIFY(!HexData::parseByteString(QStringLiteral("123")).has_value());
    QVERIFY(!HexData::parseByteString(QStringLiteral("0x")).has_value());
    QVERIFY(!HexData::parseByteString(QStringLiteral("-1")).has_value());
    QVERIFY(!HexData::parseByteString(QStringLiteral("GG")).has_value());
    QVERIFY(!HexData::parseByteString(QStringLiteral("0x001 0x02")).has_value());
}

void HexDataTest::acceptsFullCanFdPayload()
{
    QString text;
    for (int i = 0; i < 64; ++i)
    {
        if (i > 0)
            text += QLatin1Char(' ');
        text += QStringLiteral("%1").arg(i, 2, 16, QLatin1Char('0')).toUpper();
    }

    const auto bytes = HexData::parseByteString(text);
    QVERIFY(bytes.has_value());
    QCOMPARE(bytes->size(), 64);
    QCOMPARE(static_cast<quint8>(bytes->at(0)), quint8(0x00));
    QCOMPARE(static_cast<quint8>(bytes->at(63)), quint8(0x3F));
}

QTEST_MAIN(HexDataTest)
#include "HexDataTest.moc"
