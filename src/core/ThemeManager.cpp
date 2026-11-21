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

namespace {

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
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
    pal.setColor(QPalette::PlaceholderText, colors.inkFaint);
#endif

    pal.setColor(QPalette::Disabled, QPalette::WindowText, disabledText);
    pal.setColor(QPalette::Disabled, QPalette::Text, disabledText);
    pal.setColor(QPalette::Disabled, QPalette::ButtonText, disabledText);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
    pal.setColor(QPalette::Disabled, QPalette::PlaceholderText, disabledText);
#endif
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

void ThemeManager::applyTheme(Theme theme)
{
    _currentTheme = theme;
    updateColors(theme);
    applyPalette(theme);
    applyStyleSheet(theme);
    emit themeChanged(theme);
}

void ThemeManager::updateColors(Theme theme)
{
    if (theme == Light) {
        // Claude-like warm paper palette. The accent is terracotta; colour is
        // otherwise reserved for data and state.
        _colors.window       = QColor("#F4F1EA"); // chrome
        _colors.windowText   = QColor("#26231D"); // ink
        _colors.base         = QColor("#FFFDF8"); // canvas
        _colors.alternateBase= QColor("#F7F3E9");
        _colors.toolTipBase  = QColor("#FFFDF8");
        _colors.toolTipText  = QColor("#26231D");
        _colors.text         = QColor("#26231D");
        _colors.button       = QColor("#FBF8F1");
        _colors.buttonText   = QColor("#26231D");
        _colors.brightText   = QColor("#FFFFFF");
        _colors.link         = QColor("#B4522F");
        _colors.highlight    = QColor("#F0E1D5");
        _colors.highlightedText = QColor("#26231D");

        _colors.chrome       = QColor("#F4F1EA");
        _colors.panel        = QColor("#ECE7DB");
        _colors.border       = QColor("#E1D9CA");
        _colors.borderStrong = QColor("#CBBFA9");
        _colors.inkMuted     = QColor("#6B655A");
        _colors.inkFaint     = QColor("#948C7D");
        _colors.tabBackground= QColor("#EFEBE0");

        _colors.accent        = QColor("#C96442");
        _colors.accentHover   = QColor("#B85433");
        _colors.accentPressed = QColor("#9E4528");
        _colors.accentSoft    = QColor("#F4E4D9");
        _colors.onAccent      = QColor("#FFFCF8");

        _colors.rx           = QColor("#1F6F78");
        _colors.tx           = QColor("#A46A1F");
        _colors.dataChanged  = QColor("#9A5B1A");
        _colors.channels = {
            QColor("#C96442"), QColor("#2A7F86"), QColor("#A46A1F"), QColor("#6E5AA8"),
            QColor("#4E7A3F"), QColor("#3F6A94"), QColor("#8C5A4A"), QColor("#8A7A5C")
        };

        _colors.graphBackground = QColor("#FFFDF8");
        _colors.graphGrid       = QColor("#EAE4D6");
        _colors.graphAxisText   = QColor("#6B655A");
        _colors.graphCursor     = QColor("#26231D");

        _colors.statusSuccess = QColor("#3F7A4E");
        _colors.statusWarning = QColor("#966B12");
        _colors.statusError   = QColor("#B2352B");
        _colors.statusInfo    = QColor("#3E6A93");
        _colors.bannerWarningBg     = QColor("#FBF1DB");
        _colors.bannerWarningBorder = QColor("#DCC186");
        _colors.bannerWarningText   = QColor("#584414");
        _colors.cursorA       = QColor("#C96442");
        _colors.cursorB       = QColor("#1F6F78");
        _colors.triggerMarker = QColor("#7A5AA0");
    } else {
        // P1 keeps the existing dark scheme; only the new tokens are mapped.
        _colors.window       = QColor(45, 45, 48);
        _colors.windowText   = QColor(220, 220, 220);
        _colors.base         = QColor(30, 30, 30);
        _colors.alternateBase= QColor(37, 37, 38);
        _colors.toolTipBase  = QColor(48, 48, 52);
        _colors.toolTipText  = QColor(240, 240, 240);
        _colors.text         = QColor(220, 220, 220);
        _colors.button       = QColor(45, 45, 48);
        _colors.buttonText   = QColor(220, 220, 220);
        _colors.brightText   = Qt::white;
        _colors.link         = QColor(86, 156, 214);
        _colors.highlight    = QColor(38, 79, 120);
        _colors.highlightedText = Qt::white;

        _colors.chrome       = QColor(45, 45, 48);
        _colors.panel        = QColor(37, 37, 38);
        _colors.border       = QColor(63, 63, 70);
        _colors.borderStrong = QColor(90, 90, 96);
        _colors.inkMuted     = QColor(160, 160, 160);
        _colors.inkFaint     = QColor(101, 101, 101);
        _colors.tabBackground= QColor(37, 37, 38);

        _colors.accent        = QColor("#007ACC");
        _colors.accentHover   = QColor("#1A8AD4");
        _colors.accentPressed = QColor("#005A99");
        _colors.accentSoft    = QColor("#1F3A4D");
        _colors.onAccent      = QColor("#FFFFFF");

        _colors.rx           = QColor("#4FB3D9");
        _colors.tx           = QColor("#C97F33");
        _colors.dataChanged  = QColor("#D27800");
        _colors.channels = {
            QColor("#3498DB"), QColor("#E74C3C"), QColor("#2ECC71"), QColor("#F1C40F"),
            QColor("#9B59B6"), QColor("#1ABC9C"), QColor("#E67E22"), QColor("#5D8AA8")
        };

        _colors.graphBackground = QColor(25, 25, 25);
        _colors.graphGrid       = QColor(60, 60, 60);
        _colors.graphAxisText   = QColor(180, 180, 180);
        _colors.graphCursor     = Qt::white;

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
    const QColor disabledText = (theme == Light) ? _colors.inkFaint : QColor(127, 127, 127);
    qApp->setPalette(paletteFromColors(_colors, disabledText));
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
