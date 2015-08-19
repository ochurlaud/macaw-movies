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

#ifndef FETCH_H
#define FETCH_H

#include <QObject>

#include "Entities/Movie.h"

class Application;
class FetchMetadataDialog;
class FetchMetadataQuery;
class Movie;

/**
 * @brief The FetchMetadata class
 *
 * @author Olivier CHURLAUD <olivier@churlaud.com>
 */
class FetchMetadata : public QObject
{
Q_OBJECT

public:
    explicit FetchMetadata(QObject *parent = 0);
    ~FetchMetadata();
    void addMoviesToQueue(const QList<Movie> &movieList);

signals:
    void jobDone();
    void exitInitWaitingLoop();
    void updatedMovie();

private slots:
    void initTimerDone();
    void processPrimaryResponse(const QList<Movie> &movieList);
    void processMovieResponse(const Movie &receivedMovie);
    void on_selectedMovie(const Movie &movie);
    void on_searchMovies(const QString title);
    void processPrimaryResponseDialog(const QList<Movie> &movieList);
    void on_searchCanceled();
    void on_dontAskUser();
    void on_neverAskUser(Movie movie);
    void on_jobDone();
    void networkError(QString error);

private:
    FetchMetadataQuery *m_fetchMetadataQuery;

    /**
     * @brief Dialog for metadata-fetching manager
     */
    FetchMetadataDialog *m_fetchMetadataDialog;

    Movie m_movie;
    QList<Movie> m_movieQueue;
    bool m_askUser;
    bool m_running;
    int m_initialMovieQueueSize, m_moviesProcessed;
    QString cleanString(const QString title);
    void openFetchMetadataDialog(const Movie &movie, const QList<Movie> &accurateList);
    void updateFetchMetadataDialog(const QList<Movie> &updatedList);
    void startProcess();
};

#endif // FETCH_H
