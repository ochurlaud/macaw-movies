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

    FetchMetadataQuery l_fetchMetadataQuery;
    QObject::connect(&l_fetchMetadataQuery, SIGNAL(primaryResponse(QList<Movie>)),
            this, SLOT(processPrimaryResponse(QList<Movie>)));

    QString l_cleanedTitle = cleanString(m_movie.title());
    l_fetchMetadataQuery.sendPrimaryRequest(l_cleanedTitle);

    QEventLoop l_loop;
    connect(this, SIGNAL(jobDone()),
            &l_loop, SLOT(quit()));

    l_loop.exec();
    m_app->debug("[FetchMetadata] Construction done");
}

/*
void FetchMetadata::fetchMetadata(QString title)
{
    m_app->debug("[FetchMetadata] Enter fetchMetadata, " + title);
    this->getRelatedMovies(title);
    connect(this, SIGNAL(movieHydrated(Movie&)),
            this, SLOT(updateMovieInDatabase(Movie&)));

    QEventLoop l_loop;
    connect(this, SIGNAL(movieHydrated(Movie&)),
            &l_loop, SLOT(quit()));
    connect(this, SIGNAL(noMovieFound()),
            &l_loop, SLOT(quit()));

    l_loop.exec();
    m_app->debug("[FetchMetadata] Exit fetchMetadata");
}
*/

QString FetchMetadata::cleanString(QString title)
{
    QRegExp l_sep("(\\_|\\-|\\ |\\,|\\.|\\!|\\?)");
    QStringList l_splittedTitle = title.split(l_sep);

    QRegExp l_alphaOnly("(^[À-Ÿà-ÿA-Za-z]*$)");
    l_splittedTitle = l_splittedTitle.filter(l_alphaOnly);

    return l_splittedTitle.join("%20");
}

void FetchMetadata::processPrimaryResponse(const QList<Movie> movieList)
{
    m_app->debug("[FetchMetadata] Signal from primary request received");
    qDebug() << movieList[0].title();
    qDebug() << movieList[0].id();
    if (movieList.count() == 1) {
        Movie l_movie = movieList.at(0);
        FetchMetadataQuery l_fetchMetadataQuery;
        QObject::connect(&l_fetchMetadataQuery, SIGNAL(finalResponse(Movie)),
                this, SLOT(processFinalResponse(Movie)));
        m_app->debug("[FetchMetadata] Final request to be sent");
        l_fetchMetadataQuery.sendFinalRequest(l_movie.id());
    } else {
        FetchMetadataDialog l_fetchMetadataDialog(m_movie, movieList);
    }
}

void FetchMetadata::processFinalResponse(const Movie receivedMovie)
{
    m_app->debug("[FetchMetadata] Signal from final request received");

    m_movie.setTitle(receivedMovie.title());
    m_movie.setOriginalTitle(receivedMovie.originalTitle());
    m_movie.setReleaseDate(receivedMovie.releaseDate());
    m_movie.setDuration(receivedMovie.duration());
    m_movie.setCountry(receivedMovie.country());
    m_movie.setSynopsis(receivedMovie.synopsis());
    m_movie.setPeopleList(receivedMovie.peopleList());
    m_movie.setColored(receivedMovie.isColored());

    m_app->getDatabaseManager()->updateMovie(m_movie);
    emit(jobDone());
}
