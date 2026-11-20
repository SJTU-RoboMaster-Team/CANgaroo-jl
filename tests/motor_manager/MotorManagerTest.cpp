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

#include <QSignalSpy>

#include "core/BusMessage.h"
#include "core/MotorManager.h"
#include "decoders/motor/MotorTypes.h"

namespace {

BusMessage makeFrame(quint32 id, quint8 d0, quint8 d1, quint8 d2, quint8 d3,
                     quint8 d4, quint8 d5, quint8 d6, quint8 d7)
{
    BusMessage frame(id);
    frame.setLength(8);
    frame.setByte(0, d0);
    frame.setByte(1, d1);
    frame.setByte(2, d2);
    frame.setByte(3, d3);
    frame.setByte(4, d4);
    frame.setByte(5, d5);
    frame.setByte(6, d6);
    frame.setByte(7, d7);
    frame.setTimestamp_us(1000);
    return frame;
}

MotorConfig djiConfig(const QString &name, MotorModel model, quint32 feedbackId)
{
    MotorConfig config;
    config.name = name;
    config.model = model;
    config.feedbackId = feedbackId;
    return config;
}

} // namespace

class MotorManagerTest : public QObject
{
    Q_OBJECT

private slots:
    void decodesDjiFeedback();
    void decodesDamiaoFeedback();
    void ignoresFramesOfDisabledMotors();
    void publishesStateUpdates();
    void resetsDecodedValues();
    void marksMotorOfflineWhenFeedbackStops();
    void invertFlipsAngleSpeedAndCurrent();
};

void MotorManagerTest::decodesDjiFeedback()
{
    MotorManager manager;
    manager.setMotors({djiConfig(QStringLiteral("FL"), MotorModel::DjiM3508, 0x201)});

    manager.enqueue(makeFrame(0x201, 0x01, 0x18, 0x0B, 0xB8, 0x03, 0xE8, 0x23, 0xA5));
    QTest::qWait(80);

    const QVector<MotorState> states = manager.states();
    QCOMPARE(states.size(), 1);
    const MotorState &state = states.at(0);
    QVERIFY(state.hasData);
    QVERIFY(state.online);
    QCOMPARE(state.timestampUs, qint64(1000));
    QVERIFY(qAbs(state.angleDeg - 12.3046875) < 1e-9);
    QCOMPARE(state.speedRpm, 3000.0);
    QVERIFY(qAbs(state.currentA - 1.220703125) < 1e-9);
    QCOMPARE(state.temperatureC, 35.0);
    QVERIFY(state.currentValid);
    QVERIFY(state.temperatureValid);
}

void MotorManagerTest::decodesDamiaoFeedback()
{
    MotorManager manager;
    MotorConfig config;
    config.name = QStringLiteral("DM");
    config.model = MotorModel::DamiaoMit;
    config.feedbackId = 0x005;
    manager.setMotors({config});

    manager.enqueue(makeFrame(0x005, 0x13, 0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0x2A, 0x41));
    QTest::qWait(80);

    const QVector<MotorState> states = manager.states();
    QCOMPARE(states.size(), 1);
    const MotorState &state = states.at(0);
    QVERIFY(state.hasData);
    QCOMPARE(state.damiaoState, quint32(1));
    QVERIFY(qAbs(state.positionRad - 12.5) < 1e-9);
    QVERIFY(qAbs(state.velocityRadPerSec - 30.0) < 1e-9);
    QVERIFY(qAbs(state.torqueNm - (-10.0)) < 1e-9);
    QCOMPARE(state.temperatureC, 65.0);
    QCOMPARE(state.mosTemperatureC, 42.0);
    QVERIFY(!state.currentValid);
}

void MotorManagerTest::ignoresFramesOfDisabledMotors()
{
    MotorManager manager;
    MotorConfig config = djiConfig(QStringLiteral("off"), MotorModel::DjiM3508, 0x201);
    config.enabled = false;
    manager.setMotors({config});

    manager.enqueue(makeFrame(0x201, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00));
    QTest::qWait(80);

    const QVector<MotorState> states = manager.states();
    QCOMPARE(states.size(), 1);
    QVERIFY(!states.at(0).hasData);
}

void MotorManagerTest::publishesStateUpdates()
{
    MotorManager manager;
    manager.setMotors({djiConfig(QStringLiteral("FL"), MotorModel::DjiM3508, 0x201)});

    QSignalSpy spy(&manager, &MotorManager::statesUpdated);
    manager.enqueue(makeFrame(0x201, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1E, 0x00));

    QTRY_VERIFY_WITH_TIMEOUT(spy.count() > 0, 500);
}

void MotorManagerTest::resetsDecodedValues()
{
    MotorManager manager;
    manager.setMotors({djiConfig(QStringLiteral("FL"), MotorModel::DjiM3508, 0x201)});
    manager.enqueue(makeFrame(0x201, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1E, 0x00));
    QTest::qWait(80);
    QVERIFY(manager.states().at(0).hasData);

    manager.resetStates();
    QTest::qWait(80);

    const QVector<MotorState> states = manager.states();
    QCOMPARE(states.size(), 1);
    QVERIFY(!states.at(0).hasData);
}

void MotorManagerTest::marksMotorOfflineWhenFeedbackStops()
{
    MotorManager manager;
    manager.setMotors({djiConfig(QStringLiteral("FL"), MotorModel::DjiM3508, 0x201)});
    manager.enqueue(makeFrame(0x201, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1E, 0x00));
    QTest::qWait(80);
    QVERIFY(manager.states().at(0).online);

    QTest::qWait(MotorManager::offlineTimeoutMs + 200);

    const QVector<MotorState> states = manager.states();
    QVERIFY(states.at(0).hasData);
    QVERIFY(!states.at(0).online);
}

void MotorManagerTest::invertFlipsAngleSpeedAndCurrent()
{
    MotorManager manager;
    MotorConfig config = djiConfig(QStringLiteral("inv"), MotorModel::DjiM3508, 0x201);
    config.invert = true;
    manager.setMotors({config});

    // angle 8192/2 = 180 deg, speed 3000 rpm, current 1.22 A
    manager.enqueue(makeFrame(0x201, 0x10, 0x00, 0x0B, 0xB8, 0x03, 0xE8, 0x23, 0x00));
    QTest::qWait(80);

    const MotorState &state = manager.states().at(0);
    QVERIFY(qAbs(state.angleDeg - 180.0) < 1e-9);
    QCOMPARE(state.speedRpm, -3000.0);
    QVERIFY(qAbs(state.currentA - (-1.220703125)) < 1e-9);
}

QTEST_MAIN(MotorManagerTest)
#include "MotorManagerTest.moc"
