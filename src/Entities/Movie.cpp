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
    m_id = 0;
    m_title = "";
    m_originalTitle = "";
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

void Movie::setId(const int id)
{
    m_id = id;
}

QString Movie::getTitle() const
{
    return m_title;
}

void Movie::setTitle(const QString title)
{
    m_title = title;
}

QString Movie::getOriginalTitle() const
{
    return m_originalTitle;
}

void Movie::setOriginalTitle(const QString originalTitle)
{
    m_originalTitle = originalTitle;
}

QList<People> Movie::getPeopleList() const
{
    return m_peopleList;
}

QList<People> Movie::getPeopleList(const int type)
{
    QList<People> l_peopleList;
    foreach (People l_people, m_peopleList)
    {
        if (l_people.getType() == type)
        {
            l_peopleList.push_back(l_people);
        }
    }

    return l_peopleList;
}

void Movie::setPeopleList(const QList<People> &peopleList)
{
    m_peopleList = peopleList;
}

void Movie::addPeople(const People &people)
{
    m_peopleList.push_back(people);
}

void Movie::removePeople(const People &people)
{
    int l_index = m_peopleList.indexOf(people);
    if (l_index != -1)
    {
        m_peopleList.removeAt(l_index);
    }
}

/**
 * @brief Movie::updateDirector
 * @param People
 */
void Movie::updatePeople(const People &people)
{
    for (int i = 0 ; i < m_peopleList.size() ; i++)
    {
        if(m_peopleList.at(i).getId() == people.getId())
        {
            m_peopleList.replace(i, people);
        }
   }
}

QDate Movie::getReleaseDate() const
{
    return m_releaseDate;
}

void Movie::setReleaseDate(QDate releaseDate)
{
    m_releaseDate = releaseDate;
}

QString Movie::getCountry() const
{
    return m_country;
}

void Movie::setCountry(QString country)
{
    m_country = country;
}

QTime Movie::getDuration() const
{
    return m_duration;
}

void Movie::setDuration(const QTime duration)
{
    m_duration = duration;
}

QString Movie::getSynopsis() const
{
    return m_synopsis;
}

void Movie::setSynopsis(const QString synopsis)
{
    m_synopsis = synopsis;
}

QString Movie::getFilePath() const
{
    return m_filePath;
}

void Movie::setFilePath(const QString filePath)
{
    m_filePath = filePath;
}

bool Movie::isColored() const
{
    return m_colored;
}

void Movie::setColored(const bool colored)
{
    m_colored = colored;
}

QString Movie::getFormat() const
{
    return m_format;
}

void Movie::setFormat(const QString format)
{
    m_format = format;
}

QString Movie::getSuffix() const
{
    return m_suffix;
}

void Movie::setSuffix(const QString suffix)
{
    m_suffix = suffix;
}

int Movie::getRank() const
{
    return m_rank;
}

void Movie::setRank(const int rank)
{
    m_rank = rank;
}

QList<Tag> Movie::getTagList()  const
{
    return m_tagList;
}


void Movie::setTagList(const QList<Tag> &tagList)
{
    m_tagList = tagList;
}

void Movie::addTag(const Tag &tag)
{
    m_tagList.push_back(tag);
}

void Movie::removeTag(const Tag &tag)
{
    int l_index = m_tagList.indexOf(tag);
    if (l_index != -1)
    {
        m_tagList.removeAt(l_index);
    }
}


bool Movie::operator== (const Movie &other)
{
    if ( this->getId() == other.getId() &&
         this->getTitle() == other.getTitle() &&
         this->getOriginalTitle() == other.getOriginalTitle() &&
         this->getCountry() == other.getCountry() &&
         this->getReleaseDate() == other.getReleaseDate() &&
         this->getDuration() == other.getDuration() &&
         this->getFilePath() == other.getFilePath() &&
         this->getSynopsis() == other.getSynopsis() &&
         this->getSuffix() == other.getSuffix() &&
         this->getRank() == other.getRank() &&
         this->getFormat() == other.getFormat() &&
         this->isColored() == other.isColored() &&
         this->getPeopleList() == other.getPeopleList() &&
         this->getTagList() == other.getTagList() )
    {
        return true;
    }

    return false;
}

bool Movie::operator!= (const Movie &other)
{
    return !this->operator==(other);
}
