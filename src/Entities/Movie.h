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
    QVector<People> getDirectors() { return m_directors; }
    QVector<People> getProducers() { return m_producers; }
    QVector<People> getActors() { return m_actors; }

private:
    int m_id;
    QString m_title;
    QString m_originalTitle;
    QVector<People> m_directors;
    QVector<People> m_producers;
    QVector<People> m_actors;
    int m_year;
    QString m_country;
    int m_duration;
    QString m_synopsys;
    QString m_filePath;
    bool colored;
    QString m_format;
    int m_rank;
    QVector<Tag> m_tags;
};

#endif // MOVIE_H
