#include "PathForMovies.h"

PathForMovies::PathForMovies():
    m_id(0),
    m_path("")
{
}


int PathForMovies::id() const
{
    return m_id;
}

void PathForMovies::setId(const int id)
{
    m_id = id;
}

QString PathForMovies::path() const
{
    return m_path;
}

void PathForMovies::setPath(const QString path)
{
    m_path = path;
}
