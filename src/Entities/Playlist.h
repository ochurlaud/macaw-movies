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

#ifndef PLAYLIST_H
#define PLAYLIST_H


#include "Entities/Entity.h"
#include "Entities/Movie.h"

class QDateTime;
template<class T> class QList;

class Movie;

class Playlist : public Entity
{
public:
    explicit Playlist(const QString name = "");
    int rate() const;
    void setRate(const int rate);
    QDateTime creationDate() const;
    void setCreationDate(const QDateTime creationDate);
    QList<Movie> movieList() const;
    void setMovieList(const QList<Movie> &movieList);
    void addMovie(const Movie &movie);
    void removeMovie(const Movie &movie);
    void updateMovie(const Movie &movie);
    enum typePlaylist { New = -1, None = 0, ToWatch = 1 };

private:
    int m_rate;
    QDateTime m_creationDate;
    QList<Movie> m_movieList;
};

#endif // PLAYLIST_H
