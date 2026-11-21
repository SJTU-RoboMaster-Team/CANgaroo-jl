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

#include "translation.h"

#include <QCoreApplication>
#include <QLibraryInfo>
#include <QLocale>
#include <QSettings>
#include <QTranslator>

#include "core/Log.h"

namespace
{
QTranslator *s_appTranslator = nullptr;
QTranslator *s_qtTranslator = nullptr;
QString s_currentLocale = QStringLiteral("en_US");

QLocale toQLocale(const QString &locale)
{
    if (locale == QLatin1String("zh_cn"))
        return QLocale(QLocale::Chinese, QLocale::China);
    return QLocale(locale);
}

QStringList qtTranslationDirs()
{
    QStringList dirs;
    const QString qtPath = QLibraryInfo::path(QLibraryInfo::TranslationsPath);
    if (!qtPath.isEmpty())
        dirs.append(qtPath);
    // windeployqt ships the combined meta catalogs (qt_<locale>.qm) in a
    // "translations" directory next to the executable. On a machine without
    // Qt installed the compiled-in translations path above does not exist.
    const QString appPath = QCoreApplication::applicationDirPath()
                            + QStringLiteral("/translations");
    if (!dirs.contains(appPath))
        dirs.append(appPath);
    return dirs;
}

void removeTranslator(QTranslator *&translator)
{
    if (!translator)
        return;
    QCoreApplication::removeTranslator(translator);
    delete translator;
    translator = nullptr;
}
} // namespace

QString Translation::savedLocale()
{
    QSettings settings;
    return settings.value(QStringLiteral("ui/language"), QStringLiteral("en_US")).toString();
}

QStringList Translation::supportedLocales()
{
    return {QStringLiteral("en_US"), QStringLiteral("zh_cn")};
}

QString Translation::currentLocale()
{
    return s_currentLocale;
}

bool Translation::apply(const QString &locale)
{
    if (!supportedLocales().contains(locale))
    {
        log_error(QStringLiteral("Unknown UI locale \"%1\".").arg(locale));
        return false;
    }

    if (locale == QLatin1String("en_US"))
    {
        removeTranslator(s_appTranslator);
        removeTranslator(s_qtTranslator);
        s_currentLocale = locale;
        return true;
    }

    // Load everything before touching the installed translators: a missing
    // catalog must leave the current language fully intact.
    auto *appTranslator = new QTranslator;
    const QString catalog = QStringLiteral(":/translations/i18n_") + locale + QStringLiteral(".qm");
    if (!appTranslator->load(catalog))
    {
        delete appTranslator;
        log_error(QStringLiteral("Translation catalog %1 is missing, keeping the current UI language.").arg(catalog));
        return false;
    }

    // Qt's own strings (standard dialog buttons, built-in dialogs) need the
    // Qt translation catalog; without it a Chinese UI still shows "OK"/"Cancel".
    QTranslator *qtTranslator = new QTranslator;
    QString qtCatalogFound;
    // The development install ships qtbase_<locale>.qm, while windeployqt
    // packages the combined qt_<locale>.qm meta catalog; try both.
    for (const QString &dir : qtTranslationDirs())
    {
        for (const QString &prefix : {QStringLiteral("qtbase"), QStringLiteral("qt")})
        {
            if (qtTranslator->load(toQLocale(locale), prefix, QStringLiteral("_"), dir))
            {
                qtCatalogFound = dir + QLatin1Char('/') + prefix;
                break;
            }
        }
        if (!qtCatalogFound.isEmpty())
            break;
    }

    if (!qtCatalogFound.isEmpty())
    {
        removeTranslator(s_qtTranslator);
        QCoreApplication::installTranslator(qtTranslator);
        s_qtTranslator = qtTranslator;
    }
    else
    {
        delete qtTranslator;
        s_qtTranslator = nullptr;
        log_warning(QStringLiteral("Qt base translation catalog for %1 not found in \"%2\"; "
                                   "Qt-built-in dialogs and standard buttons stay English.")
                        .arg(locale, qtTranslationDirs().join(QStringLiteral("\", \""))));
    }

    removeTranslator(s_appTranslator);
    QCoreApplication::installTranslator(appTranslator);
    s_appTranslator = appTranslator;
    s_currentLocale = locale;
    return true;
}
