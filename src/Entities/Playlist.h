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

#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QWidget>

#include "Movie.h"

class Playlist
{
public:
    Playlist();
    int getId() const;
    void setId(const int id);
    QString getName() const;
    void setName(const QString name);
    int getRate() const;
    void setRate(const int rate);
    QDateTime getCreationDate() const;
    void setCreationDate(const QDateTime creationDate);
    QList<Movie> getMovieList() const;
    void setMovieList(const QList<Movie> &movieList);
    void addMovie(const Movie &movie);
    void removeMovie(const Movie &movie);
    void updateMovie(const Movie &movie);

private:
    int m_id;
    QString m_name;
    int m_rate;
    QDateTime m_creationDate;
    QList<Movie> m_movieList;
};

#endif // PLAYLIST_H
