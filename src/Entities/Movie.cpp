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

#include "Movie.h"

Movie::Movie()
{
    m_id = 0;
    m_title = "";
    m_originalTitle = "";
    m_country =  "";
    m_synopsis = "";
    m_filePath = "";
    m_colored = true;
    m_format = "";
    m_suffix = "";
    m_rank = 0;
    m_imported = false;
}

int Movie::id() const
{
    return m_id;
}

void Movie::setId(const int id)
{
    m_id = id;
}

QString Movie::title() const
{
    return m_title;
}

void Movie::setTitle(const QString title)
{
    m_title = title;
}

QString Movie::originalTitle() const
{
    return m_originalTitle;
}

void Movie::setOriginalTitle(const QString originalTitle)
{
    m_originalTitle = originalTitle;
}

QList<People> Movie::peopleList() const
{
    return m_peopleList;
}

QList<People> Movie::peopleList(const int type) const
{
    QList<People> l_peopleList;
    foreach (People l_people, m_peopleList)
    {
        if (l_people.type() == type)
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

void Movie::updatePeople(const People &people)
{
    for (int i = 0 ; i < m_peopleList.size() ; i++)
    {
        if(m_peopleList.at(i).id() == people.id())
        {
            m_peopleList.replace(i, people);
        }
   }
}

QDate Movie::releaseDate() const
{
    return m_releaseDate;
}

void Movie::setReleaseDate(QDate releaseDate)
{
    m_releaseDate = releaseDate;
}

QString Movie::country() const
{
    return m_country;
}

void Movie::setCountry(QString country)
{
    m_country = country;
}

QTime Movie::duration() const
{
    return m_duration;
}

void Movie::setDuration(const QTime duration)
{
    m_duration = duration;
}

QString Movie::synopsis() const
{
    return m_synopsis;
}

void Movie::setSynopsis(const QString synopsis)
{
    m_synopsis = synopsis;
}

QString Movie::filePath() const
{
    return m_filePath;
}

void Movie::setFilePath(const QString filePath)
{
    m_filePath = filePath;
}

QString Movie::posterPath() const
{
    return m_posterPath;
}

void Movie::setPosterPath(const QString posterPath)
{
    m_posterPath = posterPath;
}

bool Movie::isColored() const
{
    return m_colored;
}

void Movie::setColored(const bool colored)
{
    m_colored = colored;
}

QString Movie::format() const
{
    return m_format;
}

void Movie::setFormat(const QString format)
{
    m_format = format;
}

QString Movie::suffix() const
{
    return m_suffix;
}

void Movie::setSuffix(const QString suffix)
{
    m_suffix = suffix;
}

int Movie::rank() const
{
    return m_rank;
}

void Movie::setRank(const int rank)
{
    m_rank = rank;
}

bool Movie::isImported() const
{
    return m_imported;
}

void Movie::setImported(const bool imported)
{
    m_imported = imported;
}

QList<Tag> Movie::tagList()  const
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
    if ( this->id() == other.id() &&
         this->title() == other.title() &&
         this->originalTitle() == other.originalTitle() &&
         this->country() == other.country() &&
         this->releaseDate() == other.releaseDate() &&
         this->duration() == other.duration() &&
         this->filePath() == other.filePath() &&
         this->synopsis() == other.synopsis() &&
         this->suffix() == other.suffix() &&
         this->rank() == other.rank() &&
         this->format() == other.format() &&
         this->isColored() == other.isColored() &&
         this->peopleList() == other.peopleList() &&
         this->tagList() == other.tagList() )
    {
        return true;
    }

    return false;
}

bool Movie::operator!= (const Movie &other)
{
    return !this->operator==(other);
}
