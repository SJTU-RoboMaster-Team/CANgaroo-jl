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

#include "ProtocolManager.h"

ProtocolManager::ProtocolManager() = default;

void ProtocolManager::addDecoder(const std::shared_ptr<IDecoder> &decoder)
{
    if (decoder) {
        _decoders.append(decoder);
    }
}

DecodeStatus ProtocolManager::processFrame(const BusMessage &frame, ProtocolMessage &outMsg)
{
    for (const std::shared_ptr<IDecoder> &decoder : _decoders) {
        const DecodeStatus status = decoder->tryDecode(frame, outMsg);
        if (status == DecodeStatus::Ignored) {
            continue;
        }
        if (status == DecodeStatus::Completed) {
            outMsg.globalIndex = ++_msgCounter;
        }
        return status;
    }
    return DecodeStatus::Ignored;
}

void ProtocolManager::reset()
{
    _msgCounter = 0;
    for (const std::shared_ptr<IDecoder> &decoder : _decoders) {
        decoder->reset();
    }
}
