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

#include <QByteArray>
#include <QString>

#include <optional>

namespace HexData
{
/**
 * Parses a human-typed hexadecimal byte string into bytes.
 *
 * Accepted forms: "00 11 22", "001122", "0x01, 0x02", "1 2 3"
 * (separators: spaces, tabs, commas, semicolons, newlines).
 *
 * @return the parsed bytes, or std::nullopt when the text is empty or a token
 *         is not a hexadecimal byte (odd-length values without separators,
 *         values above 0xFF, non-hex characters).
 */
[[nodiscard]] std::optional<QByteArray> parseByteString(const QString &text);
}
