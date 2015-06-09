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

#include "Entity.h"

Entity::Entity(const QString name) :
    m_id(0),
    m_name(name)
{

}

int Entity::id() const
{
    return m_id;
}

void Entity::setId(int id)
{
    m_id = id;
}

QString Entity::name() const
{
    return m_name;
}

void Entity::setName(const QString name)
{
    m_name = name;
}

bool Entity::operator== (const Entity &other)
{
    if ( this->id() == other.id() &&
         this->name() == other.name() )
    {
        return true;
    }

    return false;
}

bool Entity::operator!= (const Entity &other)
{
    return !this->operator==(other);
}
