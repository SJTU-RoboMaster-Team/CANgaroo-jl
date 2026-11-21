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

#include "RawTxMessageBuilder.h"

#include <algorithm>

#include <QCoreApplication>

#include "core/TraceLineFormat.h"

std::optional<BusMessage> buildRawTxMessage(const RawTxMessageInput &input, QString *error)
{
    const auto reject = [error](const char *reason) -> std::optional<BusMessage> {
        if (error) {
            *error = QString::fromLatin1(reason);
        }
        return std::nullopt;
    };

    if (input.dlc < 0 || (!input.fd && input.dlc > 8)
        || (input.fd && !std::ranges::binary_search(TraceLineFormat::can_fd_dlc_lengths, input.dlc))) {
        return reject(QT_TRANSLATE_NOOP("RawTxWindow", "Invalid payload length for the selected frame format."));
    }
    if (input.rtr && input.fd) {
        return reject(QT_TRANSLATE_NOOP("RawTxWindow", "CAN FD does not support remote frames."));
    }
    if (input.brs && !input.fd) {
        return reject(QT_TRANSLATE_NOOP("RawTxWindow", "BRS requires CAN FD."));
    }

    BusMessage message(std::min(input.id, quint32{0x1FFFFFFF}));
    message.setExtended(input.extended);
    message.setRTR(input.rtr);
    message.setFD(input.fd);
    message.setBRS(input.brs);
    message.setLength(static_cast<quint8>(input.dlc));
    message.setInterfaceId(input.interfaceId);
    message.setRX(false);
    message.setShow(true);
    if (!input.rtr) {
        for (int i = 0; i < input.dlc; ++i) {
            message.setDataAt(i, input.data[i]);
        }
    }
    if (error) {
        error->clear();
    }
    return message;
}
