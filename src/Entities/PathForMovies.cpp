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

#include "PathForMovies.h"
#include "QDebug"
PathForMovies::PathForMovies(QString path, bool movies, bool shows) :
    m_id(0),
    m_path(path)
{
    setMovies(movies);
    setShows(shows);
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
    return (m_type & Macaw::hasMovies) >> Macaw::hasMovies/2;
}

void PathForMovies::setMovies(const bool movies)
{
    m_type = (m_type & (255^Macaw::hasMovies)) | ((movies << Macaw::hasMovies/2) & Macaw::hasMovies);
}

bool PathForMovies::hasShows() const
{
    return (m_type & Macaw::hasShows) >> Macaw::hasShows/2;
}

void PathForMovies::setShows(const bool shows)
{
    /*
     * if m_type = 10, shows = true  : (10 & 11101) | 1 << (2 & 10) = 10
     * if m_type = 01, shows = true  : (11 & 11101) | 1 << (2 & 10) = 11
     * if m_type = 01, shows = false : (11 & 11101) | 0 << (2 & 10) = 01
     * if m_type = 10, shows = false : (11 & 11101) | 0 << (2 & 10) = 00
     * ...
     */
    m_type = (m_type & (255^Macaw::hasShows)) | ((shows << Macaw::hasShows/2) & Macaw::hasShows);
}

int PathForMovies::type() const
{
    return m_type;
}

void PathForMovies::setType(const int type)
{
    m_type = type;
}
