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

// End-to-end tests for the built-in motor DBC generator: the generated text has
// to be accepted by the normal DBC parser and the parsed signals have to decode
// exactly like MotorDecoder does.

#include <QtTest>

#include <cmath>
#include <memory>

#include "core/BusMessage.h"
#include "db/model/CanDb.h"
#include "db/model/CanDbMessage.h"
#include "db/model/CanDbSignal.h"
#include "decoders/ProtocolManager.h"
#include "decoders/motor/MotorDecoder.h"
#include "decoders/motor/MotorDbc.h"

namespace {

constexpr qint64 k_timestampUs = 1757000000000000LL;

[[nodiscard]] bool near(double actual, double expected)
{
    return std::abs(actual - expected) <= 1e-9;
}

[[nodiscard]] BusMessage feedbackFrame(quint32 id, const quint8 (&bytes)[8])
{
    BusMessage frame(id);
    frame.setData(bytes[0], bytes[1], bytes[2], bytes[3],
                  bytes[4], bytes[5], bytes[6], bytes[7]);
    frame.setTimestamp_us(k_timestampUs);
    frame.setRX(true);
    return frame;
}

[[nodiscard]] MotorConfig makeMotor(MotorModel model, quint32 feedbackId, const QString &name)
{
    MotorConfig config;
    config.model = model;
    config.feedbackId = feedbackId;
    config.name = name;
    return config;
}

/// Decodes through the same decoder chain the application uses.
[[nodiscard]] bool decodeWithMotorDecoder(const MotorConfig &config,
                                          const BusMessage &frame,
                                          MotorState *state)
{
    auto decoder = std::make_shared<MotorDecoder>();
    decoder->setMotors({config});

    ProtocolManager manager;
    manager.addDecoder(decoder);

    ProtocolMessage message;
    if (manager.processFrame(frame, message) != DecodeStatus::Completed) {
        return false;
    }

    bool ok = false;
    *state = motorStateFromProtocolMessage(message, nullptr, &ok);
    return ok;
}

[[nodiscard]] CanDbSignal *signalOf(const pCanDb &db, quint32 id, const QString &name)
{
    if (!db) {
        return nullptr;
    }
    CanDbMessage *message = db->getMessageById(id);
    return message ? message->getSignalByName(name) : nullptr;
}

} // namespace

class MotorDbcTest : public QObject
{
    Q_OBJECT

private slots:
    void emptyConfigurationGeneratesNothing();

    void buildsM3508Database();
    void m3508DecodingMatchesMotorDecoder();
    void m2006HasRawTorqueAndNoTemperature();
    void gm6020CurrentScale();
    void damiaoDecodingMatchesMotorDecoder();
    void damiaoRangesFollowConfiguration();

    void disabledMotorsAreSkipped();
    void rejectsDuplicateFeedbackId();
    void rejectsOutOfRangeFeedbackId();
    void rejectsDuplicateNames();
    void sanitizesAndDeduplicatesMessageNames();
    void buildTextUsesDecimalIdsAndParses();
};

void MotorDbcTest::emptyConfigurationGeneratesNothing()
{
    QString error = QStringLiteral("sentinel");
    QVERIFY(MotorDbc::buildText({}, &error).isEmpty());
    QVERIFY(error.isEmpty());

    error = QStringLiteral("sentinel");
    QVERIFY(MotorDbc::buildDatabase({}, &error).isNull());
    QVERIFY(error.isEmpty());
}

void MotorDbcTest::buildsM3508Database()
{
    const MotorConfig config = makeMotor(MotorModel::DjiM3508, 0x201, QStringLiteral("Chassis1"));
    QString error;
    const pCanDb db = MotorDbc::buildDatabase({config}, &error);
    QVERIFY2(db, qPrintable(error));
    QVERIFY(error.isEmpty());
    QCOMPARE(db->getNumberOfMessages(), size_t(1));

    CanDbMessage *message = db->getMessageById(0x201);
    QVERIFY(message);
    QCOMPARE(message->getName(), QStringLiteral("Chassis1"));
    QCOMPARE(message->getDlc(), uint8_t(8));
    QVERIFY(message->getSender() != nullptr);
    QCOMPARE(message->getSender()->name(), QStringLiteral("Node1"));
    QCOMPARE(message->getComment(), QStringLiteral("DJI M3508 feedback"));
    QCOMPARE(message->getSignals().size(), 4);

    CanDbSignal *angle = message->getSignalByName(QStringLiteral("Angle"));
    QVERIFY(angle);
    QCOMPARE(angle->startBit(), uint16_t(0));
    QCOMPARE(angle->dbcStartBit(), uint16_t(7));
    QCOMPARE(angle->length(), uint16_t(16));
    QVERIFY(angle->isBigEndian());
    QVERIFY(angle->isUnsigned());
    QCOMPARE(angle->getFactor(), 0.0439453125);
    QCOMPARE(angle->getOffset(), 0.0);
    QCOMPARE(angle->getMinimumValue(), 0.0);
    QCOMPARE(angle->getMaximumValue(), 359.9560546875);
    QCOMPARE(angle->getUnit(), QStringLiteral("deg"));

    CanDbSignal *speed = message->getSignalByName(QStringLiteral("Speed"));
    QVERIFY(speed);
    QCOMPARE(speed->dbcStartBit(), uint16_t(23));
    QCOMPARE(speed->length(), uint16_t(16));
    QVERIFY(!speed->isUnsigned());
    QCOMPARE(speed->getFactor(), 1.0);
    QCOMPARE(speed->getOffset(), 0.0);
    QCOMPARE(speed->getMinimumValue(), -32768.0);
    QCOMPARE(speed->getMaximumValue(), 32767.0);
    QCOMPARE(speed->getUnit(), QStringLiteral("rpm"));

    CanDbSignal *current = message->getSignalByName(QStringLiteral("Current"));
    QVERIFY(current);
    QCOMPARE(current->dbcStartBit(), uint16_t(39));
    QVERIFY(!current->isUnsigned());
    QCOMPARE(current->getFactor(), 20.0 / 16384.0);
    QCOMPARE(current->getOffset(), 0.0);
    QCOMPARE(current->getMinimumValue(), -20.0);
    QCOMPARE(current->getMaximumValue(), 20.0);
    QCOMPARE(current->getUnit(), QStringLiteral("A"));

    CanDbSignal *temperature = message->getSignalByName(QStringLiteral("Temperature"));
    QVERIFY(temperature);
    QCOMPARE(temperature->dbcStartBit(), uint16_t(55));
    QCOMPARE(temperature->length(), uint16_t(8));
    QVERIFY(temperature->isUnsigned());
    QCOMPARE(temperature->getUnit(), QStringLiteral("degC"));
}

void MotorDbcTest::m3508DecodingMatchesMotorDecoder()
{
    const MotorConfig config = makeMotor(MotorModel::DjiM3508, 0x201, QStringLiteral("Chassis1"));
    const pCanDb db = MotorDbc::buildDatabase({config});
    QVERIFY(db);

    // 280 raw angle, 3000 rpm, 1000 raw current, 35 deg C.
    const quint8 bytes[8] = {0x01, 0x18, 0x0B, 0xB8, 0x03, 0xE8, 0x23, 0xA5};
    const BusMessage frame = feedbackFrame(0x201, bytes);

    CanDbSignal *angle = signalOf(db, 0x201, QStringLiteral("Angle"));
    CanDbSignal *speed = signalOf(db, 0x201, QStringLiteral("Speed"));
    CanDbSignal *current = signalOf(db, 0x201, QStringLiteral("Current"));
    CanDbSignal *temperature = signalOf(db, 0x201, QStringLiteral("Temperature"));
    QVERIFY(angle && speed && current && temperature);

    QVERIFY(near(angle->extractPhysicalFromMessage(frame), 12.3046875));
    QVERIFY(near(speed->extractPhysicalFromMessage(frame), 3000.0));
    QVERIFY(near(current->extractPhysicalFromMessage(frame), 1.220703125));
    QVERIFY(near(temperature->extractPhysicalFromMessage(frame), 35.0));

    MotorState state;
    QVERIFY(decodeWithMotorDecoder(config, frame, &state));
    QVERIFY(state.hasData);
    QCOMPARE(state.timestampUs, k_timestampUs);
    QVERIFY(near(state.angleDeg, angle->extractPhysicalFromMessage(frame)));
    QVERIFY(near(state.speedRpm, speed->extractPhysicalFromMessage(frame)));
    QVERIFY(near(state.currentA, current->extractPhysicalFromMessage(frame)));
    QVERIFY(near(state.temperatureC, temperature->extractPhysicalFromMessage(frame)));
}

void MotorDbcTest::m2006HasRawTorqueAndNoTemperature()
{
    const MotorConfig config = makeMotor(MotorModel::DjiM2006, 0x202, QStringLiteral("Intake"));
    const pCanDb db = MotorDbc::buildDatabase({config});
    QVERIFY(db);

    CanDbMessage *message = db->getMessageById(0x202);
    QVERIFY(message);
    QCOMPARE(message->getSignals().size(), 3);

    CanDbSignal *torque = message->getSignalByName(QStringLiteral("TorqueRaw"));
    QVERIFY(torque);
    QCOMPARE(torque->dbcStartBit(), uint16_t(39));
    QCOMPARE(torque->length(), uint16_t(16));
    QVERIFY(!torque->isUnsigned());
    QCOMPARE(torque->getFactor(), 1.0);
    QCOMPARE(torque->getOffset(), 0.0);
    QVERIFY(torque->getUnit().isEmpty());
    QVERIFY(!message->getSignalByName(QStringLiteral("Current")));
    QVERIFY(!message->getSignalByName(QStringLiteral("Temperature")));

    // Raw C610 torque is passed through unchanged.
    const quint8 bytes[8] = {0x08, 0x00, 0x01, 0x2C, 0x03, 0x20, 40, 0x00};
    QCOMPARE(torque->extractPhysicalFromMessage(feedbackFrame(0x202, bytes)), 800.0);
}

void MotorDbcTest::gm6020CurrentScale()
{
    const MotorConfig config = makeMotor(MotorModel::DjiGM6020, 0x205, QStringLiteral("GimbalYaw"));
    const pCanDb db = MotorDbc::buildDatabase({config});
    QVERIFY(db);

    CanDbSignal *current = signalOf(db, 0x205, QStringLiteral("Current"));
    QVERIFY(current);
    QCOMPARE(current->getFactor(), 3.0 / 16384.0);
    QCOMPARE(current->getOffset(), 0.0);
    QCOMPARE(current->getMinimumValue(), -3.0);
    QCOMPARE(current->getMaximumValue(), 3.0);
    QCOMPARE(current->getUnit(), QStringLiteral("A"));

    // 180 deg, -40 rpm, 1000 raw current, 50 deg C.
    const quint8 bytes[8] = {0x10, 0x00, 0xFF, 0xD8, 0x03, 0xE8, 50, 0x00};
    QVERIFY(near(current->extractPhysicalFromMessage(feedbackFrame(0x205, bytes)),
                 0.18310546875));
}

void MotorDbcTest::damiaoDecodingMatchesMotorDecoder()
{
    const MotorConfig config = makeMotor(MotorModel::DamiaoMit, 0x005, QStringLiteral("Arm1"));
    QString error;
    const pCanDb db = MotorDbc::buildDatabase({config}, &error);
    QVERIFY2(db, qPrintable(error));

    CanDbMessage *message = db->getMessageById(0x005);
    QVERIFY(message);
    QCOMPARE(message->getDlc(), uint8_t(8));
    QVERIFY(message->getSender() != nullptr);
    QCOMPARE(message->getSignals().size(), 6);
    QCOMPARE(message->getComment(), QStringLiteral("DAMIAO MIT feedback"));

    CanDbSignal *stateSignal = message->getSignalByName(QStringLiteral("State"));
    CanDbSignal *position = message->getSignalByName(QStringLiteral("Position"));
    CanDbSignal *velocity = message->getSignalByName(QStringLiteral("Velocity"));
    CanDbSignal *torque = message->getSignalByName(QStringLiteral("Torque"));
    CanDbSignal *mosTemperature = message->getSignalByName(QStringLiteral("MosTemperature"));
    CanDbSignal *rotorTemperature = message->getSignalByName(QStringLiteral("RotorTemperature"));
    QVERIFY(stateSignal && position && velocity && torque && mosTemperature && rotorTemperature);

    QCOMPARE(stateSignal->dbcStartBit(), uint16_t(7));
    QCOMPARE(stateSignal->length(), uint16_t(4));
    QVERIFY(stateSignal->isUnsigned());
    QCOMPARE(stateSignal->getValueName(0), QStringLiteral("Disabled"));
    QCOMPARE(stateSignal->getValueName(1), QStringLiteral("Enabled"));
    QCOMPARE(stateSignal->getValueName(8), QStringLiteral("Overvoltage"));
    QCOMPARE(stateSignal->getValueName(14), QStringLiteral("Overload"));
    QVERIFY(stateSignal->getValueName(2).isEmpty());

    QCOMPARE(position->getUnit(), QStringLiteral("rad"));
    QCOMPARE(velocity->getUnit(), QStringLiteral("rad/s"));
    QCOMPARE(torque->getUnit(), QStringLiteral("N\u00B7m"));
    QCOMPARE(mosTemperature->getUnit(), QStringLiteral("degC"));
    QCOMPARE(rotorTemperature->getUnit(), QStringLiteral("degC"));

    // state 1, motor id 3, position max, velocity max, torque min, MOS 42, rotor 65.
    const quint8 bytes[8] = {0x13, 0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0x2A, 0x41};
    const BusMessage frame = feedbackFrame(0x005, bytes);

    QCOMPARE(stateSignal->extractRawDataFromMessage(frame), quint64(1));
    QVERIFY(near(position->extractPhysicalFromMessage(frame), 12.5));
    QVERIFY(near(velocity->extractPhysicalFromMessage(frame), 30.0));
    QVERIFY(near(torque->extractPhysicalFromMessage(frame), -10.0));
    QVERIFY(near(mosTemperature->extractPhysicalFromMessage(frame), 42.0));
    QVERIFY(near(rotorTemperature->extractPhysicalFromMessage(frame), 65.0));

    MotorState state;
    QVERIFY(decodeWithMotorDecoder(config, frame, &state));
    QCOMPARE(state.damiaoState, quint32(1));
    QVERIFY(near(state.positionRad, position->extractPhysicalFromMessage(frame)));
    QVERIFY(near(state.velocityRadPerSec, velocity->extractPhysicalFromMessage(frame)));
    QVERIFY(near(state.torqueNm, torque->extractPhysicalFromMessage(frame)));
    QVERIFY(near(state.temperatureC, rotorTemperature->extractPhysicalFromMessage(frame)));
    QVERIFY(near(state.mosTemperatureC, mosTemperature->extractPhysicalFromMessage(frame)));
}

void MotorDbcTest::damiaoRangesFollowConfiguration()
{
    MotorConfig config = makeMotor(MotorModel::DamiaoMit, 0x010, QStringLiteral("Arm2"));
    config.damiaoPmax = 2.0;
    config.damiaoVmax = 10.0;
    config.damiaoTmax = 3.0;

    const pCanDb db = MotorDbc::buildDatabase({config});
    QVERIFY(db);

    CanDbSignal *position = signalOf(db, 0x010, QStringLiteral("Position"));
    CanDbSignal *velocity = signalOf(db, 0x010, QStringLiteral("Velocity"));
    CanDbSignal *torque = signalOf(db, 0x010, QStringLiteral("Torque"));
    QVERIFY(position && velocity && torque);

    QVERIFY(std::abs(position->getFactor() - (4.0 / 65535.0)) < 1e-12);
    QVERIFY(std::abs(velocity->getFactor() - (20.0 / 4095.0)) < 1e-12);
    QVERIFY(std::abs(torque->getFactor() - (6.0 / 4095.0)) < 1e-12);
    QCOMPARE(position->getOffset(), -2.0);
    QCOMPARE(velocity->getOffset(), -10.0);
    QCOMPARE(torque->getOffset(), -3.0);
    QCOMPARE(position->getMinimumValue(), -2.0);
    QCOMPARE(position->getMaximumValue(), 2.0);
    QCOMPARE(velocity->getMinimumValue(), -10.0);
    QCOMPARE(velocity->getMaximumValue(), 10.0);
    QCOMPARE(torque->getMinimumValue(), -3.0);
    QCOMPARE(torque->getMaximumValue(), 3.0);

    const quint8 bytes[8] = {0x11, 0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x00};
    const BusMessage frame = feedbackFrame(0x010, bytes);
    QVERIFY(near(position->extractPhysicalFromMessage(frame), 2.0));
    QVERIFY(near(velocity->extractPhysicalFromMessage(frame), 10.0));
    QVERIFY(near(torque->extractPhysicalFromMessage(frame), -3.0));

    MotorState state;
    QVERIFY(decodeWithMotorDecoder(config, frame, &state));
    QVERIFY(near(state.positionRad, position->extractPhysicalFromMessage(frame)));
    QVERIFY(near(state.velocityRadPerSec, velocity->extractPhysicalFromMessage(frame)));
    QVERIFY(near(state.torqueNm, torque->extractPhysicalFromMessage(frame)));
}

void MotorDbcTest::disabledMotorsAreSkipped()
{
    MotorConfig disabled = makeMotor(MotorModel::DjiM3508, 0x201, QStringLiteral("Off"));
    disabled.enabled = false;
    QString error;
    QVERIFY(MotorDbc::buildText({disabled}, &error).isEmpty());
    QVERIFY(error.isEmpty());
    QVERIFY(MotorDbc::buildDatabase({disabled}, &error).isNull());
    QVERIFY(error.isEmpty());

    const MotorConfig enabled = makeMotor(MotorModel::DjiM3508, 0x202, QStringLiteral("On"));
    const pCanDb db = MotorDbc::buildDatabase({disabled, enabled}, &error);
    QVERIFY2(db, qPrintable(error));
    QVERIFY(db->getMessageById(0x202));
    QVERIFY(!db->getMessageById(0x201));
}

void MotorDbcTest::rejectsDuplicateFeedbackId()
{
    const MotorConfig first = makeMotor(MotorModel::DjiM3508, 0x201, QStringLiteral("A"));
    const MotorConfig second = makeMotor(MotorModel::DjiM3508, 0x201, QStringLiteral("B"));
    QString error;
    QVERIFY(MotorDbc::buildText({first, second}, &error).isEmpty());
    QVERIFY(!error.isEmpty());
    QVERIFY(MotorDbc::buildDatabase({first, second}, &error).isNull());
    QVERIFY(!error.isEmpty());
}

void MotorDbcTest::rejectsOutOfRangeFeedbackId()
{
    const MotorConfig config = makeMotor(MotorModel::DjiM3508, 0x800, QStringLiteral("Bad"));
    QString error;
    QVERIFY(MotorDbc::buildText({config}, &error).isEmpty());
    QVERIFY(!error.isEmpty());
    QVERIFY(MotorDbc::buildDatabase({config}, &error).isNull());
    QVERIFY(!error.isEmpty());
}

void MotorDbcTest::rejectsDuplicateNames()
{
    const MotorConfig first = makeMotor(MotorModel::DjiM3508, 0x201, QStringLiteral("Same"));
    const MotorConfig second = makeMotor(MotorModel::DjiM3508, 0x202, QStringLiteral("Same"));
    QString error;
    QVERIFY(MotorDbc::buildText({first, second}, &error).isEmpty());
    QVERIFY(!error.isEmpty());
    QVERIFY(MotorDbc::buildDatabase({first, second}, &error).isNull());
    QVERIFY(!error.isEmpty());
}

void MotorDbcTest::sanitizesAndDeduplicatesMessageNames()
{
    const QVector<MotorConfig> motors = {
        makeMotor(MotorModel::DjiM3508, 0x201, QStringLiteral("Chassis 1#")),
        makeMotor(MotorModel::DjiM3508, 0x202, QStringLiteral("\u5E95\u76D8")),
        makeMotor(MotorModel::DjiM3508, 0x203, QStringLiteral("Chassis-1")),
        makeMotor(MotorModel::DjiM3508, 0x204, QStringLiteral("3508")),
    };

    QString error;
    const QString text = MotorDbc::buildText(motors, &error);
    QVERIFY2(error.isEmpty(), qPrintable(error));
    QVERIFY(text.contains(QStringLiteral("BU_: Node1 Node2 Node3 Node4")));

    const pCanDb db = MotorDbc::buildDatabase(motors, &error);
    QVERIFY2(db, qPrintable(error));
    QCOMPARE(db->getNumberOfMessages(), size_t(4));
    QCOMPARE(db->getMessageById(0x201)->getName(), QStringLiteral("Chassis1"));
    QCOMPARE(db->getMessageById(0x202)->getName(), QStringLiteral("Motor2"));
    QCOMPARE(db->getMessageById(0x203)->getName(), QStringLiteral("Chassis1_2"));
    QCOMPARE(db->getMessageById(0x204)->getName(), QStringLiteral("M3508"));

    for (quint32 id = 0x201; id <= 0x204; ++id) {
        CanDbMessage *message = db->getMessageById(id);
        QVERIFY(message);
        QVERIFY(message->getSender() != nullptr);
    }
}

void MotorDbcTest::buildTextUsesDecimalIdsAndParses()
{
    const MotorConfig config = makeMotor(MotorModel::DjiM3508, 0x201, QStringLiteral("Chassis1"));
    QString error;
    const QString text = MotorDbc::buildText({config}, &error);
    QVERIFY(error.isEmpty());
    QVERIFY(text.contains(QStringLiteral("VERSION \"\"")));
    QVERIFY(text.contains(QStringLiteral("BU_: Node1")));
    QVERIFY(text.contains(QStringLiteral("BO_ 513 Chassis1: 8 Node1")));
    QVERIFY(!text.contains(QStringLiteral("BO_ 0x")));

    const pCanDb db = MotorDbc::buildDatabase({config}, &error);
    QVERIFY2(db, qPrintable(error));
    QVERIFY(db->getMessageById(513));
}

QTEST_APPLESS_MAIN(MotorDbcTest)
#include "MotorDbcTest.moc"
