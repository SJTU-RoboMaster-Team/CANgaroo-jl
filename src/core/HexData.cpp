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

#include "HexData.h"

#include <QStringList>
#include <QChar>

namespace
{
bool isHexDigit(QChar c)
{
    return (c >= QLatin1Char('0') && c <= QLatin1Char('9'))
        || (c >= QLatin1Char('a') && c <= QLatin1Char('f'))
        || (c >= QLatin1Char('A') && c <= QLatin1Char('F'));
}
} // namespace

std::optional<QByteArray> HexData::parseByteString(const QString &text)
{
    QString normalized = text;
    for (const QChar separator : {QLatin1Char(','), QLatin1Char(';'),
                                  QLatin1Char('\t'), QLatin1Char('\n'),
                                  QLatin1Char('\r')})
    {
        normalized.replace(separator, QLatin1Char(' '));
    }

    const QStringList tokens = normalized.split(QLatin1Char(' '), Qt::SkipEmptyParts);
    if (tokens.isEmpty())
        return std::nullopt;

    QByteArray bytes;
    for (QString token : tokens)
    {
        if (token.startsWith(QLatin1String("0x"), Qt::CaseInsensitive))
            token.remove(0, 2);
        if (token.isEmpty())
            return std::nullopt;

        for (const QChar c : token)
        {
            if (!isHexDigit(c))
                return std::nullopt;
        }

        // A token longer than one byte is accepted as a compact byte run and
        // must therefore have an even number of hex digits.
        if (token.size() > 2 && token.size() % 2 != 0)
            return std::nullopt;

        if (token.size() <= 2)
        {
            bytes.append(static_cast<char>(token.toUInt(nullptr, 16)));
            continue;
        }

        for (qsizetype i = 0; i < token.size(); i += 2)
        {
            bytes.append(static_cast<char>(token.mid(i, 2).toUInt(nullptr, 16)));
        }
    }

    return bytes;
}
