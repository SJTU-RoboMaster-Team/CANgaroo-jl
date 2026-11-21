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

#include <array>
#include <optional>

#include "core/BusMessage.h"

struct RawTxMessageInput
{
    quint32 id = 0;
    bool extended = false;
    bool rtr = false;
    bool fd = false;
    bool brs = false;
    int dlc = 8; // Payload length in bytes.
    std::array<quint8, BusMessage::k_maxDataBytes> data{};
    BusInterfaceId interfaceId = InvalidBusInterfaceId;
};

[[nodiscard]] std::optional<BusMessage> buildRawTxMessage(const RawTxMessageInput &input,
                                                         QString *error = nullptr);
