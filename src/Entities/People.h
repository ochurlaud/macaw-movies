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

#ifndef PEOPLE_H
#define PEOPLE_H

#include <QDate>

#include "Entities/Entity.h"

class QDate;
class QString;

/**
 * @brief The People class
 *
 * @author Olivier CHURLAUD <olivier@churlaud.com>
 */
class People : public Entity
{
public:
    explicit People(const QString name = "");
    QDate birthday() const;
    void setBirthday(const QDate birthday);
    QString biography() const;
    void setBiography(const QString biography);
    int type() const;
    void setType(const int type);
    int tmdbId() const;
    void setTmdbId(const int id);
    bool imported() const;
    void setImported(const bool imported);
    bool operator== (const People&);
    bool operator!= (const People&);

    enum typePeople {None, Director, Producer, Actor};

private:
    QDate m_birthday;
    QString m_biography;
    int m_type;
    int m_tmdbId;
    bool m_imported;
};

#endif // PEOPLE_H
