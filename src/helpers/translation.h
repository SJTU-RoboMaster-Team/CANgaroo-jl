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

#include <QString>
#include <QStringList>

/**
 * Application language handling.
 *
 * One place installs both the application catalog (i18n_<locale>.qm) and Qt's
 * own catalog (qtbase_<locale>.qm, needed for the standard buttons of
 * QMessageBox/QDialogButtonBox and Qt's built-in dialogs), so that every window
 * created afterwards sees the right language without screen-by-screen setup.
 */
namespace Translation
{
/// Locale id saved in QSettings ("ui/language"), defaults to en_US.
QString savedLocale();

/// Locale ids the application ships catalogs for.
QStringList supportedLocales();

/// Locale id currently installed.
QString currentLocale();

/**
 * Removes the previously installed translators and installs the ones for
 * \a locale. Returns false (and logs an error) if the application catalog
 * cannot be loaded; the previously active language then stays active.
 */
bool apply(const QString &locale);
}
