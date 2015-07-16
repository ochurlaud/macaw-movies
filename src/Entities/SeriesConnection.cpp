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

#include "SeriesConnection.h"

SeriesConnection::SeriesConnection():
    m_id(0),
    m_season(0),
    m_episode(0)
{
}

int SeriesConnection::id() const
{
    return m_id;
}

void SeriesConnection::setId(const int id)
{
    m_id = id;
}

int SeriesConnection::episode() const
{
    return m_episode;
}

void SeriesConnection::setEpisode(const int episode)
{
    m_episode = episode;
}

Movie SeriesConnection::movie() const
{
    return m_movie;
}

void SeriesConnection::setMovie(const Movie &movie)
{
    m_movie = movie;
}

int SeriesConnection::season() const
{
    return m_season;
}

void SeriesConnection::setSeason(const int season)
{
    m_season = season;
}
/*
Series SeriesConnection::series() const
{
    return m_series;
}

void SeriesConnection::setSeries(const Series &series)
{
    m_series = series;
}
*/
bool SeriesConnection::operator== (const SeriesConnection &other)
{
    if ( this->id() == other.id() &&
         this->episode() == other.episode() &&
         this->season() == other.season() &&
         /*this->series() == other.series() &&*/
         this->movie() == other.movie() )
    {
        return true;
    }

    return false;
}

bool SeriesConnection::operator!= (const SeriesConnection &other)
{
    return !this->operator==(other);
}
