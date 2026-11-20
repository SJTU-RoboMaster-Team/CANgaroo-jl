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

#include "core/BusMessage.h"
#include "db/model/CanDb.h"
#include "db/model/CanDbMessage.h"
#include "db/model/CanDbSignal.h"
#include "decoders/ProtocolManager.h"
#include "decoders/motor/MotorDecoder.h"
#include "decoders/motor/MotorDbc.h"

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

MotorConfig damiaoConfig(const QString &name, quint32 feedbackId,
                         double pmax = 12.5, double vmax = 30.0, double tmax = 10.0)
{
    MotorConfig config;
    config.name = name;
    config.model = MotorModel::DamiaoMit;
    config.feedbackId = feedbackId;
    config.damiaoPmax = pmax;
    config.damiaoVmax = vmax;
    config.damiaoTmax = tmax;
    return config;
}

bool decodeWithMotorDecoder(const MotorConfig &config, const BusMessage &frame, MotorState *state)
{
    MotorDecoder decoder;
    decoder.setMotors({config});

    ProtocolMessage message;
    if (decoder.tryDecode(frame, message) != DecodeStatus::Completed) {
        return false;
    }
    bool ok = false;
    *state = motorStateFromProtocolMessage(message, nullptr, &ok);
    return ok;
}

CanDbSignal *signalOf(const pCanDb &db, quint32 id, const QString &name)
{
    if (!db) {
        return nullptr;
    }
    CanDbMessage *message = db->getMessageById(id);
    if (!message) {
        return nullptr;
    }
    return message->getSignalByName(name);
}

} // namespace

class MotorDbcTest : public QObject
{
    Q_OBJECT

private slots:
    void emptyConfigurationGeneratesNothing();
    void buildsDjiDatabase();
    void djiDecodingMatchesDecoder();
    void m2006HasRawTorqueAndNoTemperature();
    void gm6020CurrentScale();
    void damiaoDecodingMatchesDecoder();
    void damiaoRangesFollowConfiguration();
    void disabledMotorsAreSkipped();
    void rejectsDuplicateCanIds();
    void rejectsInvalidCanId();
};

void MotorDbcTest::emptyConfigurationGeneratesNothing()
{
    QString error;
    QVERIFY(MotorDbc::buildText({}, &error).isEmpty());
    QVERIFY(error.isEmpty());
    QVERIFY(MotorDbc::buildDatabase({}, &error).isNull());
    QVERIFY(error.isEmpty());
}

void MotorDbcTest::buildsDjiDatabase()
{
    const MotorConfig config = djiConfig(QStringLiteral("FL"), MotorModel::DjiM3508, 0x201);
    QString error;
    const pCanDb db = MotorDbc::buildDatabase({config}, &error);
    QVERIFY2(db, qPrintable(error));
    QVERIFY(error.isEmpty());

    CanDbMessage *message = db->getMessageById(0x201);
    QVERIFY(message);
    QCOMPARE(message->getName(), QStringLiteral("FL"));
    QCOMPARE(message->getDlc(), uint8_t(8));
    QVERIFY(message->getSender());
    QCOMPARE(message->getSignals().size(), 4);

    CanDbSignal *angle = signalOf(db, 0x201, QStringLiteral("Angle"));
    QVERIFY(angle);
    QVERIFY(qAbs(angle->getFactor() - 360.0 / 8192.0) < 1e-12);
    QCOMPARE(angle->getUnit(), QStringLiteral("deg"));
    QVERIFY(angle->isBigEndian());

    CanDbSignal *current = signalOf(db, 0x201, QStringLiteral("Current"));
    QVERIFY(current);
    QVERIFY(qAbs(current->getFactor() - 20.0 / 16384.0) < 1e-12);
    QVERIFY(!current->isUnsigned());

    QVERIFY(signalOf(db, 0x201, QStringLiteral("Speed")));
    QVERIFY(signalOf(db, 0x201, QStringLiteral("Temperature")));
}

void MotorDbcTest::djiDecodingMatchesDecoder()
{
    const MotorConfig config = djiConfig(QStringLiteral("FL"), MotorModel::DjiM3508, 0x201);
    const pCanDb db = MotorDbc::buildDatabase({config});
    QVERIFY(db);

    const BusMessage frame = makeFrame(0x201, 0x01, 0x18, 0x0B, 0xB8, 0x03, 0xE8, 0x23, 0xA5);
    CanDbSignal *angle = signalOf(db, 0x201, QStringLiteral("Angle"));
    CanDbSignal *speed = signalOf(db, 0x201, QStringLiteral("Speed"));
    CanDbSignal *current = signalOf(db, 0x201, QStringLiteral("Current"));
    CanDbSignal *temperature = signalOf(db, 0x201, QStringLiteral("Temperature"));
    QVERIFY(angle && speed && current && temperature);

    QVERIFY(qAbs(angle->extractPhysicalFromMessage(frame) - 12.3046875) < 1e-9);
    QVERIFY(qAbs(speed->extractPhysicalFromMessage(frame) - 3000.0) < 1e-9);
    QVERIFY(qAbs(current->extractPhysicalFromMessage(frame) - 1.220703125) < 1e-9);
    QVERIFY(qAbs(temperature->extractPhysicalFromMessage(frame) - 35.0) < 1e-9);

    MotorState state;
    QVERIFY(decodeWithMotorDecoder(config, frame, &state));
    QVERIFY(qAbs(state.angleDeg - angle->extractPhysicalFromMessage(frame)) < 1e-9);
    QVERIFY(qAbs(state.speedRpm - speed->extractPhysicalFromMessage(frame)) < 1e-9);
    QVERIFY(qAbs(state.currentA - current->extractPhysicalFromMessage(frame)) < 1e-9);
    QVERIFY(qAbs(state.temperatureC - temperature->extractPhysicalFromMessage(frame)) < 1e-9);
}

void MotorDbcTest::m2006HasRawTorqueAndNoTemperature()
{
    const MotorConfig config = djiConfig(QStringLiteral("M2006"), MotorModel::DjiM2006, 0x202);
    const pCanDb db = MotorDbc::buildDatabase({config});
    QVERIFY(db);

    CanDbSignal *torque = signalOf(db, 0x202, QStringLiteral("TorqueRaw"));
    QVERIFY(torque);
    QCOMPARE(torque->getFactor(), 1.0);
    QVERIFY(!signalOf(db, 0x202, QStringLiteral("Current")));
    QVERIFY(!signalOf(db, 0x202, QStringLiteral("Temperature")));
}

void MotorDbcTest::gm6020CurrentScale()
{
    const MotorConfig config = djiConfig(QStringLiteral("GM"), MotorModel::DjiGM6020, 0x205);
    const pCanDb db = MotorDbc::buildDatabase({config});
    QVERIFY(db);

    CanDbSignal *current = signalOf(db, 0x205, QStringLiteral("Current"));
    QVERIFY(current);
    QVERIFY(qAbs(current->getFactor() - 3.0 / 16384.0) < 1e-12);
}

void MotorDbcTest::damiaoDecodingMatchesDecoder()
{
    const MotorConfig config = damiaoConfig(QStringLiteral("DM"), 0x005);
    QString error;
    const pCanDb db = MotorDbc::buildDatabase({config}, &error);
    QVERIFY2(db, qPrintable(error));

    CanDbMessage *message = db->getMessageById(0x005);
    QVERIFY(message);
    QCOMPARE(message->getSignals().size(), 6);
    QVERIFY(message->getSignalByName(QStringLiteral("State"))->getValueName(1) == QStringLiteral("Enabled"));

    const BusMessage frame = makeFrame(0x005, 0x13, 0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0x2A, 0x41);
    CanDbSignal *state = signalOf(db, 0x005, QStringLiteral("State"));
    CanDbSignal *position = signalOf(db, 0x005, QStringLiteral("Position"));
    CanDbSignal *velocity = signalOf(db, 0x005, QStringLiteral("Velocity"));
    CanDbSignal *torque = signalOf(db, 0x005, QStringLiteral("Torque"));
    CanDbSignal *mos = signalOf(db, 0x005, QStringLiteral("MosTemperature"));
    CanDbSignal *rotor = signalOf(db, 0x005, QStringLiteral("RotorTemperature"));
    QVERIFY(state && position && velocity && torque && mos && rotor);

    QCOMPARE(state->extractRawDataFromMessage(frame), quint64(1));
    QVERIFY(qAbs(position->extractPhysicalFromMessage(frame) - 12.5) < 1e-9);
    QVERIFY(qAbs(velocity->extractPhysicalFromMessage(frame) - 30.0) < 1e-9);
    QVERIFY(qAbs(torque->extractPhysicalFromMessage(frame) - (-10.0)) < 1e-9);
    QVERIFY(qAbs(mos->extractPhysicalFromMessage(frame) - 42.0) < 1e-9);
    QVERIFY(qAbs(rotor->extractPhysicalFromMessage(frame) - 65.0) < 1e-9);

    MotorState motorState;
    QVERIFY(decodeWithMotorDecoder(config, frame, &motorState));
    QCOMPARE(motorState.damiaoState, quint32(1));
    QVERIFY(qAbs(motorState.positionRad - position->extractPhysicalFromMessage(frame)) < 1e-9);
    QVERIFY(qAbs(motorState.velocityRadPerSec - velocity->extractPhysicalFromMessage(frame)) < 1e-9);
    QVERIFY(qAbs(motorState.torqueNm - torque->extractPhysicalFromMessage(frame)) < 1e-9);
    QVERIFY(qAbs(motorState.temperatureC - rotor->extractPhysicalFromMessage(frame)) < 1e-9);
    QVERIFY(qAbs(motorState.mosTemperatureC - mos->extractPhysicalFromMessage(frame)) < 1e-9);
}

void MotorDbcTest::damiaoRangesFollowConfiguration()
{
    const MotorConfig config = damiaoConfig(QStringLiteral("DM"), 0x006, 2.0, 10.0, 3.0);
    const pCanDb db = MotorDbc::buildDatabase({config});
    QVERIFY(db);

    CanDbSignal *position = signalOf(db, 0x006, QStringLiteral("Position"));
    CanDbSignal *velocity = signalOf(db, 0x006, QStringLiteral("Velocity"));
    CanDbSignal *torque = signalOf(db, 0x006, QStringLiteral("Torque"));
    QVERIFY(position && velocity && torque);
    QVERIFY(qAbs(position->getFactor() - (4.0 / 65535.0)) < 1e-12);
    QVERIFY(qAbs(velocity->getFactor() - (20.0 / 4095.0)) < 1e-12);
    QVERIFY(qAbs(torque->getFactor() - (6.0 / 4095.0)) < 1e-12);

    const BusMessage frame = makeFrame(0x006, 0x11, 0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x00);
    MotorState state;
    QVERIFY(decodeWithMotorDecoder(config, frame, &state));
    QVERIFY(qAbs(position->extractPhysicalFromMessage(frame) - state.positionRad) < 1e-9);
    QVERIFY(qAbs(velocity->extractPhysicalFromMessage(frame) - state.velocityRadPerSec) < 1e-9);
    QVERIFY(qAbs(torque->extractPhysicalFromMessage(frame) - state.torqueNm) < 1e-9);
}

void MotorDbcTest::disabledMotorsAreSkipped()
{
    MotorConfig disabled = djiConfig(QStringLiteral("off"), MotorModel::DjiM3508, 0x201);
    disabled.enabled = false;
    QString error;
    QVERIFY(MotorDbc::buildText({disabled}, &error).isEmpty());
    QVERIFY(error.isEmpty());

    MotorConfig enabled = djiConfig(QStringLiteral("on"), MotorModel::DjiM3508, 0x202);
    const pCanDb db = MotorDbc::buildDatabase({disabled, enabled}, &error);
    QVERIFY2(db, qPrintable(error));
    QVERIFY(db->getMessageById(0x202));
    QVERIFY(!db->getMessageById(0x201));
}

void MotorDbcTest::rejectsDuplicateCanIds()
{
    const MotorConfig first = djiConfig(QStringLiteral("A"), MotorModel::DjiM3508, 0x201);
    const MotorConfig second = djiConfig(QStringLiteral("B"), MotorModel::DjiM3508, 0x201);
    QString error;
    QVERIFY(MotorDbc::buildText({first, second}, &error).isEmpty());
    QVERIFY(!error.isEmpty());
    QVERIFY(MotorDbc::buildDatabase({first, second}, &error).isNull());
    QVERIFY(!error.isEmpty());
}

void MotorDbcTest::rejectsInvalidCanId()
{
    const MotorConfig config = djiConfig(QStringLiteral("bad"), MotorModel::DjiM3508, 0x800);
    QString error;
    QVERIFY(MotorDbc::buildText({config}, &error).isEmpty());
    QVERIFY(!error.isEmpty());
}

QTEST_APPLESS_MAIN(MotorDbcTest)
#include "MotorDbcTest.moc"
