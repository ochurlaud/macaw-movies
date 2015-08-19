/* Copyright (C) 2014 Macaw-Movies
 * (Olivier CHURLAUD, Sébastien TOUZÉ)
 *
 * This file is part of Macaw-Movies.
 *
 * Macaw-Movies is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Macaw-Movies is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Macaw-Movies.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "MacawDebug.h"

#include <QtDebug>

MacawDebug::MacawDebug():
    m_debug(false)
  , m_spaces(0)
{}

void MacawDebug::print(const QString text)
{
    if (isDebug())
    {
        qDebug() << qPrintable(this->space())
                 << qPrintable(text);
    }
    // else do nothing
}

void MacawDebug::printIn(const QString text)
{
    this->print(text);
    m_spaces++;
}

void MacawDebug::printOut(const QString text)
{
    m_spaces--;
    this->print(text);
}

QString MacawDebug::space()
{
    QString l_space(m_spaces, ' ');

    return l_space;
}

namespace Macaw
{
    MacawDebug macawDebug_extern;
}
