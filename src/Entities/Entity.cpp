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
