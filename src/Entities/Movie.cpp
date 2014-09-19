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

#include "Movie.h"

Movie::Movie()
{
    m_title = "";
    m_originalTitle = "";
    m_year = 0;
    m_country =  "";
    m_duration = 0;
    m_synopsis = "";
    m_filePath = "";
    m_colored = true;
    m_format = "";
    m_suffix = "";
    m_rank = 0;
}

int Movie::getId() const
{
    return m_id;
}

void Movie::setId(int id)
{
    m_id = id;
}

QString Movie::getTitle() const
{
    return m_title;
}

void Movie::setTitle(QString title)
{
    m_title = title;
}

QString Movie::getOriginalTitle() const
{
    return m_originalTitle;
}

void Movie::setOriginalTitle(QString originalTitle)
{
    m_originalTitle = originalTitle;
}

QVector<People> Movie::getDirectors() const
{
    return m_directors;
}

void Movie::setDirectors(QVector<People> directors)
{
    m_directors = directors;
}

void Movie::addDirector(People director)
{
    m_directors.push_back(director);
}

void Movie::removeDirector(People director)
{
    int l_index = m_directors.indexOf(director);
    if (l_index != -1)
    {
        m_directors.removeAt(l_index);
    }
}

QVector<People> Movie::getProducers() const
{
    return m_producers;
}

void Movie::setProducers(QVector<People> producers)
{
    m_producers = producers;
}

void Movie::addProducer(People producer)
{
    m_producers.push_back(producer);
}

void Movie::removeProducer(People producer)
{
    int l_index = m_producers.indexOf(producer);
    if (l_index != -1)
    {
        m_producers.removeAt(l_index);
    }
}

QVector<People> Movie::getActors() const
{
    return m_actors;
}

void Movie::setActors(QVector<People> actors)
{
    m_actors = actors;
}

void Movie::addActor(People actor)
{
    m_actors.push_back(actor);
}

void Movie::removeActor(People actor)
{
    int l_index = m_actors.indexOf(actor);
    if (l_index != -1)
    {
        m_actors.removeAt(l_index);
    }
}

int Movie::getYear() const
{
    return m_year;
}

void Movie::setYear(int year)
{
    m_year = year;
}

QString Movie::getCountry() const
{
    return m_country;
}

void Movie::setCountry(QString country)
{
    m_country = country;
}

int Movie::getDuration() const
{
    return m_duration;
}

void Movie::setDuration(int duration)
{
    m_duration = duration;
}

QString Movie::getSynopsis() const
{
    return m_synopsis;
}

void Movie::setSynopsis(QString synopsis)
{
    m_synopsis = synopsis;
}

QString Movie::getFilePath() const
{
    return m_filePath;
}

void Movie::setFilePath(QString filePath)
{
    m_filePath = filePath;
}

bool Movie::isColored() const
{
    return m_colored;
}

void Movie::setColored(bool colored)
{
    m_colored = colored;
}

QString Movie::getFormat() const
{
    return m_format;
}

void Movie::setFormat(QString format)
{
    m_format = format;
}

QString Movie::getSuffix() const
{
    return m_suffix;
}

void Movie::setSuffix(QString suffix)
{
    m_suffix = suffix;
}

int Movie::getRank() const
{
    return m_rank;
}

void Movie::setRank(int rank)
{
    m_rank = rank;
}

QVector<Tag> Movie::getTags()  const
{
    return m_tags;
}

void Movie::addTag(Tag tag)
{
    m_tags.push_back(tag);
}

void Movie::removeTag(Tag tag)
{
    int l_index = m_tags.indexOf(tag);
    if (l_index != -1)
    {
        m_tags.removeAt(l_index);
    }
}

