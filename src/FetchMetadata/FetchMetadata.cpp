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

#include <QEventLoop>
#include <QMessageBox>
#include <QTimer>

#include "DatabaseManager.h"
#include "MacawDebug.h"
#include "ServicesManager.h"
#include "FetchMetadata/FetchMetadataQuery.h"
#include "FetchMetadata/FetchMetadataDialog.h"

FetchMetadata::FetchMetadata(QObject *parent) :
    QObject(parent)
{
    Macaw::DEBUG("[FetchMetadata] Constructor");

    m_running = false;
    m_askUser = true;
    m_fetchMetadataQuery = new FetchMetadataQuery(this);
    m_fetchMetadataDialog = NULL;
    m_initialMovieQueueSize = 0;
    m_moviesProcessed = 0;

    connect(this, SIGNAL(jobDone()),
            this, SLOT(on_jobDone()));

    Macaw::DEBUG("[FetchMetadata] Construction done");
}

FetchMetadata::~FetchMetadata()
{
    Macaw::DEBUG("[FetchMetadata] Object destructed");
}

void FetchMetadata::addMoviesToQueue(const QList<Movie> &movieList)
{
    Macaw::DEBUG("[FetchMetadata] Add movies to the queue list");

    m_movieQueue.append(movieList);
    if (m_movieQueue.count() == movieList.count()) {
        this->startMovieProcess();
    }
}

void FetchMetadata::addPeopleToQueue(const QList<People> &peopleList)
{
    Macaw::DEBUG("[FetchMetadata] Add people to the queue list");

    m_peopleQueue.append(peopleList);

    if (m_peopleQueue.count() == peopleList.count()) {
        this->startPeopleProcess();
    }
}
void FetchMetadata::on_jobDone()
{
     m_running = false;
}

void FetchMetadata::startProcess()
{
    Macaw::DEBUG("[FetchMetadata] Start the process of metadata fetching");
    if (!m_fetchMetadataQuery->isInitialized()) {
        Macaw::DEBUG("[FetchMetadata] FetchMetadataQuery not initialized, wait a little.");
        int l_maxTime = 5000;
        QTimer::singleShot(l_maxTime, this, SLOT(initTimerDone()));
        QEventLoop l_initWaitingLoop;
        connect(this, SIGNAL(exitInitWaitingLoop()),
                &l_initWaitingLoop, SLOT(quit()));
        l_initWaitingLoop.exec();
    } else {
        this->startMovieProcess();
        this->startPeopleProcess();
    }
}

void FetchMetadata::startMovieProcess()
{
    Macaw::DEBUG("[FetchMetadata] Start the process of metadata fetching for Movies");
    if (!m_fetchMetadataQuery->isInitialized()) {
        this->startProcess();

        return;
    }
    if (!m_movieQueue.isEmpty()) {
        //Showing a message in status bar on the processing status
        m_initialMovieQueueSize == 0 ? m_initialMovieQueueSize = m_movieQueue.size() : m_moviesProcessed++;
        ServicesManager::instance()->requestTempStatusBarMessage("Movies fetched: "+QString::number(m_moviesProcessed) + '/' +QString::number(m_initialMovieQueueSize));

        m_movie = m_movieQueue.takeFirst();

        connect(m_fetchMetadataQuery, SIGNAL(primaryResponse(QList<Movie>)),
                this, SLOT(processPrimaryResponse(QList<Movie>)));
        connect(m_fetchMetadataQuery, SIGNAL(networkError(QString)),
                this, SLOT(networkError(QString)));

        QString l_cleanedTitle = cleanString(m_movie.title());
        m_fetchMetadataQuery->sendPrimaryRequest(l_cleanedTitle);
    } else {
        // m_fetchMetadataQuery->deleteLater();
        ServicesManager::instance()->requestTempStatusBarMessage("Movies fetching completed! ", 10000);
        // id = 0 so we know that no movie is being processed
        m_movie = Movie();
    }
}

void FetchMetadata::startPeopleProcess()
{
    Macaw::DEBUG("[FetchMetadata] Start the process of metadata fetching for People");
    if (!m_fetchMetadataQuery->isInitialized()) {
        this->startProcess();

        return;
    }
    if (m_people.id() != 0) {
        Macaw::DEBUG("[FetchMetadata] A person is currently being processed." +QString::number(m_people.id()));

        return;
    }
    if (!m_peopleQueue.isEmpty()) {
        m_people = m_peopleQueue.takeFirst();
        connect(m_fetchMetadataQuery, SIGNAL(peopleResponse(People)),
                this, SLOT(processPeopleResponse(People)));

        Macaw::DEBUG(QString::number(m_people.id()));
        if (m_people.tmdbId() == 0 || m_people.id() == 0) {
            // We don't take in account people added by users
            this->startPeopleProcess();
        } else {
            m_fetchMetadataQuery->sendPeopleRequest(m_people.tmdbId());
        }
    } else {
        // m_fetchMetadataQuery->deleteLater();
        ServicesManager::instance()->requestTempStatusBarMessage("People fetching completed! ", 10000);
        // id = 0 so we know that no people is being processed
        m_people = People();
    }
}

void FetchMetadata::initTimerDone()
{
    Macaw::DEBUG("[FetchMetadata] Initialization timer is done");
    emit exitInitWaitingLoop();
    if (!m_fetchMetadataQuery->isInitialized()) {
        QMessageBox l_msgBox;
        l_msgBox.setIcon(QMessageBox::Critical);
        l_msgBox.setWindowTitle("Internet connection error");
        l_msgBox.setText("Initialization of the connection to TMDb failed");
        l_msgBox.setInformativeText("Please check your internet connection or try again later...");
        l_msgBox.setStandardButtons(QMessageBox::Abort|QMessageBox::Retry);
        int l_ret = l_msgBox.exec();

        if(l_ret == QMessageBox::Abort) {
            emit jobDone();
        } else if (l_ret == QMessageBox::Retry) {
            this->startProcess();
        }
    } else {
        this->startProcess();
    }
}

QString FetchMetadata::cleanString(QString title)
{
    QRegExp l_sep("(\\_|\\-|\\ |\\,|\\)|\\(|\\]|\\[|\\.|\\!|\\?|\\#)");
    QStringList l_splittedTitle = title.split(l_sep, QString::SkipEmptyParts);

    QRegExp l_alphaOnly("(^['À-Ÿà-ÿA-Za-z]*$)");
    l_splittedTitle = l_splittedTitle.filter(l_alphaOnly);

    return l_splittedTitle.join(" ");
}

void FetchMetadata::processPrimaryResponse(const QList<Movie> &movieList)
{
    Macaw::DEBUG("[FetchMetadata] Signal from primary request received");

    disconnect(m_fetchMetadataQuery, SIGNAL(primaryResponse(QList<Movie>)),
            this, SLOT(processPrimaryResponse(QList<Movie>)));

    QList<Movie> l_accurateList;

    if(movieList.count() == 1) {
        l_accurateList = movieList;
    } else if(movieList.count() > 1) {

        foreach(Movie l_movie, movieList) {

            if(cleanString(l_movie.title()).compare(cleanString(m_movie.title()), Qt::CaseInsensitive) == 0) {
                Macaw::DEBUG("[FetchMetadata] One title matches");
                l_accurateList.append(l_movie);
            }
        }
    }

    if(l_accurateList.count() == 1) {
        Movie l_movie = l_accurateList.at(0);

        connect(m_fetchMetadataQuery, SIGNAL(movieResponse(Movie)),
                this, SLOT(processMovieResponse(Movie)));
        Macaw::DEBUG("[FetchMetadata] Movie request to be sent ["+QString::number(l_movie.tmdbId())+"]");
        m_fetchMetadataQuery->sendMovieRequest(l_movie.tmdbId());
    } else if (m_askUser) {
        if(l_accurateList.isEmpty()) {
            l_accurateList = movieList;
        }
        this->openFetchMetadataDialog(m_movie, l_accurateList);
    } else {
        this->startMovieProcess();
    }
}

void FetchMetadata::processMovieResponse(const Movie &receivedMovie)
{
    Macaw::DEBUG("[FetchMetadata] Signal from movie request received");
    DatabaseManager *databaseManager = ServicesManager::instance()->databaseManager();

    disconnect(m_fetchMetadataQuery, SIGNAL(movieResponse(Movie)),
            this, SLOT(processMovieResponse(Movie)));

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
    m_movie.setPosterPath(receivedMovie.posterPath().right(receivedMovie.posterPath().size()-1));
    m_movie.setTmdbId(receivedMovie.tmdbId());

    m_movie.setImported(true);

    if (databaseManager->updateMovie(m_movie)) {
        m_movie = databaseManager->getOneMovieById(m_movie.id());

        Macaw::DEBUG("**** "+QString::number(m_movie.peopleList().count()));
        foreach (People p, m_movie.peopleList())
        {
            Macaw::DEBUG("**** "+QString::number(p.id()))   ;
        }

        // while updating the movie, the new people get id and the movie is reset to be aware of it.
        // So we can directly append them to the queue
        this->addPeopleToQueue(m_movie.peopleList());
    }

    emit updatedMovie();
    this->startMovieProcess();
}

void FetchMetadata::processPeopleResponse(const People &receivedPeople)
{
    Macaw::DEBUG("[FetchMetadata] Signal from movie request received");
    DatabaseManager *databaseManager = ServicesManager::instance()->databaseManager();

    disconnect(m_fetchMetadataQuery, SIGNAL(peopleResponse(People)),
            this, SLOT(processPeopleResponse(People)));

    m_people.setBiography(receivedPeople.biography());
    m_people.setBirthday(receivedPeople.birthday());
    m_people.setName(receivedPeople.name());
    m_people.setTmdbId(receivedPeople.tmdbId());

    m_people.setImported(true);

    databaseManager->updatePeople(m_people);
    emit updatedPeople();
    this->startPeopleProcess();
}

void FetchMetadata::on_searchCanceled()
{
    Macaw::DEBUG("[FetchMetadata] Dialog canceled");
    this->startProcess();
}

void FetchMetadata::on_selectedMovie(const Movie &movie)
{
    QList<Movie> l_movieList;
    l_movieList.append(movie);
    processPrimaryResponse(l_movieList);
}

void FetchMetadata::on_searchMovies(QString title)
{
    connect(m_fetchMetadataQuery, SIGNAL(primaryResponse(QList<Movie>)),
            this, SLOT(processPrimaryResponseDialog(QList<Movie>)));

    m_fetchMetadataQuery->sendPrimaryRequest(title);
}

void FetchMetadata::processPrimaryResponseDialog(const QList<Movie> &movieList)
{
    disconnect(m_fetchMetadataQuery, SIGNAL(primaryResponse(QList<Movie>)),
                this, SLOT(processPrimaryResponseDialog(QList<Movie>)));

    this->updateFetchMetadataDialog(movieList);
}

/**
 * @brief Construct and shows a FetchMetadataDialog to let the user to choose between a list of movies.
 * @param movie known by Macaw
 * @param accurateList of Movies proposed to the User
 */
void FetchMetadata::openFetchMetadataDialog(const Movie &movie, const QList<Movie> &accurateList)
{
    Macaw::DEBUG("[FetchMetadata] Enters openFetchMetadataDialog");

    m_fetchMetadataDialog = new FetchMetadataDialog(movie, accurateList);
    connect(m_fetchMetadataDialog, SIGNAL(selectedMovie(Movie)),
            this, SLOT(on_selectedMovie(Movie)));
    connect(m_fetchMetadataDialog, SIGNAL(searchMovies(QString)),
            this, SLOT(on_searchMovies(QString)));
    connect(m_fetchMetadataDialog, SIGNAL(searchCanceled()),
            this, SLOT(on_searchCanceled()));
    connect(m_fetchMetadataDialog, SIGNAL(dontAskUser()),
            this, SLOT(on_dontAskUser()));
    connect(m_fetchMetadataDialog, SIGNAL(neverAskUser(Movie)),
            this, SLOT(on_neverAskUser(Movie)));
    m_fetchMetadataDialog->show();
    Macaw::DEBUG("[FetchMetadata] Exits openFetchMetadataDialog");
}

/**
 * @brief Update the list of movies shown by FetchMetadataDialog
 * @param updatedList of the movies
 */
void FetchMetadata::updateFetchMetadataDialog(const QList<Movie> &updatedList)
{
    m_fetchMetadataDialog->setMovieList(updatedList);
}

void FetchMetadata::networkError(QString error)
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setText("Network error: " + error);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}

void FetchMetadata::on_dontAskUser()
{
    m_askUser = false;
}

void FetchMetadata::on_neverAskUser(Movie movie)
{
    DatabaseManager *databaseManager = ServicesManager::instance()->databaseManager();

    movie.setImported(true);
    databaseManager->updateMovie(movie);
}
