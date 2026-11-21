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

#include <QVector>

#include "decoders/IDecoder.h"
#include "decoders/motor/MotorTypes.h"

/// Keys used in ProtocolMessage::metadata for a decoded motor feedback frame.
namespace MotorMetadataKeys {
inline constexpr char motorIndex[] = "motorIndex";
inline constexpr char model[] = "model";
inline constexpr char angleDeg[] = "angleDeg";
inline constexpr char speedRpm[] = "speedRpm";
inline constexpr char currentA[] = "currentA";
inline constexpr char temperatureC[] = "temperatureC";
inline constexpr char positionRad[] = "positionRad";
inline constexpr char velocityRadPerSec[] = "velocityRadPerSec";
inline constexpr char torqueNm[] = "torqueNm";
inline constexpr char damiaoState[] = "damiaoState";
inline constexpr char rawAngle[] = "rawAngle";
inline constexpr char rawSpeed[] = "rawSpeed";
inline constexpr char rawCurrent[] = "rawCurrent";
inline constexpr char rawTemperature[] = "rawTemperature";
inline constexpr char currentValid[] = "currentValid";
inline constexpr char temperatureValid[] = "temperatureValid";
inline constexpr char mosTemperatureC[] = "mosTemperatureC";
} // namespace MotorMetadataKeys

/// Decodes motor feedback frames for the configured motors:
/// DJI M3508/M2006/GM6020 (one 8 byte feedback frame per motor) and DAMIAO DM
/// series in MIT mode (8 bit packed fields). Frames from unconfigured IDs,
/// disabled motors or with the wrong DLC are ignored.
class MotorDecoder : public IDecoder
{
public:
    MotorDecoder();
    ~MotorDecoder() override = default;

    void setMotors(const QVector<MotorConfig> &motors);
    [[nodiscard]] QVector<MotorConfig> motors() const;

    DecodeStatus tryDecode(const BusMessage &frame, ProtocolMessage &outMsg) override;
    void reset() override;

private:
    static bool decodeDjiFeedback(const MotorConfig &config,
                                  const BusMessage &frame,
                                  MotorState &state,
                                  QString *detail);
    static bool decodeDamiaoFeedback(const MotorConfig &config,
                                     const BusMessage &frame,
                                     MotorState &state,
                                     QString *detail);

    QVector<MotorConfig> _motors;
};

/// Reads the decoded MotorState back out of a completed ProtocolMessage.
[[nodiscard]] MotorState motorStateFromProtocolMessage(const ProtocolMessage &msg,
                                                       int *motorIndex = nullptr,
                                                       bool *ok = nullptr);
