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

#ifndef MACAWDEBUG_H
#define MACAWDEBUG_H

#include <QObject>

/**
 * @brief Debug mode using qDebug()
 * @author Olivier CHURLAUD <olivier@churlaud.com>
 */
class MacawDebug : public QObject
{
    Q_OBJECT

public:
    MacawDebug();
    void print(const QString);
    void printIn(const QString);
    void printOut(const QString);
    void setDebug(bool value) { m_debug = value; }
    bool isDebug() { return m_debug; }

private:
    bool m_debug;
    int m_spaces;
    QString space();

};

namespace Macaw {
    extern MacawDebug macawDebug_extern;
    inline void DEBUG(QString text) { macawDebug_extern.print(text);}
    inline void DEBUG_IN(QString text) { macawDebug_extern.printIn(text);}
    inline void DEBUG_OUT(QString text) { macawDebug_extern.printOut(text);}
}

#endif // MACAWDEBUG_H
