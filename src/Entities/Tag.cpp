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

#include "Tag.h"

Tag::Tag()
{
    m_name = "";
}

int Tag::getId() const
{
    return m_id;
}

void Tag::setId(int id)
{
    m_id = id;
}

QString Tag::getName() const
{
    return m_name;
}

void Tag::setName(QString name)
{
    m_name = name;
}

bool Tag::operator== (const Tag& other)
{
    if ( this->getId() == other.getId() &&
         this->getName() == other.getName() )
    {
        return true;
    }

    return false;
}
