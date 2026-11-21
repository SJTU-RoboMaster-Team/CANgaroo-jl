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

#include "appfonts.h"

#include <QApplication>
#include <QDebug>
#include <QFontDatabase>
#include <QStringList>

namespace
{
QString g_sansFamily = QStringLiteral("Segoe UI");
QString g_monoFamily = QStringLiteral("Consolas");
bool g_bundled = false;

QString registerFont(const QString &resourcePath)
{
    const int id = QFontDatabase::addApplicationFont(resourcePath);
    if (id < 0) {
        qWarning() << "AppFonts: failed to register" << resourcePath;
        return QString();
    }
    const QStringList families = QFontDatabase::applicationFontFamilies(id);
    return families.isEmpty() ? QString() : families.first();
}
} // namespace

namespace AppFonts
{

bool install()
{
    const QString sans = registerFont(QStringLiteral(":/assets/fonts/IBMPlexSans-Regular.ttf"));
    registerFont(QStringLiteral(":/assets/fonts/IBMPlexSans-Medium.ttf"));
    registerFont(QStringLiteral(":/assets/fonts/IBMPlexSans-SemiBold.ttf"));
    const QString mono = registerFont(QStringLiteral(":/assets/fonts/IBMPlexMono-Regular.ttf"));
    registerFont(QStringLiteral(":/assets/fonts/IBMPlexMono-Medium.ttf"));

    g_bundled = !sans.isEmpty() && !mono.isEmpty();
    if (g_bundled) {
        g_sansFamily = sans;
        g_monoFamily = mono;
    } else {
        qWarning() << "AppFonts: bundled IBM Plex faces are unavailable; "
                      "falling back to system fonts";
    }

    QApplication::setFont(uiFont(9));
    return g_bundled;
}

bool bundled()
{
    return g_bundled;
}

QString uiFamily()
{
    return g_sansFamily;
}

QString monoFamily()
{
    return g_monoFamily;
}

QFont uiFont(qreal pointSize, int weight)
{
    QFont font;
    font.setFamilies({g_sansFamily,
                      QStringLiteral("Microsoft YaHei UI"),
                      QStringLiteral("Microsoft YaHei")});
    font.setPointSizeF(pointSize);
    font.setWeight(QFont::Weight(weight));
    return font;
}

QFont monoFont(qreal pointSize, int weight)
{
    QFont font;
    font.setFamilies({g_monoFamily, QStringLiteral("Consolas")});
    font.setPointSizeF(pointSize);
    font.setWeight(QFont::Weight(weight));
    return font;
}

} // namespace AppFonts
