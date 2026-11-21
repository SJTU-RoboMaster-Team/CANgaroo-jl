/*
  Copyright (c) 2026 Schildkroet

  This file is part of CANgaroo.

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

#include <QFont>
#include <QString>

namespace AppFonts
{
    // Registers the bundled IBM Plex faces and makes the UI family the
    // application default. Returns true when the bundled faces were found;
    // on failure the system UI/monospace families are used and a warning is
    // logged (the app keeps working).
    bool install();

    [[nodiscard]] bool bundled();

    // UI family list: IBM Plex Sans -> Microsoft YaHei UI -> Microsoft YaHei.
    [[nodiscard]] QString uiFamily();
    // Data family list: IBM Plex Mono -> Consolas.
    [[nodiscard]] QString monoFamily();

    [[nodiscard]] QFont uiFont(qreal pointSize, int weight = QFont::Normal);
    [[nodiscard]] QFont monoFont(qreal pointSize, int weight = QFont::Normal);
}
