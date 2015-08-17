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

bool PathForMovies::hasMovies() const
{
    return (m_type & 01) >> 0;
}
void PathForMovies::setMovies(const bool movies)
{
    m_type = m_type | (movies & Macaw::movie);
}

bool PathForMovies::hasShows() const
{
    return (m_type & 10) >> 1;
}

void PathForMovies::setShows(const bool shows)
{
    m_type = m_type | (shows & Macaw::show);
}
