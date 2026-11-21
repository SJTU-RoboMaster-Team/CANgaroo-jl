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

#pragma once

#include <cstdint>

#include <QString>
#include <QVector>

/// Motor protocols supported by the built-in motor decoder.
enum class MotorModel {
    DjiM3508,   ///< DJI RoboMaster M3508 with C620 ESC
    DjiM2006,   ///< DJI RoboMaster M2006 with C610 ESC
    DjiGM6020,  ///< DJI RoboMaster GM6020
    DamiaoMit,  ///< DAMIAO DM series in MIT mode (DM-J4310/4340/8006, ...)
};

/// One configured motor. The CAN ID is the ID of the feedback frame:
/// 0x201..0x20B for DJI ESCs (0x200 + ESC ID), the motor's master ID for DAMIAO.
struct MotorConfig {
    QString name;
    MotorModel model = MotorModel::DjiM3508;
    quint32 feedbackId = 0x201;
    bool invert = false;
    bool enabled = true;

    // DAMIAO MIT only: output range registers (PMAX/VMAX/TMAX). They are
    // writable on the motor, so they are configuration, not constants.
    double damiaoPmax = 12.5;  // rad
    double damiaoVmax = 30.0;  // rad/s
    double damiaoTmax = 10.0;  // N·m
};

/// Latest decoded state of one motor (rotor side values).
struct MotorState {
    bool hasData = false;
    bool online = false;
    int64_t timestampUs = 0;

    double angleDeg = 0.0;        ///< Mechanical angle, 0..360°
    double speedRpm = 0.0;        ///< Rotor speed
    double currentA = 0.0;        ///< Actual torque current (DJI); not valid for DAMIAO
    double temperatureC = 0.0;

    // DAMIAO MIT feedback additionally carries these; they stay 0 for DJI.
    double positionRad = 0.0;
    double velocityRadPerSec = 0.0;
    double torqueNm = 0.0;        ///< DAMIAO feedback torque; 0 for DJI
    quint32 damiaoState = 0;
    double mosTemperatureC = 0.0; ///< DAMIAO MOS temperature

    // Some protocols do not document a physical conversion for a field; the UI
    // must show "—" instead of a made up value.
    bool currentValid = true;
    bool temperatureValid = true;

    // Raw values as sent by the motor, for diagnostics.
    quint32 rawAngle = 0;
    qint32 rawSpeed = 0;
    qint32 rawCurrent = 0;
    qint32 rawTemperature = 0;

    qint64 lastSeenMs = 0;        ///< Wall clock of the last decoded frame
};

namespace MotorProtocols {

[[nodiscard]] bool isDjiModel(MotorModel model);
[[nodiscard]] bool isDamiaoModel(MotorModel model);

/// Stable key used in workspace XML, e.g. "dji_m3508".
[[nodiscard]] QString modelKey(MotorModel model);
/// Inverse of modelKey(); `ok` is set to false for unknown keys.
[[nodiscard]] MotorModel modelFromKey(const QString &key, bool *ok = nullptr);

/// DJI feedback current: amps per raw LSB (raw range is -16384..16384).
/// Returns 0 for models without a documented conversion (C610/M2006) — check
/// djiCurrentConversionValid() first.
[[nodiscard]] double djiCurrentPerRaw(MotorModel model);
/// Nominal feedback current range of the motor in amps; 0 when not documented.
[[nodiscard]] double djiNominalCurrentA(MotorModel model);
/// Whether the DJI manual documents a physical conversion for the feedback current.
[[nodiscard]] bool djiCurrentConversionValid(MotorModel model);

/// How long a motor may stay silent before the UI marks it offline.
/// DAMIAO motors only answer after a control frame or refresh request, so
/// passive listening produces sparse feedback; they get a longer window.
[[nodiscard]] int64_t offlineTimeoutMsForModel(MotorModel model);

} // namespace MotorProtocols
