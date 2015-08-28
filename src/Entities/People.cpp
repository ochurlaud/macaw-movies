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

#include "People.h"

People::People(const QString name) :
    Entity(name)
{
    m_biography = "";
    m_tmdbId = 0;
    m_imported = false;
}

QDate People::birthday() const
{
    return m_birthday;
}

void People::setBirthday(const QDate birthday)
{
    m_birthday = birthday;
}

QString People::biography() const
{
    return m_biography;
}

void People::setBiography(const QString biography)
{
    m_biography = biography;
}

int People::type() const
{
    return m_type;
}

void People::setType(const int type)
{
    m_type = type;
}

int People::tmdbId() const
{
    return m_tmdbId;
}

void People::setTmdbId(const int id)
{
    m_tmdbId = id;
}

bool People::imported() const
{
    return m_imported;
}

void People::setImported(const bool imported)
{
    m_imported = imported;
}

bool People::operator== (const People &other)
{
    if ( this->id() == other.id() &&
         this->name() == other.name() &&
         this->birthday() == other.birthday() &&
         this->biography() == other.biography() &&
         this->type() == other.type() )
    {
        return true;
    }

    return false;
}

bool People::operator!= (const People &other)
{
    return !this->operator==(other);
}
