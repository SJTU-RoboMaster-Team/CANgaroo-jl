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
#include <memory>

#include <QVector>

#include "IDecoder.h"

/// Generic frame decoder registry: every registered decoder is offered each
/// frame in registration order and the first one that does not ignore it wins.
/// Protocol implementations (motor feedback, UDS, J1939, ...) only have to
/// implement IDecoder; the manager takes care of sequence numbers and resets.
class ProtocolManager
{
public:
    ProtocolManager();
    ~ProtocolManager() = default;

    void addDecoder(const std::shared_ptr<IDecoder> &decoder);

    /**
     * @brief Processes a CAN frame through all registered decoders.
     * @return The status of the decoding process.
     */
    DecodeStatus processFrame(const BusMessage &frame, ProtocolMessage &outMsg);

    void reset();

    [[nodiscard]] uint32_t messageCounter() const { return _msgCounter; }

private:
    QVector<std::shared_ptr<IDecoder>> _decoders;
    uint32_t _msgCounter = 0;
};
