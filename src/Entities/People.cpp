/* Copyright (C) 2014 Movie-Project
 * (Olivier CHURLAUD, Sébastien TOUZÉ)
 *
 * This file is part of Movie-Project.
 *
 * Movie-Project is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Movie-Project is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Movie-Project.  If not, see <http://www.gnu.org/licenses/>.
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

int People::getId() const
{
    return m_id;
}

void People::setId(int id)
{
    m_id = id;
}

QString People::getLastname() const
{
    return m_lastname;
}

void People::setLastname(QString lastname)
{
    m_lastname = lastname;
}

QString People::getFirstname() const
{
    return m_firstname;
}

void People::setFirstname(QString firstname)
{
    m_firstname = firstname;
}

QString People::getRealname() const
{
    return m_realname;
}

void People::setRealname(QString realname)
{
    m_realname = realname;
}

QDate People::getBirthday() const
{
    return m_birthday;
}

void People::setBirthday(QDate birthday)
{
    m_birthday = birthday;
}

QString People::getBiography() const
{
    return m_biography;
}

void People::setBiography(QString biography)
{
    m_biography = biography;
}

bool People::operator== (const People& other)
{
    if ( this->getId() == other.getId() &&
         this->getFirstname() == other.getFirstname() &&
         this->getLastname() == other.getLastname() &&
         this->getRealname() == other.getRealname() &&
         this->getBirthday() == other.getBirthday() &&
         this->getBiography() == other.getBiography() )
    {
        return true;
    }

    return false;
}

bool People::operator!= (const People& other)
{
    return !this->operator==(other);
}
