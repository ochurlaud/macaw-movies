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

People::People()
{
    m_id = 0;
    m_lastname = "";
    m_firstname = "";
    m_realname ="";
    m_biography = "";
}

int People::id() const
{
    return m_id;
}

void People::setId(const int id)
{
    m_id = id;
}

QString People::lastname() const
{
    return m_lastname;
}

void People::setLastname(const QString lastname)
{
    m_lastname = lastname;
}

QString People::firstname() const
{
    return m_firstname;
}

void People::setFirstname(const QString firstname)
{
    m_firstname = firstname;
}

QString People::realname() const
{
    return m_realname;
}

void People::setRealname(const QString realname)
{
    m_realname = realname;
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

bool People::operator== (const People &other)
{
    if ( this->id() == other.id() &&
         this->firstname() == other.firstname() &&
         this->lastname() == other.lastname() &&
         this->realname() == other.realname() &&
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
