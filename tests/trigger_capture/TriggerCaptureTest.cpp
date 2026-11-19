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
#include <QDir>
#include <QSignalSpy>
#include <QTemporaryDir>

#include "core/BusMessage.h"
#include "core/TraceFileReader.h"
#include "core/TriggerCaptureManager.h"

namespace
{

constexpr qint64 k_baseUs = 1757000000000000LL;

QString ifaceName(BusInterfaceId id)
{
    return QStringLiteral("can%1").arg(id);
}

BusMessage dataFrame(qint64 offsetMs, uint32_t id, uint8_t b0, uint8_t b1 = 0)
{
    BusMessage msg;
    msg.setTimestamp_us(k_baseUs + offsetMs * 1000LL);
    msg.setInterfaceId(0);
    msg.setId(id);
    msg.setRX(true);
    msg.setLength(8);
    msg.setByte(0, b0);
    msg.setByte(1, b1);
    return msg;
}

} // namespace

class TriggerCaptureTest : public QObject
{
    Q_OBJECT

private slots:
    void busErrorFrameTriggersPreAndPostSlice();
    void softwareFrameMaskTriggersAndSavesCsv();
    void watchdogTimeoutTriggersOnSilentId();
    void signalThresholdTriggersViaEvaluator();
};

void TriggerCaptureTest::busErrorFrameTriggersPreAndPostSlice()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());

    TriggerCaptureManager mgr(ifaceName);
    TriggerCaptureConfig cfg;
    cfg.outputFolder = dir.path();
    cfg.format = TraceFileFormat::Csv;
    cfg.preTriggerSeconds = 2.0;
    cfg.postTriggerSeconds = 1.0;
    cfg.oneShot = true;

    TriggerRule rule;
    rule.name = QStringLiteral("BusOff");
    rule.kind = TriggerRule::Kind::BusErrorEvent;
    rule.errorMask = BusError::BusOff;
    cfg.rules.append(rule);

    mgr.setConfig(cfg);
    QSignalSpy savedSpy(&mgr, &TriggerCaptureManager::captureSaved);
    mgr.setArmed(true);

    // Pre-trigger frames spanning 5 seconds (only last 2s should stay)
    for (int ms = 0; ms <= 5000; ms += 500)
    {
        mgr.enqueue(dataFrame(ms, 0x201, static_cast<uint8_t>(ms / 500)));
    }

    // Triggering BusOff frame at t=5500ms
    BusMessage err;
    err.setTimestamp_us(k_baseUs + 5500LL * 1000LL);
    err.setInterfaceId(0);
    err.setErrorFlag(BusError::BusOff);
    err.setRX(true);
    mgr.enqueue(err);

    // Post-trigger frames up to t=6600ms (> 1.0s post window)
    mgr.enqueue(dataFrame(6000, 0x201, 0xFE));
    mgr.enqueue(dataFrame(6600, 0x201, 0xFF));
    mgr.processNow();

    QCOMPARE(savedSpy.count(), 1);
    QVERIFY(!mgr.isArmed()); // oneShot disarms after saving

    const QString savedPath = savedSpy.at(0).at(0).toString();
    const TraceReadResult readBack = TraceFileReader::readFile(savedPath);
    QVERIFY2(readBack.ok, qPrintable(readBack.errorMessage));
    // Pre frames from 3500, 4000, 4500, 5000 (4) + err(5500) + post(6000, 6600) = 7
    QVERIFY(readBack.messages.size() >= 6);
    bool foundErr = false;
    for (const BusMessage &m : readBack.messages)
    {
        if (m.isErrorFrame() && m.errorFlags().testFlag(BusError::BusOff))
        {
            foundErr = true;
            break;
        }
    }
    QVERIFY(foundErr);
}

void TriggerCaptureTest::softwareFrameMaskTriggersAndSavesCsv()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());

    TriggerCaptureManager mgr(ifaceName);
    TriggerCaptureConfig cfg;
    cfg.outputFolder = dir.path();
    cfg.format = TraceFileFormat::VectorAsc;
    cfg.preTriggerSeconds = 1.0;
    cfg.postTriggerSeconds = 0.0;
    cfg.oneShot = true;

    TriggerRule rule;
    rule.name = QStringLiteral("FaultBit");
    rule.kind = TriggerRule::Kind::FrameMatch;
    rule.idMin = 0x201;
    rule.idMax = 0x208;
    rule.byteIndex = 0;
    rule.byteMask = 0x80;
    rule.byteValue = 0x80;
    rule.byteOp = TriggerRule::CompareOp::Equal;
    cfg.rules.append(rule);

    mgr.setConfig(cfg);
    QSignalSpy savedSpy(&mgr, &TriggerCaptureManager::captureSaved);
    mgr.setArmed(true);

    mgr.enqueue(dataFrame(100, 0x202, 0x01));
    mgr.enqueue(dataFrame(200, 0x202, 0x85)); // bit 7 set -> fires!
    mgr.processNow();

    QCOMPARE(savedSpy.count(), 1);
    const TraceReadResult readBack = TraceFileReader::readFile(savedSpy.at(0).at(0).toString());
    QVERIFY2(readBack.ok, qPrintable(readBack.errorMessage));
    QCOMPARE(readBack.messages.size(), 2);
}

void TriggerCaptureTest::watchdogTimeoutTriggersOnSilentId()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());

    TriggerCaptureManager mgr(ifaceName);
    TriggerCaptureConfig cfg;
    cfg.outputFolder = dir.path();
    cfg.format = TraceFileFormat::Csv;
    cfg.preTriggerSeconds = 2.0;
    cfg.postTriggerSeconds = 0.0;
    cfg.oneShot = true;

    TriggerRule rule;
    rule.name = QStringLiteral("Motor1Lost");
    rule.kind = TriggerRule::Kind::Watchdog;
    rule.idMin = 0x201;
    rule.idMax = 0x201;
    rule.watchdogTimeoutMs = 100;
    cfg.rules.append(rule);

    mgr.setConfig(cfg);
    QSignalSpy trigSpy(&mgr, &TriggerCaptureManager::triggered);
    QSignalSpy savedSpy(&mgr, &TriggerCaptureManager::captureSaved);
    mgr.setArmed(true);

    mgr.enqueue(dataFrame(100, 0x201, 0x11));
    mgr.enqueue(dataFrame(150, 0x202, 0x22));
    mgr.processNow();
    QCOMPARE(trigSpy.count(), 0);

    // Advance clock 250ms past 100ms (> 100ms timeout)
    mgr.checkWatchdogsAt(k_baseUs + 350LL * 1000LL);
    QCOMPARE(trigSpy.count(), 1);
    QCOMPARE(savedSpy.count(), 1);
}

void TriggerCaptureTest::signalThresholdTriggersViaEvaluator()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());

    auto eval = [](const BusMessage &msg, const QString &, const QString &, double &out) -> bool {
        if (msg.getId() != 0x201 || msg.getLength() < 1) return false;
        out = static_cast<double>(msg.getByte(0));
        return true;
    };

    TriggerCaptureManager mgr(ifaceName, eval);
    TriggerCaptureConfig cfg;
    cfg.outputFolder = dir.path();
    cfg.format = TraceFileFormat::Csv;
    cfg.preTriggerSeconds = 1.0;
    cfg.postTriggerSeconds = 0.0;
    cfg.oneShot = true;

    TriggerRule rule;
    rule.name = QStringLiteral("OverTemp");
    rule.kind = TriggerRule::Kind::SignalThreshold;
    rule.signalMessageName = QStringLiteral("Motor1");
    rule.signalName = QStringLiteral("Temp");
    rule.signalOp = TriggerRule::CompareOp::GreaterEqual;
    rule.signalThreshold = 80.0;
    cfg.rules.append(rule);

    mgr.setConfig(cfg);
    QSignalSpy savedSpy(&mgr, &TriggerCaptureManager::captureSaved);
    mgr.setArmed(true);

    mgr.enqueue(dataFrame(100, 0x201, 60));
    mgr.enqueue(dataFrame(200, 0x201, 85));
    mgr.processNow();

    QCOMPARE(savedSpy.count(), 1);
}

QTEST_MAIN(TriggerCaptureTest)
#include "TriggerCaptureTest.moc"
