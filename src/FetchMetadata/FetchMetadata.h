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

#ifndef FETCHMETADATA_H
#define FETCHMETADATA_H

#include <QWidget>
#include <QLocale>

#include "Application.h"
#include "FetchMetadata/FetchMetadataDialog.h"
#include "FetchMetadata/FetchMetadataQuery.h"

class FetchMetadata : public QObject
{
Q_OBJECT

public:
    explicit FetchMetadata(Movie movie, QObject *parent = 0);
    void fetchMetadata(QString title);

signals:
    void jobDone();

public slots:
    void processPrimaryResponse(const QList<Movie> movieList);
    void processFinalResponse(const Movie receivedMovie);

private:
    Application *m_app;
    Movie m_movie;
    void getRelatedMovies(QString title);
    void getMetadata(int tmdbID);
    QString cleanString(QString title);

};

#endif // FETCHMETADATA_H
