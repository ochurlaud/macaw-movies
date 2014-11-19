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

#include "Playlist.h"

Playlist::Playlist()
{
}

int Playlist::getId() const
{
    return m_id;
}

void Playlist::setId(const int id)
{
    m_id = id;
}

QString Playlist::getName() const
{
    return m_name;
}

void Playlist::setName(const QString name)
{
    m_name = name;
}

int Playlist::getRate() const
{
    return m_rate;
}

void Playlist::setRate(const int rate)
{
    m_rate = rate;
}

QDateTime Playlist::getCreationDate() const
{
    return m_creationDate;
}

void Playlist::setCreationDate(const QDateTime creationDate)
{
    m_creationDate = creationDate;
}

QList<Movie> Playlist::getMovieList() const
{
    return m_movieList;
}
void Playlist::setMovieList(const QList<Movie> &movieList)
{
    m_movieList = movieList;
}

void Playlist::addMovie(const Movie &movie)
{
    m_movieList.push_back(movie);
}

void Playlist::removeMovie(const Movie &movie)
{
    int l_index = m_movieList.indexOf(movie);
    if (l_index != -1)
    {
        m_movieList.removeAt(l_index);
    }
}

/**
 * @brief Movie::updateDirector
 * @param People
 */
void Playlist::updateMovie(const Movie &movie)
{
    for (int i = 0 ; i < m_movieList.size() ; i++)
    {
        if(m_movieList.at(i).getId() == movie.getId())
        {
            m_movieList.replace(i, movie);
        }
    }
}
