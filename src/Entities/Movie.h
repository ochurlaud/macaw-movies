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

#ifndef MOVIE_H
#define MOVIE_H

#include <QtWidgets>
#include "People.h"
#include "Tag.h"


class Movie
{
public:
    Movie();
    int getId() const;
    void setId(const int id);
    QString getTitle() const;
    void setTitle(const QString title);
    QString getOriginalTitle() const;
    void setOriginalTitle(const QString originalTitle);
    QList<People> getPeopleList() const;
    QList<People> getPeopleList(const int type);
    void setPeopleList(const QList<People> &peopleList);
    void addPeople(const People &people);
    void removePeople(const People &people);
    void updatePeople(const People &people);
    QDate getReleaseDate() const;
    void setReleaseDate(const QDate releaseDate);
    QString getCountry() const;
    void setCountry(const QString country);
    QTime getDuration() const;
    void setDuration(const QTime duration);
    QString getSynopsis() const;
    void setSynopsis(QString synopsis);
    QString getFilePath() const;
    void setFilePath(QString filePath);
    bool isColored() const;
    void setColored(const bool colored);
    QString getFormat() const;
    void setFormat(const QString format);
    QString getSuffix() const;
    void setSuffix(const QString suffix);
    int getRank() const;
    void setRank(const int rank);
    QList<Tag> getTagList() const;
    void setTagList(const QList<Tag> &tagList);
    void addTag(const Tag &tag);
    void removeTag(const Tag &tag);
    bool operator== (const Movie &other);
    bool operator!= (const Movie &other);

private:
    int m_id;
    QString m_title;
    QString m_originalTitle;
    QList<People> m_peopleList;
    QDate m_releaseDate;
    QString m_country;
    QTime m_duration;
    QString m_synopsis;
    QString m_filePath;
    bool m_colored;
    QString m_format;
    QString m_suffix;
    int m_rank;
    QList<Tag> m_tagList;
};

#endif // MOVIE_H
