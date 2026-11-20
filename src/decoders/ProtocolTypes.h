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

#include <QByteArray>
#include <QString>
#include <QVariantMap>
#include <QVector>

#include "core/BusMessage.h"

enum class MessageType {
    Request,
    PositiveResponse,
    NegativeResponse,
    Unknown
};

/// One decoded protocol message (a single frame or a reassembled sequence).
/// The generic decoder framework was introduced upstream for UDS/J1939; it is
/// reused here for per-frame analysis such as the motor feedback protocols.
struct ProtocolMessage {
    QString name;                  ///< Human readable message name, e.g. the motor name
    QString description;           ///< Details, e.g. formatted decoded values
    int64_t timestampUs = 0;       ///< Device timestamp of the (last) frame, microseconds
    QByteArray payload;            ///< Reassembled data
    QVector<BusMessage> rawFrames; ///< Frames that make up this message
    QString protocol;              ///< Protocol identifier, e.g. "Motor"
    MessageType type = MessageType::Unknown;
    uint32_t id = 0;               ///< Protocol specific identifier (CAN ID, SID, PGN, ...)
    uint32_t globalIndex = 0;      ///< Monotonic sequence number assigned by the manager
    QVariantMap metadata;          ///< Protocol specific fields
};
