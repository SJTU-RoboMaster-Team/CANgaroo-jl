/*

  Copyright (c) 2026 Jayachandran Dharuman

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

#include "ThemeManager.h"
#include <QApplication>
#include <QFile>
#include <QGuiApplication>
#include <QStyle>
#include <QStyleFactory>

namespace {

bool usesLegacyWindowsStandardPalette(const QStyle *style)
{
    if (!style)
        return false;
    const QString name = style->objectName();
    return name.compare(QLatin1String("windows11"), Qt::CaseInsensitive) == 0
        || name.compare(QLatin1String("windowsvista"), Qt::CaseInsensitive) == 0
        || name.compare(QLatin1String("windows"), Qt::CaseInsensitive) == 0;
}

QPalette paletteFromColors(const ThemeColors &colors, const QColor &disabledText)
{
    QPalette pal;
    pal.setColor(QPalette::Window, colors.window);
    pal.setColor(QPalette::WindowText, colors.windowText);
    pal.setColor(QPalette::Base, colors.base);
    pal.setColor(QPalette::AlternateBase, colors.alternateBase);
    pal.setColor(QPalette::ToolTipBase, colors.toolTipBase);
    pal.setColor(QPalette::ToolTipText, colors.toolTipText);
    pal.setColor(QPalette::Text, colors.text);
    pal.setColor(QPalette::Button, colors.button);
    pal.setColor(QPalette::ButtonText, colors.buttonText);
    pal.setColor(QPalette::BrightText, colors.brightText);
    pal.setColor(QPalette::Link, colors.link);
    pal.setColor(QPalette::Highlight, colors.highlight);
    pal.setColor(QPalette::HighlightedText, colors.highlightedText);

    pal.setColor(QPalette::Disabled, QPalette::WindowText, disabledText);
    pal.setColor(QPalette::Disabled, QPalette::Text, disabledText);
    pal.setColor(QPalette::Disabled, QPalette::ButtonText, disabledText);
    return pal;
}

} // namespace

ThemeManager::ThemeManager(QObject *parent)
    : QObject(parent), _currentTheme(Light)
{
    updateColors(Light);
}

ThemeManager& ThemeManager::instance()
{
    static ThemeManager inst;
    return inst;
}

QPalette ThemeManager::resolveNativePalette()
{
    QStyle *style = qApp ? qApp->style() : nullptr;
    if (!style)
        return QGuiApplication::palette();

    if (usesLegacyWindowsStandardPalette(style))
    {
        // On Windows, QStyle::standardPalette() falls back to the classic
        // #d4d0c8 Win9x palette instead of the OS light/dark theme. Clearing any
        // custom palette override lets Qt supply the active Windows palette.
        qApp->setPalette(QPalette());
        return qApp->palette();
    }

    return style->standardPalette();
}

void ThemeManager::applyTheme(Theme theme, bool nativeStyling)
{
    _currentTheme = theme;
    _nativeStyling = nativeStyling;
    updateColors(theme);

    if (nativeStyling)
    {
        // Defer entirely to the active QStyle and platform theme: drop any
        // global stylesheet and resolve the active native palette.
        qApp->setStyleSheet("");
        QPalette palette = resolveNativePalette();

        _colors.window = palette.color(QPalette::Window);
        _colors.windowText = palette.color(QPalette::WindowText);
        _colors.base = palette.color(QPalette::Base);
        _colors.alternateBase = palette.color(QPalette::AlternateBase);
        _colors.toolTipBase = palette.color(QPalette::ToolTipBase);
        _colors.toolTipText = palette.color(QPalette::ToolTipText);
        _colors.text = palette.color(QPalette::Text);
        _colors.button = palette.color(QPalette::Button);
        _colors.buttonText = palette.color(QPalette::ButtonText);
        _colors.brightText = palette.color(QPalette::BrightText);
        _colors.link = palette.color(QPalette::Link);
        _colors.highlight = palette.color(QPalette::Highlight);
        _colors.highlightedText = palette.color(QPalette::HighlightedText);

        // Some platform themes (e.g. GNOME/Adwaita) supply a heavily dimmed
        // "Inactive" color group to mimic GTK's unfocused-window "backdrop"
        // look. Qt briefly paints widgets with that group whenever window
        // activation churns (e.g. right after a QFileDialog closes), which
        // made freshly loaded script text flash faded/gray. Normalize the
        // Inactive text/base colors to match Active only when needed so styles
        // like Windows 11 retain an untouched system palette.
        bool normalizedInactive = false;
        for (QPalette::ColorRole role : {QPalette::WindowText, QPalette::Text, QPalette::ButtonText, QPalette::Base, QPalette::Window})
        {
            const QColor activeColor = palette.color(QPalette::Active, role);
            if (palette.color(QPalette::Inactive, role) != activeColor)
            {
                palette.setColor(QPalette::Inactive, role, activeColor);
                normalizedInactive = true;
            }
        }

        if (normalizedInactive || !usesLegacyWindowsStandardPalette(qApp->style()))
            qApp->setPalette(palette);
    }
    else
    {
        applyPalette(theme);
        applyStyleSheet(theme);
    }

    emit themeChanged(theme);
}

void ThemeManager::updateColors(Theme theme)
{
    if (theme == Light) {
        // Capture default system colors or standard light theme
        _colors.window = QColor(240, 240, 240);
        _colors.windowText = Qt::black;
        _colors.base = Qt::white;
        _colors.alternateBase = QColor(233, 231, 227);
        _colors.toolTipBase = Qt::white;
        _colors.toolTipText = Qt::black;
        _colors.text = Qt::black;
        _colors.button = QColor(240, 240, 240);
        _colors.buttonText = Qt::black;
        _colors.brightText = Qt::white;
        _colors.link = QColor(0, 0, 255);
        _colors.highlight = QColor(48, 140, 198);
        _colors.highlightedText = Qt::white;
        
        _colors.graphBackground = Qt::white;
        _colors.graphGrid = QColor(230, 230, 230);
        _colors.graphAxisText = Qt::black;
        _colors.graphCursor = Qt::black;

        _colors.statusSuccess = QColor(0x1b, 0x7f, 0x37);
        _colors.statusWarning = QColor(0xb4, 0x53, 0x09);
        _colors.statusError   = QColor(0xc6, 0x28, 0x28);
        _colors.statusInfo    = QColor(0x09, 0x69, 0xda);
        _colors.bannerWarningBg     = QColor(0xff, 0xf4, 0xd6);
        _colors.bannerWarningBorder = QColor(0xd9, 0x77, 0x06);
        _colors.bannerWarningText   = QColor(0x5c, 0x2d, 0x06);
        _colors.cursorA       = QColor(0x09, 0x69, 0xda);
        _colors.cursorB       = QColor(0xd9, 0x77, 0x06);
        _colors.triggerMarker = QColor(0xdc, 0x26, 0x26);
    } else {
        _colors.window = QColor(45, 45, 48);
        _colors.windowText = QColor(220, 220, 220);
        _colors.base = QColor(30, 30, 30);
        _colors.alternateBase = QColor(37, 37, 38);
        _colors.toolTipBase = QColor(48, 48, 52);
        _colors.toolTipText = QColor(240, 240, 240);
        _colors.text = QColor(220, 220, 220);
        _colors.button = QColor(45, 45, 48);
        _colors.buttonText = QColor(220, 220, 220);
        _colors.brightText = Qt::white;
        _colors.link = QColor(86, 156, 214);
        _colors.highlight = QColor(38, 79, 120);
        _colors.highlightedText = Qt::white;
        
        _colors.graphBackground = QColor(25, 25, 25);
        _colors.graphGrid = QColor(60, 60, 60);
        _colors.graphAxisText = QColor(180, 180, 180);
        _colors.graphCursor = Qt::white;

        _colors.statusSuccess = QColor(0x4a, 0xde, 0x80);
        _colors.statusWarning = QColor(0xfb, 0xbf, 0x24);
        _colors.statusError   = QColor(0xf8, 0x71, 0x71);
        _colors.statusInfo    = QColor(0x60, 0xa5, 0xfa);
        _colors.bannerWarningBg     = QColor(0x45, 0x2a, 0x0d);
        _colors.bannerWarningBorder = QColor(0xd9, 0x77, 0x06);
        _colors.bannerWarningText   = QColor(0xfd, 0xe6, 0x8a);
        _colors.cursorA       = QColor(0x60, 0xa5, 0xfa);
        _colors.cursorB       = QColor(0xfb, 0xbf, 0x24);
        _colors.triggerMarker = QColor(0xf8, 0x71, 0x71);
    }
}

void ThemeManager::applyPalette(Theme theme)
{
    if (theme == Light) {
        if (usesLegacyWindowsStandardPalette(qApp->style()))
            qApp->setPalette(paletteFromColors(_colors, QColor(140, 140, 140)));
        else
            qApp->setPalette(qApp->style()->standardPalette());
    } else {
        qApp->setPalette(paletteFromColors(_colors, QColor(127, 127, 127)));
    }
}

void ThemeManager::applyStyleSheet(Theme theme)
{
    QString qssPath = (theme == Light) ? ":/assets/light_theme.qss" : ":/assets/dark_theme.qss";
    QFile file(qssPath);
    if (file.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(file.readAll());
        qApp->setStyleSheet(styleSheet);
    } else {
        qApp->setStyleSheet("");
    }
}
