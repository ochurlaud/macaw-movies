#ifndef ENTITY_H
#define ENTITY_H

#include <QtWidgets>

class Entity
{
public:
    Entity(const QString name = "");
    int id() const;
    void setId(int id);
    QString name() const;
    void setName(const QString name);
    bool operator== (const Entity &other);
    bool operator!= (const Entity &other);

protected:
    int m_id;
    QString m_name;
};

#endif // ENTITY_H
