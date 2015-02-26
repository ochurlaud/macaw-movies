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

#include "FetchMetadata.h"
#include <QLocale>

FetchMetadata::FetchMetadata(Movie movie, QObject *parent) :
    QObject(parent),
    m_movie(movie)
{
    m_app = qobject_cast<Application *>(qApp);
    m_app->debug("[FetchMetadata] Constructor");

    m_fetchMetadataQuery = new FetchMetadataQuery(this);

    connect(m_fetchMetadataQuery, SIGNAL(primaryResponse(QList<Movie>&)),
            this, SLOT(processPrimaryResponse(QList<Movie>&)));
    connect(m_fetchMetadataQuery, SIGNAL(networkError(QString)),
            this, SLOT(networkError(QString)));

    QString l_cleanedTitle = cleanString(m_movie.title());
    m_fetchMetadataQuery->sendPrimaryRequest(l_cleanedTitle);

    QEventLoop l_loop;
    connect(this, SIGNAL(jobDone()),
            &l_loop, SLOT(quit()));

    l_loop.exec();
    m_app->debug("[FetchMetadata] Construction done");
}

FetchMetadata::~FetchMetadata()
{
    delete m_fetchMetadataQuery;
    delete m_fetchMetadataDialog;
    m_app->debug("[FetchMetadata] Object destructed");

}

QString FetchMetadata::cleanString(QString title)
{
    QRegExp l_sep("(\\_|\\-|\\ |\\,|\\.|\\!|\\?)");
    QStringList l_splittedTitle = title.split(l_sep);

    QRegExp l_alphaOnly("(^[À-Ÿà-ÿA-Za-z]*$)");
    l_splittedTitle = l_splittedTitle.filter(l_alphaOnly);

    return l_splittedTitle.join("%20");
}

void FetchMetadata::processPrimaryResponse(QList<Movie> &movieList)
{
    disconnect(m_fetchMetadataQuery, SIGNAL(primaryResponse(QList<Movie>&)),
            this, SLOT(processPrimaryResponse(QList<Movie>&)));

    m_app->debug("[FetchMetadata] Signal from primary request received");

    if (movieList.count() == 1) {
        Movie l_movie = movieList.at(0);

        connect(m_fetchMetadataQuery, SIGNAL(movieResponse(Movie&)),
                this, SLOT(processMovieResponse(Movie&)));
        m_app->debug("[FetchMetadata] Movie request to be sent");
        m_fetchMetadataQuery->sendMovieRequest(l_movie.id());
    } else {
        m_fetchMetadataDialog = new FetchMetadataDialog(m_movie, movieList);
        connect(m_fetchMetadataDialog, SIGNAL(selectedMovie(Movie&)),
                this, SLOT(on_selectedMovie(Movie&)));
        connect(m_fetchMetadataDialog, SIGNAL(searchMovies(QString)),
                this, SLOT(on_searchMovies(QString)));
        m_fetchMetadataDialog->show();
    }
}

void FetchMetadata::processMovieResponse(Movie &receivedMovie)
{
    m_app->debug("[FetchMetadata] Signal from movie request received");

    disconnect(m_fetchMetadataQuery, SIGNAL(movieResponse(Movie&)),
            this, SLOT(processMovieResponse(Movie&)));

    // Do not set the id since receivedMovie's id is from TMDB
    m_movie.setTitle(receivedMovie.title());
    m_movie.setOriginalTitle(receivedMovie.originalTitle());
    m_movie.setReleaseDate(receivedMovie.releaseDate());
    m_movie.setDuration(receivedMovie.duration());
    m_movie.setCountry(receivedMovie.country());
    m_movie.setSynopsis(receivedMovie.synopsis());
    m_movie.setPeopleList(receivedMovie.peopleList());
    m_movie.setColored(receivedMovie.isColored());
    m_movie.setPeopleList(receivedMovie.peopleList());

    m_app->getDatabaseManager()->updateMovie(m_movie);
    emit(jobDone());
}

void FetchMetadata::on_searchCanceled()
{
    emit(jobDone());
}

void FetchMetadata::on_selectedMovie(Movie &movie)
{
    QList<Movie> l_movieList;
    l_movieList.append(movie);
    processPrimaryResponse(l_movieList);
}

void FetchMetadata::on_searchMovies(QString title)
{
    connect(m_fetchMetadataQuery, SIGNAL(primaryResponse(QList<Movie>&)),
            this, SLOT(processPrimaryResponseDialog(QList<Movie>&)));

    QString l_cleanedTitle = cleanString(title);
    m_fetchMetadataQuery->sendPrimaryRequest(l_cleanedTitle);
}

void FetchMetadata::processPrimaryResponseDialog(QList<Movie> &movieList)
{
    m_fetchMetadataDialog->setMovieList(movieList);
}

void FetchMetadata::networkError(QString error)
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setText("Network error: " + error);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}
