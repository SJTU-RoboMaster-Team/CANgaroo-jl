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

#include <cmath>
#include <memory>

#include <QByteArray>
#include <QVariantMap>

#include "decoders/ProtocolManager.h"
#include "decoders/motor/MotorDecoder.h"
#include "decoders/motor/MotorTypes.h"

namespace {

constexpr qint64 k_timestampUs = 1757000000000000LL;

// DJI angle 280 raw steps, speed 3000 rpm, current 1000 raw, 35 deg C.
constexpr quint8 k_m3508Positive[8] = {0x01, 0x18, 0x0B, 0xB8, 0x03, 0xE8, 35, 0xA5};
// Same layout with negative speed (-120 rpm) and current (-200 raw).
constexpr quint8 k_m3508Negative[8] = {0x00, 0x00, 0xFF, 0x88, 0xFF, 0x38, 41, 0x00};

[[nodiscard]] bool near(double actual, double expected)
{
    return std::abs(actual - expected) <= 1e-9;
}

/// Classic CAN feedback frame: 8 byte payload, standard 11-bit identifier.
[[nodiscard]] BusMessage feedbackFrame(quint32 id, const quint8 (&bytes)[8])
{
    BusMessage msg(id);
    msg.setData(bytes[0], bytes[1], bytes[2], bytes[3], bytes[4], bytes[5], bytes[6], bytes[7]);
    msg.setTimestamp_us(k_timestampUs);
    msg.setRX(true);
    return msg;
}

/// DAMIAO MIT feedback: d0 = state<<4 | motor id, 12 bit velocity/torque fields.
[[nodiscard]] BusMessage damiaoFrame(quint32 id, quint8 state, quint8 motorId,
                                     quint16 positionRaw, quint16 velocityRaw,
                                     quint16 torqueRaw, quint8 mosTemperature,
                                     quint8 rotorTemperature)
{
    const quint8 d0 = static_cast<quint8>((state << 4) | (motorId & 0x0Fu));
    const quint8 d1 = static_cast<quint8>(positionRaw >> 8);
    const quint8 d2 = static_cast<quint8>(positionRaw & 0xFFu);
    const quint8 d3 = static_cast<quint8>((velocityRaw >> 4) & 0xFFu);
    const quint8 d4 = static_cast<quint8>(((velocityRaw & 0x0Fu) << 4)
                                          | ((torqueRaw >> 8) & 0x0Fu));
    const quint8 d5 = static_cast<quint8>(torqueRaw & 0xFFu);
    const quint8 bytes[8] = {d0, d1, d2, d3, d4, d5, mosTemperature, rotorTemperature};
    return feedbackFrame(id, bytes);
}

[[nodiscard]] MotorConfig makeMotor(MotorModel model, quint32 feedbackId, const QString &name)
{
    MotorConfig config;
    config.model = model;
    config.feedbackId = feedbackId;
    config.name = name;
    return config;
}

[[nodiscard]] MotorState decodeSingle(MotorDecoder &decoder, const BusMessage &frame)
{
    ProtocolMessage msg;
    const DecodeStatus status = decoder.tryDecode(frame, msg);
    if (status != DecodeStatus::Completed) {
        return MotorState();
    }
    return motorStateFromProtocolMessage(msg);
}

} // namespace

class MotorDecoderTest : public QObject
{
    Q_OBJECT

private slots:
    void djiM3508DecodesPositiveFields();
    void djiM3508DecodesNegativeFields();
    void djiM2006KeepsRawCurrentAndHasNoTemperature();
    void djiGm6020UsesThreeAmpCurrentRange();
    void damiaoMitDecodesPackedFields();
    void damiaoMitDecodesStatusAndPositiveTorque();
    void damiaoMitUsesConfiguredRanges();
    void ignoresDisabledOrUnconfiguredMotors();
    void ignoresMalformedFrames();
    void fillsProtocolMessageAndMetadata();
    void protocolManagerRoutesAndAssignsGlobalIndex();
    void motorStateFromProtocolMessageRequiresMotorIndex();
    void resetKeepsConfigurationUsable();
};

void MotorDecoderTest::djiM3508DecodesPositiveFields()
{
    MotorDecoder decoder;
    decoder.setMotors({makeMotor(MotorModel::DjiM3508, 0x201, QStringLiteral("Chassis1"))});
    QVERIFY(decoder.motors().size() == 1);

    ProtocolMessage msg;
    const BusMessage frame = feedbackFrame(0x201, k_m3508Positive);
    QVERIFY(decoder.tryDecode(frame, msg) == DecodeStatus::Completed);

    const MotorState state = motorStateFromProtocolMessage(msg);
    QVERIFY(state.hasData);
    QVERIFY(state.online);
    QCOMPARE(state.timestampUs, k_timestampUs);
    // 280 * 360 / 8192
    QVERIFY(near(state.angleDeg, 12.3046875));
    QCOMPARE(state.speedRpm, 3000.0);
    // 1000 * 20 / 16384
    QVERIFY(near(state.currentA, 1.220703125));
    QCOMPARE(state.temperatureC, 35.0);
    QVERIFY(state.currentValid);
    QVERIFY(state.temperatureValid);
    QCOMPARE(state.rawAngle, 280u);
    QCOMPARE(state.rawSpeed, 3000);
    QCOMPARE(state.rawCurrent, 1000);
    QCOMPARE(state.rawTemperature, 35);
    // DJI feedback carries no angles in radians, no torque and no MOS temperature.
    QCOMPARE(state.positionRad, 0.0);
    QCOMPARE(state.velocityRadPerSec, 0.0);
    QCOMPARE(state.torqueNm, 0.0);
    QCOMPARE(state.damiaoState, 0u);
    QCOMPARE(state.mosTemperatureC, 0.0);
}

void MotorDecoderTest::djiM3508DecodesNegativeFields()
{
    MotorDecoder decoder;
    decoder.setMotors({makeMotor(MotorModel::DjiM3508, 0x202, QStringLiteral("Chassis2"))});

    const MotorState state = decodeSingle(decoder, feedbackFrame(0x202, k_m3508Negative));
    QVERIFY(state.hasData);
    QCOMPARE(state.angleDeg, 0.0);
    QCOMPARE(state.speedRpm, -120.0);
    // -200 * 20 / 16384
    QVERIFY(near(state.currentA, -0.244140625));
    QCOMPARE(state.temperatureC, 41.0);
    QCOMPARE(state.rawAngle, 0u);
    QCOMPARE(state.rawSpeed, -120);
    QCOMPARE(state.rawCurrent, -200);
}

void MotorDecoderTest::djiM2006KeepsRawCurrentAndHasNoTemperature()
{
    MotorDecoder decoder;
    decoder.setMotors({makeMotor(MotorModel::DjiM2006, 0x201, QStringLiteral("Intake"))});

    // 2048 / 8192 * 360 = 90 deg, 300 rpm, current raw 800, d6 = 40.
    const quint8 bytes[8] = {0x08, 0x00, 0x01, 0x2C, 0x03, 0x20, 40, 0x00};
    ProtocolMessage msg;
    QVERIFY(decoder.tryDecode(feedbackFrame(0x201, bytes), msg) == DecodeStatus::Completed);

    const MotorState state = motorStateFromProtocolMessage(msg);
    QVERIFY(near(state.angleDeg, 90.0));
    QCOMPARE(state.speedRpm, 300.0);
    // C610 manual has no amps conversion: raw value is kept, flagged invalid.
    QVERIFY(!state.currentValid);
    QCOMPARE(state.currentA, 800.0);
    QCOMPARE(state.rawCurrent, 800);
    // C610 feedback has no temperature byte: value is kept raw but flagged invalid.
    QVERIFY(!state.temperatureValid);
    QCOMPARE(state.temperatureC, 40.0);
    QCOMPARE(state.rawTemperature, 40);
    QCOMPARE(msg.description,
             QStringLiteral("M2006 angle 90.0 deg speed 300 rpm current raw 800 temperature n/a"));
}

void MotorDecoderTest::djiGm6020UsesThreeAmpCurrentRange()
{
    MotorDecoder decoder;
    decoder.setMotors({makeMotor(MotorModel::DjiGM6020, 0x205, QStringLiteral("GimbalYaw"))});

    // angle raw 4096 = 180 deg, speed -40 rpm, current 1000 raw, 50 deg C.
    const quint8 bytes[8] = {0x10, 0x00, 0xFF, 0xD8, 0x03, 0xE8, 50, 0x00};
    const MotorState state = decodeSingle(decoder, feedbackFrame(0x205, bytes));
    QVERIFY(near(state.angleDeg, 180.0));
    QCOMPARE(state.speedRpm, -40.0);
    QVERIFY(state.currentValid);
    // 1000 * 3 / 16384
    QVERIFY(near(state.currentA, 0.18310546875));
    QCOMPARE(state.temperatureC, 50.0);
}

void MotorDecoderTest::damiaoMitDecodesPackedFields()
{
    MotorConfig config = makeMotor(MotorModel::DamiaoMit, 0x001, QStringLiteral("DM1"));
    MotorDecoder decoder;
    decoder.setMotors({config});

    // state 1 (enabled), motor id 3, position 0xFFFF (max), 12 bit velocity
    // 0xFFF (max), 12 bit torque 0 (min), MOS 42 C, rotor 65 C.
    const BusMessage frame = damiaoFrame(0x001, 1, 3, 0xFFFF, 0xFFF, 0x000, 42, 65);
    ProtocolMessage msg;
    QVERIFY(decoder.tryDecode(frame, msg) == DecodeStatus::Completed);

    const MotorState state = motorStateFromProtocolMessage(msg);
    QVERIFY(state.hasData);
    QVERIFY(state.timestampUs == k_timestampUs);
    QCOMPARE(state.damiaoState, 1u);
    // 65535 / 65535 * (2 * 12.5) - 12.5
    QVERIFY(near(state.positionRad, 12.5));
    QVERIFY(near(state.angleDeg, 356.197243913529));
    // 4095 / 4095 * (2 * 30) - 30
    QVERIFY(near(state.velocityRadPerSec, 30.0));
    QVERIFY(near(state.speedRpm, 286.478897565412));
    // 0 / 4095 * (2 * 10) - 10
    QVERIFY(near(state.torqueNm, -10.0));
    QCOMPARE(state.temperatureC, 65.0);
    QCOMPARE(state.mosTemperatureC, 42.0);
    QVERIFY(state.temperatureValid);
    QVERIFY(!state.currentValid);
    QCOMPARE(state.rawAngle, 65535u);
    QCOMPARE(state.rawSpeed, 4095);
    QCOMPARE(state.rawCurrent, 0);
    QCOMPARE(state.rawTemperature, 65);
    QCOMPARE(msg.description,
             QStringLiteral("DM MIT id 3 angle 356.2 deg pos 12.50 rad vel 30.00 rad/s "
                            "torque -10.00 Nm state 1 rotor 65 C MOS 42 C"));
}

void MotorDecoderTest::damiaoMitDecodesStatusAndPositiveTorque()
{
    MotorDecoder decoder;
    decoder.setMotors({makeMotor(MotorModel::DamiaoMit, 0x001, QStringLiteral("DM1"))});

    // state 12 (rotor over-temperature), motor id 5, position 0 (min), velocity
    // 0 (min), torque 0xFFF (max), MOS 20 C, rotor 30 C.
    const BusMessage frame = damiaoFrame(0x001, 12, 5, 0x0000, 0x000, 0xFFF, 20, 30);
    const MotorState state = decodeSingle(decoder, frame);
    QCOMPARE(state.damiaoState, 12u);
    QVERIFY(near(state.positionRad, -12.5));
    QVERIFY(near(state.angleDeg, 3.80275608647094));
    QVERIFY(near(state.velocityRadPerSec, -30.0));
    QVERIFY(near(state.speedRpm, -286.478897565412));
    QVERIFY(near(state.torqueNm, 10.0));
    QCOMPARE(state.temperatureC, 30.0);
    QCOMPARE(state.mosTemperatureC, 20.0);
    QCOMPARE(state.rawAngle, 0u);
    QCOMPARE(state.rawSpeed, 0);
    QCOMPARE(state.rawCurrent, 4095);
}

void MotorDecoderTest::damiaoMitUsesConfiguredRanges()
{
    MotorConfig config = makeMotor(MotorModel::DamiaoMit, 0x010, QStringLiteral("DM2"));
    config.damiaoPmax = 2.0;
    config.damiaoVmax = 10.0;
    config.damiaoTmax = 3.0;
    MotorDecoder decoder;
    decoder.setMotors({config});

    // position 0xFFFF (max = +Pmax), velocity 0 (min = -Vmax), torque 0xFFF
    // (max = +Tmax).
    const BusMessage frame = damiaoFrame(0x010, 1, 7, 0xFFFF, 0x000, 0xFFF, 25, 45);
    const MotorState state = decodeSingle(decoder, frame);
    QVERIFY(near(state.positionRad, 2.0));
    QVERIFY(near(state.angleDeg, 114.591559026165));
    QVERIFY(near(state.velocityRadPerSec, -10.0));
    QVERIFY(near(state.speedRpm, -95.4929658551372));
    QVERIFY(near(state.torqueNm, 3.0));
    QCOMPARE(state.temperatureC, 45.0);
    QCOMPARE(state.mosTemperatureC, 25.0);
}

void MotorDecoderTest::ignoresDisabledOrUnconfiguredMotors()
{
    MotorDecoder decoder;
    ProtocolMessage msg;
    const BusMessage frame = feedbackFrame(0x201, k_m3508Positive);
    // No motors configured at all.
    QVERIFY(decoder.tryDecode(frame, msg) == DecodeStatus::Ignored);

    // A disabled motor is not decoded.
    MotorConfig disabled = makeMotor(MotorModel::DjiM3508, 0x201, QStringLiteral("Disabled"));
    disabled.enabled = false;
    decoder.setMotors({disabled});
    QVERIFY(decoder.tryDecode(frame, msg) == DecodeStatus::Ignored);

    // An unconfigured identifier is ignored even though it is in a DJI range.
    decoder.setMotors({makeMotor(MotorModel::DjiM3508, 0x208, QStringLiteral("Other"))});
    QVERIFY(decoder.tryDecode(frame, msg) == DecodeStatus::Ignored);

    // A duplicate identifier is skipped while disabled; the next match wins.
    QVector<MotorConfig> configs;
    configs.append(disabled);
    configs.append(makeMotor(MotorModel::DjiM3508, 0x201, QStringLiteral("Enabled")));
    decoder.setMotors(configs);
    QVERIFY(decoder.tryDecode(frame, msg) == DecodeStatus::Completed);
    QCOMPARE(msg.name, QStringLiteral("Enabled"));
    QCOMPARE(msg.metadata.value(QStringLiteral("motorIndex")).toInt(), 1);
}

void MotorDecoderTest::ignoresMalformedFrames()
{
    MotorDecoder decoder;
    decoder.setMotors({makeMotor(MotorModel::DjiM3508, 0x201, QStringLiteral("Chassis1"))});
    ProtocolMessage msg;
    const BusMessage valid = feedbackFrame(0x201, k_m3508Positive);

    BusMessage wrongDlc = valid;
    wrongDlc.setLength(7);
    QVERIFY(decoder.tryDecode(wrongDlc, msg) == DecodeStatus::Ignored);

    BusMessage empty = valid;
    empty.setLength(0);
    QVERIFY(decoder.tryDecode(empty, msg) == DecodeStatus::Ignored);

    BusMessage extended = valid;
    extended.setExtended(true);
    QVERIFY(extended.getId() == 0x201u);
    QVERIFY(decoder.tryDecode(extended, msg) == DecodeStatus::Ignored);

    BusMessage rtr = valid;
    rtr.setRTR(true);
    QVERIFY(decoder.tryDecode(rtr, msg) == DecodeStatus::Ignored);

    BusMessage errorFrame = valid;
    errorFrame.setErrorFrame(true);
    QVERIFY(decoder.tryDecode(errorFrame, msg) == DecodeStatus::Ignored);

    BusMessage fd = valid;
    fd.setFD(true);
    QVERIFY(decoder.tryDecode(fd, msg) == DecodeStatus::Ignored);

    // The untouched frame still decodes.
    QVERIFY(decoder.tryDecode(valid, msg) == DecodeStatus::Completed);
}

void MotorDecoderTest::fillsProtocolMessageAndMetadata()
{
    MotorDecoder decoder;
    decoder.setMotors({makeMotor(MotorModel::DjiM3508, 0x201, QStringLiteral("Chassis1"))});

    ProtocolMessage msg;
    const BusMessage frame = feedbackFrame(0x201, k_m3508Positive);
    QVERIFY(decoder.tryDecode(frame, msg) == DecodeStatus::Completed);

    QCOMPARE(msg.name, QStringLiteral("Chassis1"));
    QCOMPARE(msg.protocol, QStringLiteral("Motor"));
    QVERIFY(msg.type == MessageType::Unknown);
    QCOMPARE(msg.id, 0x201u);
    QCOMPARE(msg.timestampUs, k_timestampUs);
    QCOMPARE(msg.globalIndex, 0u); // assigned later by ProtocolManager
    QVERIFY(msg.payload == QByteArray::fromHex("01180bb803e823a5"));

    const QVariantMap &metadata = msg.metadata;
    QCOMPARE(metadata.value(QStringLiteral("motorIndex")).toInt(), 0);
    QCOMPARE(metadata.value(QStringLiteral("model")).toInt(),
             static_cast<int>(MotorModel::DjiM3508));
    QVERIFY(near(metadata.value(QStringLiteral("angleDeg")).toDouble(), 12.3046875));
    QCOMPARE(metadata.value(QStringLiteral("speedRpm")).toDouble(), 3000.0);
    QVERIFY(near(metadata.value(QStringLiteral("currentA")).toDouble(), 1.220703125));
    QCOMPARE(metadata.value(QStringLiteral("temperatureC")).toDouble(), 35.0);
    QCOMPARE(metadata.value(QStringLiteral("positionRad")).toDouble(), 0.0);
    QCOMPARE(metadata.value(QStringLiteral("velocityRadPerSec")).toDouble(), 0.0);
    QCOMPARE(metadata.value(QStringLiteral("torqueNm")).toDouble(), 0.0);
    QCOMPARE(metadata.value(QStringLiteral("damiaoState")).toUInt(), 0u);
    QCOMPARE(metadata.value(QStringLiteral("rawAngle")).toUInt(), 280u);
    QCOMPARE(metadata.value(QStringLiteral("rawSpeed")).toInt(), 3000);
    QCOMPARE(metadata.value(QStringLiteral("rawCurrent")).toInt(), 1000);
    QCOMPARE(metadata.value(QStringLiteral("rawTemperature")).toInt(), 35);
    QCOMPARE(msg.description,
             QStringLiteral("M3508 angle 12.3 deg speed 3000 rpm current 1.22 A 35 C"));
}

void MotorDecoderTest::protocolManagerRoutesAndAssignsGlobalIndex()
{
    auto decoder = std::make_shared<MotorDecoder>();
    QVector<MotorConfig> configs;
    configs.append(makeMotor(MotorModel::DjiM3508, 0x201, QStringLiteral("Chassis1")));
    configs.append(makeMotor(MotorModel::DamiaoMit, 0x005, QStringLiteral("Arm1")));
    decoder->setMotors(configs);
    QVERIFY(decoder->motors().size() == 2);

    ProtocolManager manager;
    manager.addDecoder(decoder);

    ProtocolMessage msg;
    QVERIFY(manager.processFrame(feedbackFrame(0x201, k_m3508Positive), msg)
            == DecodeStatus::Completed);
    QCOMPARE(msg.globalIndex, 1u);
    QCOMPARE(msg.name, QStringLiteral("Chassis1"));
    QCOMPARE(msg.metadata.value(QStringLiteral("motorIndex")).toInt(), 0);

    // Unconfigured identifier: no decoder consumes the frame, no index is used.
    ProtocolMessage ignored;
    QVERIFY(manager.processFrame(feedbackFrame(0x202, k_m3508Negative), ignored)
            == DecodeStatus::Ignored);
    QCOMPARE(manager.messageCounter(), 1u);

    const BusMessage damiao = damiaoFrame(0x005, 1, 2, 0x8000, 0x800, 0x800, 30, 40);
    QVERIFY(manager.processFrame(damiao, msg) == DecodeStatus::Completed);
    QCOMPARE(msg.globalIndex, 2u);
    QCOMPARE(msg.name, QStringLiteral("Arm1"));
    QCOMPARE(msg.metadata.value(QStringLiteral("motorIndex")).toInt(), 1);

    manager.reset();
    QCOMPARE(manager.messageCounter(), 0u);
    QVERIFY(manager.processFrame(feedbackFrame(0x201, k_m3508Positive), msg)
            == DecodeStatus::Completed);
    QCOMPARE(msg.globalIndex, 1u);
}

void MotorDecoderTest::motorStateFromProtocolMessageRequiresMotorIndex()
{
    ProtocolMessage msg;
    bool ok = true;
    int index = 7;
    MotorState state = motorStateFromProtocolMessage(msg, &index, &ok);
    QVERIFY(!ok);
    QCOMPARE(index, -1);
    QVERIFY(!state.hasData);

    // All other fields together are not enough: the motor index is mandatory.
    msg.metadata.insert(QStringLiteral("model"), static_cast<int>(MotorModel::DjiM3508));
    msg.metadata.insert(QStringLiteral("angleDeg"), 12.5);
    state = motorStateFromProtocolMessage(msg, &index, &ok);
    QVERIFY(!ok);
    QCOMPARE(index, -1);
    QVERIFY(!state.hasData);

    // The default arguments are allowed too.
    state = motorStateFromProtocolMessage(msg);
    QVERIFY(!state.hasData);
}

void MotorDecoderTest::resetKeepsConfigurationUsable()
{
    MotorDecoder decoder;
    decoder.setMotors({makeMotor(MotorModel::DjiM3508, 0x201, QStringLiteral("Chassis1"))});

    ProtocolMessage msg;
    QVERIFY(decoder.tryDecode(feedbackFrame(0x201, k_m3508Positive), msg)
            == DecodeStatus::Completed);

    decoder.reset();
    QVERIFY(decoder.motors().size() == 1);
    QVERIFY(decoder.tryDecode(feedbackFrame(0x201, k_m3508Positive), msg)
            == DecodeStatus::Completed);
}

QTEST_MAIN(MotorDecoderTest)
#include "MotorDecoderTest.moc"
