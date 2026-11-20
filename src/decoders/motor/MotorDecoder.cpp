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

#include "MotorDecoder.h"

#include <cmath>

#include <QByteArray>
#include <QVariantMap>

namespace {

constexpr int k_feedbackPayloadBytes = 8;

constexpr double k_djiAngleRawMax = 8192.0;
constexpr double k_damiaoPositionRawMax = 65535.0;
constexpr double k_damiaoVelocityRawMax = 4095.0;
constexpr double k_damiaoTorqueRawMax = 4095.0;

constexpr double k_pi = 3.14159265358979323846;
constexpr double k_radToDeg = 180.0 / k_pi;
constexpr double k_radPerSecToRpm = 60.0 / (2.0 * k_pi);

[[nodiscard]] QString metadataKey(const char *name)
{
    return QString::fromLatin1(name);
}

/// All supported motors report with a single classic CAN frame (8 byte payload,
/// 11-bit identifier). Anything else is not a motor feedback frame.
[[nodiscard]] bool isFeedbackFrame(const BusMessage &frame)
{
    if (frame.isErrorFrame() || frame.isRTR() || frame.isFD() || frame.isExtended()) {
        return false;
    }
    return frame.getLength() == k_feedbackPayloadBytes;
}

[[nodiscard]] quint16 readBe16(const BusMessage &frame, uint8_t offset)
{
    return static_cast<quint16>((static_cast<quint16>(frame.getByte(offset)) << 8)
                                | frame.getByte(static_cast<uint8_t>(offset + 1)));
}

/// Wraps an angle in degrees into [0, 360).
[[nodiscard]] double normalizeDeg(double angleDeg)
{
    double wrapped = std::fmod(angleDeg, 360.0);
    if (wrapped < 0.0) {
        wrapped += 360.0;
    }
    return wrapped;
}

[[nodiscard]] QString djiModelName(MotorModel model)
{
    switch (model) {
    case MotorModel::DjiM3508:  return QStringLiteral("M3508");
    case MotorModel::DjiM2006:  return QStringLiteral("M2006");
    case MotorModel::DjiGM6020: return QStringLiteral("GM6020");
    case MotorModel::DamiaoMit: break;
    }
    return QStringLiteral("DAMIAO");
}

void fillMetadata(const MotorConfig &config, int motorIndex, const MotorState &state,
                  QVariantMap &metadata)
{
    metadata.insert(metadataKey(MotorMetadataKeys::motorIndex), motorIndex);
    metadata.insert(metadataKey(MotorMetadataKeys::model), static_cast<int>(config.model));
    metadata.insert(metadataKey(MotorMetadataKeys::angleDeg), state.angleDeg);
    metadata.insert(metadataKey(MotorMetadataKeys::speedRpm), state.speedRpm);
    metadata.insert(metadataKey(MotorMetadataKeys::currentA), state.currentA);
    metadata.insert(metadataKey(MotorMetadataKeys::temperatureC), state.temperatureC);
    metadata.insert(metadataKey(MotorMetadataKeys::positionRad), state.positionRad);
    metadata.insert(metadataKey(MotorMetadataKeys::velocityRadPerSec), state.velocityRadPerSec);
    metadata.insert(metadataKey(MotorMetadataKeys::torqueNm), state.torqueNm);
    metadata.insert(metadataKey(MotorMetadataKeys::damiaoState), state.damiaoState);
    metadata.insert(metadataKey(MotorMetadataKeys::rawAngle), state.rawAngle);
    metadata.insert(metadataKey(MotorMetadataKeys::rawSpeed), state.rawSpeed);
    metadata.insert(metadataKey(MotorMetadataKeys::rawCurrent), state.rawCurrent);
    metadata.insert(metadataKey(MotorMetadataKeys::rawTemperature), state.rawTemperature);
    metadata.insert(metadataKey(MotorMetadataKeys::currentValid), state.currentValid);
    metadata.insert(metadataKey(MotorMetadataKeys::temperatureValid), state.temperatureValid);
    metadata.insert(metadataKey(MotorMetadataKeys::mosTemperatureC), state.mosTemperatureC);
}

} // namespace

MotorDecoder::MotorDecoder() = default;

void MotorDecoder::setMotors(const QVector<MotorConfig> &motors)
{
    _motors = motors;
}

QVector<MotorConfig> MotorDecoder::motors() const
{
    return _motors;
}

DecodeStatus MotorDecoder::tryDecode(const BusMessage &frame, ProtocolMessage &outMsg)
{
    if (!isFeedbackFrame(frame)) {
        return DecodeStatus::Ignored;
    }

    for (int index = 0; index < _motors.size(); ++index) {
        const MotorConfig &config = _motors.at(index);
        if (!config.enabled || config.feedbackId != frame.getId()) {
            continue;
        }

        MotorState state;
        QString detail;
        const bool decoded = MotorProtocols::isDamiaoModel(config.model)
            ? decodeDamiaoFeedback(config, frame, state, &detail)
            : decodeDjiFeedback(config, frame, state, &detail);
        if (!decoded) {
            return DecodeStatus::Ignored;
        }

        state.hasData = true;
        state.online = true;
        state.timestampUs = frame.getTimestamp_us();

        // A motor feedback frame is a complete single-frame message; the
        // completion bookkeeping (globalIndex) is done by ProtocolManager.
        outMsg = ProtocolMessage();
        outMsg.name = config.name;
        outMsg.description = detail;
        outMsg.timestampUs = frame.getTimestamp_us();
        outMsg.payload = QByteArray(reinterpret_cast<const char *>(frame.getData()),
                                    k_feedbackPayloadBytes);
        outMsg.protocol = QStringLiteral("Motor");
        outMsg.type = MessageType::Unknown;
        outMsg.id = frame.getId();
        fillMetadata(config, index, state, outMsg.metadata);
        return DecodeStatus::Completed;
    }

    return DecodeStatus::Ignored;
}

void MotorDecoder::reset()
{
    // The DJI and DAMIAO feedback frames are stateless; only the configuration
    // is kept, so there is nothing to clear.
}

bool MotorDecoder::decodeDjiFeedback(const MotorConfig &config, const BusMessage &frame,
                                     MotorState &state, QString *detail)
{
    // Feedback frame layout (C620/C610/GM6020 manual): all 16-bit fields are
    // big endian, d6 is the ESC temperature. The C610 has no temperature byte.
    const quint32 rawAngle = readBe16(frame, 0);
    const qint32 rawSpeed = static_cast<qint16>(readBe16(frame, 2));
    const qint32 rawCurrent = static_cast<qint16>(readBe16(frame, 4));
    const qint32 rawTemperature = frame.getByte(6);

    state.rawAngle = rawAngle;
    state.rawSpeed = rawSpeed;
    state.rawCurrent = rawCurrent;
    state.rawTemperature = rawTemperature;

    state.angleDeg = static_cast<double>(rawAngle) * 360.0 / k_djiAngleRawMax;
    state.speedRpm = static_cast<double>(rawSpeed);

    // Only the C620 (M3508) and the GM6020 manual document an amps-per-LSB
    // conversion; the C610 feedback field is kept raw.
    state.currentValid = MotorProtocols::djiCurrentConversionValid(config.model);
    if (state.currentValid) {
        state.currentA = static_cast<double>(rawCurrent)
            * MotorProtocols::djiCurrentPerRaw(config.model);
    } else {
        state.currentA = static_cast<double>(rawCurrent);
    }

    state.temperatureC = static_cast<double>(rawTemperature);
    state.temperatureValid = config.model != MotorModel::DjiM2006;

    if (detail) {
        QString summary = QStringLiteral("%1 angle %2 deg speed %3 rpm")
                              .arg(djiModelName(config.model))
                              .arg(state.angleDeg, 0, 'f', 1)
                              .arg(state.speedRpm, 0, 'f', 0);
        if (state.currentValid) {
            summary += QStringLiteral(" current %1 A").arg(state.currentA, 0, 'f', 2);
        } else {
            summary += QStringLiteral(" current raw %1").arg(rawCurrent);
        }
        if (state.temperatureValid) {
            summary += QStringLiteral(" %1 C").arg(rawTemperature);
        } else {
            summary += QStringLiteral(" temperature n/a");
        }
        *detail = summary;
    }

    return true;
}

bool MotorDecoder::decodeDamiaoFeedback(const MotorConfig &config, const BusMessage &frame,
                                        MotorState &state, QString *detail)
{
    // MIT feedback frame layout:
    //   d0: state (high nibble) | motor id (low nibble)
    //   d1-d2: 16 bit position, d3-d4: 12 bit velocity, d4-d5: 12 bit torque
    //   d6: MOS temperature, d7: rotor temperature
    const uint8_t header = frame.getByte(0);
    const quint32 rawState = header >> 4;
    const quint32 motorId = header & 0x0Fu;

    const quint32 rawPosition = readBe16(frame, 1);
    const quint32 rawVelocity = (static_cast<quint32>(frame.getByte(3)) << 4)
        | (frame.getByte(4) >> 4);
    const quint32 rawTorque = (static_cast<quint32>(frame.getByte(4) & 0x0Fu) << 8)
        | frame.getByte(5);
    const qint32 mosTemperature = frame.getByte(6);
    const qint32 rawTemperature = frame.getByte(7);

    // PMAX/VMAX/TMAX are writable registers on the motor, so the physical
    // ranges come from the configuration instead of being hardcoded.
    state.positionRad = static_cast<double>(rawPosition) / k_damiaoPositionRawMax
        * (2.0 * config.damiaoPmax) - config.damiaoPmax;
    state.angleDeg = normalizeDeg(state.positionRad * k_radToDeg);
    state.velocityRadPerSec = static_cast<double>(rawVelocity) / k_damiaoVelocityRawMax
        * (2.0 * config.damiaoVmax) - config.damiaoVmax;
    state.speedRpm = state.velocityRadPerSec * k_radPerSecToRpm;
    state.torqueNm = static_cast<double>(rawTorque) / k_damiaoTorqueRawMax
        * (2.0 * config.damiaoTmax) - config.damiaoTmax;

    state.temperatureC = static_cast<double>(rawTemperature);
    state.mosTemperatureC = static_cast<double>(mosTemperature);
    state.currentValid = false; // MIT feedback carries torque, not current
    state.temperatureValid = true;

    state.damiaoState = rawState;
    state.rawAngle = rawPosition;
    state.rawSpeed = static_cast<qint32>(rawVelocity);
    state.rawCurrent = static_cast<qint32>(rawTorque);
    state.rawTemperature = rawTemperature;

    if (detail) {
        const QString summary = QStringLiteral(
                                    "DM MIT id %1 angle %2 deg pos %3 rad vel %4 rad/s "
                                    "torque %5 Nm state %6 rotor %7 C MOS %8 C")
                                    .arg(motorId)
                                    .arg(state.angleDeg, 0, 'f', 1)
                                    .arg(state.positionRad, 0, 'f', 2)
                                    .arg(state.velocityRadPerSec, 0, 'f', 2)
                                    .arg(state.torqueNm, 0, 'f', 2)
                                    .arg(rawState)
                                    .arg(rawTemperature)
                                    .arg(mosTemperature);
        *detail = summary;
    }

    return true;
}

MotorState motorStateFromProtocolMessage(const ProtocolMessage &msg, int *motorIndex, bool *ok)
{
    if (motorIndex) {
        *motorIndex = -1;
    }
    if (ok) {
        *ok = false;
    }

    MotorState state;
    const QString indexKey = metadataKey(MotorMetadataKeys::motorIndex);
    if (!msg.metadata.contains(indexKey)) {
        return state;
    }

    bool indexOk = false;
    const int index = msg.metadata.value(indexKey).toInt(&indexOk);
    if (!indexOk || index < 0) {
        return state;
    }

    state.hasData = true;
    state.online = true;
    state.timestampUs = msg.timestampUs;
    state.angleDeg = msg.metadata.value(metadataKey(MotorMetadataKeys::angleDeg)).toDouble();
    state.speedRpm = msg.metadata.value(metadataKey(MotorMetadataKeys::speedRpm)).toDouble();
    state.currentA = msg.metadata.value(metadataKey(MotorMetadataKeys::currentA)).toDouble();
    state.temperatureC = msg.metadata.value(metadataKey(MotorMetadataKeys::temperatureC)).toDouble();
    state.positionRad = msg.metadata.value(metadataKey(MotorMetadataKeys::positionRad)).toDouble();
    state.velocityRadPerSec =
        msg.metadata.value(metadataKey(MotorMetadataKeys::velocityRadPerSec)).toDouble();
    state.torqueNm = msg.metadata.value(metadataKey(MotorMetadataKeys::torqueNm)).toDouble();
    state.damiaoState = msg.metadata.value(metadataKey(MotorMetadataKeys::damiaoState)).toUInt();
    state.rawAngle = msg.metadata.value(metadataKey(MotorMetadataKeys::rawAngle)).toUInt();
    state.rawSpeed = msg.metadata.value(metadataKey(MotorMetadataKeys::rawSpeed)).toInt();
    state.rawCurrent = msg.metadata.value(metadataKey(MotorMetadataKeys::rawCurrent)).toInt();
    state.rawTemperature =
        msg.metadata.value(metadataKey(MotorMetadataKeys::rawTemperature)).toInt();
    // The decoder always writes these keys next to the public ones above; a
    // hand-built ProtocolMessage without them keeps the MotorState defaults.
    const QString currentValidKey = metadataKey(MotorMetadataKeys::currentValid);
    if (msg.metadata.contains(currentValidKey)) {
        state.currentValid = msg.metadata.value(currentValidKey).toBool();
    }
    const QString temperatureValidKey = metadataKey(MotorMetadataKeys::temperatureValid);
    if (msg.metadata.contains(temperatureValidKey)) {
        state.temperatureValid = msg.metadata.value(temperatureValidKey).toBool();
    }
    state.mosTemperatureC = msg.metadata.value(metadataKey(MotorMetadataKeys::mosTemperatureC)).toDouble();

    if (motorIndex) {
        *motorIndex = index;
    }
    if (ok) {
        *ok = true;
    }
    return state;
}
