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

#pragma once

#include <QColor>
#include <QObject>
#include <QPalette>
#include <QString>
#include <array>

struct ThemeColors {
    // Qt palette roles
    QColor window;
    QColor windowText;
    QColor base;
    QColor alternateBase;
    QColor toolTipBase;
    QColor toolTipText;
    QColor text;
    QColor button;
    QColor buttonText;
    QColor brightText;
    QColor link;
    QColor highlight;
    QColor highlightedText;

    // Chrome surfaces
    QColor chrome;       // menu bar / toolbars / status bar / dock chrome
    QColor panel;        // dock headers, table headers, group bands
    QColor border;       // 1px borders and grid lines
    QColor borderStrong; // input borders and focus frames
    QColor inkMuted;     // secondary text
    QColor inkFaint;     // disabled / placeholder text
    QColor tabBackground;// inner tab page background

    // Action accent (used for primary actions, focus and selection accents)
    QColor accent;
    QColor accentHover;
    QColor accentPressed;
    QColor accentSoft;
    QColor onAccent;

    // Data semantics
    QColor rx;
    QColor tx;
    QColor dataChanged;
    std::array<QColor, 8> channels;

    // Graph specific
    QColor graphBackground;
    QColor graphGrid;
    QColor graphAxisText;
    QColor graphCursor;

    // Semantic status, banner & measurement tokens
    QColor statusSuccess;
    QColor statusWarning;
    QColor statusError;
    QColor statusInfo;
    QColor bannerWarningBg;
    QColor bannerWarningBorder;
    QColor bannerWarningText;
    QColor cursorA;
    QColor cursorB;
    QColor triggerMarker;
};

class ThemeManager : public QObject
{
    Q_OBJECT
public:
    enum Theme {
        Light,
        Dark
    };

    static ThemeManager& instance();

    // Applies the bundled palette and stylesheet for the given theme. CANgaroo
    // ships its own visual language; the native OS palette is no longer used.
    void applyTheme(Theme theme);

    Theme currentTheme() const { return _currentTheme; }
    bool isDarkMode() const { return _currentTheme == Dark; }

    const ThemeColors& colors() const { return _colors; }

signals:
    void themeChanged(Theme theme);

private:
    explicit ThemeManager(QObject *parent = nullptr);
    Theme _currentTheme;
    ThemeColors _colors;

    void updateColors(Theme theme);
    void applyStyleSheet(Theme theme);
    void applyPalette(Theme theme);
};
