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

#include "ProtocolTypes.h"

enum class DecodeStatus {
    Ignored,   ///< Frame not relevant to this decoder
    Consumed,  ///< Frame accepted, sequence in progress
    Completed  ///< Frame accepted, sequence finished, outMsg populated
};

class IDecoder
{
public:
    virtual ~IDecoder() = default;

    /**
     * @brief Tries to decode an incoming CAN frame.
     * @param frame The raw CAN frame.
     * @param outMsg If a full protocol message is reassembled/decoded, it is stored here.
     * @return The status of the decoding process.
     */
    virtual DecodeStatus tryDecode(const BusMessage &frame, ProtocolMessage &outMsg) = 0;

    /**
     * @brief Resets the decoder state (e.g. for multiple channels or starting trace).
     */
    virtual void reset() = 0;
};
