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

#ifndef MOVIE_H
#define MOVIE_H

#include <QtWidgets>

#include "People.h"
#include "Series.h"
#include "Tag.h"

class  Movie
{
public:
    Movie();
    int id() const;
    void setId(const int id);
    QString title() const;
    void setTitle(const QString title);
    QString originalTitle() const;
    void setOriginalTitle(const QString originalTitle);
    bool isColored() const;
    void setColored(const bool colored);
    QString country() const;
    void setCountry(const QString country);
    QTime duration() const;
    void setDuration(const QTime duration);
    QString filePath() const;
    void setFilePath(const QString filePath);
    QString format() const;
    void setFormat(const QString format);
    bool isImported() const;
    void setImported(const bool imported);
    QString posterPath() const;
    void setPosterPath(const QString posterPath);
    int rank() const;
    void setRank(const int rank);
    QDate releaseDate() const;
    void setReleaseDate(const QDate releaseDate);
    QString suffix() const;
    void setSuffix(const QString suffix);
    QString synopsis() const;
    void setSynopsis(const QString synopsis);
    QList<People> peopleList() const;
    QList<People> peopleList(const int type) const;
    void setPeopleList(const QList<People> &peopleList);
    void addPeople(const People &people);
    void removePeople(const People &people);
    void updatePeople(const People &people);
    QList<Tag> tagList() const;
    void setTagList(const QList<Tag> &tagList);
    void addTag(const Tag &tag);
    void removeTag(const Tag &tag);
    bool operator== (const Movie &other);
    bool operator!= (const Movie &other);

private:
    int m_id;
    QString m_title;
    QString m_originalTitle;
    bool m_colored;
    QString m_country;
    QTime m_duration;
    QString m_filePath;
    QString m_format;
    bool m_imported;
    QString m_posterPath;
    int m_rank;
    QDate m_releaseDate;
    bool m_isSeries;
    QString m_suffix;
    QString m_synopsis;
    QList<People> m_peopleList;
    QList<Tag> m_tagList;
};

Q_DECLARE_METATYPE(Movie)

#endif // MOVIE_H
