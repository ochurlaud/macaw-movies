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

#include "Episode.h"

Episode::Episode():
    m_id(0),
    m_season(0),
    m_number(0)
{
}

int Episode::id() const
{
    return m_id;
}

void Episode::setId(const int id)
{
    m_id = id;
}

int Episode::number() const
{
    return m_number;
}

void Episode::setNumber(const int number)
{
    m_number = number;
}

Movie Episode::movie() const
{
    return m_movie;
}

void Episode::setMovie(const Movie &movie)
{
    m_movie = movie;
}

int Episode::season() const
{
    return m_season;
}

void Episode::setSeason(const int season)
{
    m_season = season;
}

Series Episode::series() const
{
    return m_series;
}

void Episode::setSeries(const Series &series)
{
    m_series = series;
}

bool Episode::operator== (const Episode &other)
{
    if ( this->id() == other.id() &&
         this->number() == other.number() &&
         this->season() == other.season() &&
         this->series() == other.series() &&
         this->movie() == other.movie() )
    {
        return true;
    }

    return false;
}

bool Episode::operator!= (const Episode &other)
{
    return !this->operator==(other);
}
