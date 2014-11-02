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

#include "MetadataFetcher.h"
#include <QLocale>

MetadataFetcher::MetadataFetcher(Movie movie, QObject *parent) :
    QObject(parent)
{
    m_app->debug("[MetadataFetcher] Constructor");
    m_app = qobject_cast<Application *>(qApp);
    m_movie = movie;
    m_networkManager = new QNetworkAccessManager(this);
    m_app->debug("[MetadataFetcher] Construction done");
}

void MetadataFetcher::getRelatedMovies(QString title)
{
    m_app->debug("[MetadataFetcher] Enter getRelatedMovie");
    connect(m_networkManager, SIGNAL(finished(QNetworkReply*)),this, SLOT(replyRelatedMovies(QNetworkReply*)));

    QNetworkRequest l_request;
    l_request.setUrl(QUrl("http://www.omdbapi.com/?s="+title+"&r=XML"));
    m_networkManager->get(l_request);

    /*connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotError(QNetworkReply::NetworkError)));
    connect(m_reply, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(slotSslErrors(QList<QSslError>)));*/
    m_app->debug("[MetadataFetcher] Exit getRelatedMovie");
}

void MetadataFetcher::replyRelatedMovies(QNetworkReply *reply)
{
    m_app->debug("[MetadataFetcher] Enter replyRelatedMovie");
    disconnect(m_networkManager, SIGNAL(finished(QNetworkReply*)),this, SLOT(replyRelatedMovies(QNetworkReply*)));
    QByteArray l_receivedData = reply->readAll();

    QDomDocument l_stream;
    l_stream.setContent(l_receivedData);

    QDomElement l_root;
    QDomNode l_node;
    QDomElement l_element;

    l_root = l_stream.documentElement();
    l_node = l_root.firstChild();

    int l_moviesNumber = l_root.childNodes().count();

    // If l_moviesNumber = 1, we can directly edit the Metadata
    // Else we ask the user to choose
    if (l_moviesNumber == 1)
    {
        l_element = l_node.toElement();
        if (l_element.tagName() == "Movie")
        {
            QString l_imdbID = l_element.attribute("imdbID");
            this->getMetadata(l_imdbID);
        }
        // else: error!
    }
    else
    {
        QStringList l_printableStrings;
        QStringList l_imdbIDs;
        m_app->debug("[MetadataFetcher] replyRelatedMovies: "
                     + QString::number(l_moviesNumber)
                     + " movies found");
        while(!l_node.isNull())
        {
            l_element = l_node.toElement();
            if (l_element.tagName() == "Movie")
            {
                l_printableStrings << l_element.attribute("Title")
                             + " [" + l_element.attribute("Year") + "] "
                             + " - " + l_element.attribute("type");
                l_imdbIDs << l_element.attribute("imdbID");
            }
            l_node = l_node.nextSibling();
        }
        QDialog *l_selectWindow;
        QListWidget *l_listWidget;
        l_listWidget->setParent(l_selectWindow);
        l_selectWindow->show();

    }
    m_app->debug("[MetadataFetcher] Exit replyRelatedMovie");
}

void MetadataFetcher::getMetadata(QString imdbID)
{
    m_app->debug("[MetadataFetcher] Enter getMetadata, " + imdbID);
    connect(m_networkManager, SIGNAL(finished(QNetworkReply*)),this, SLOT(replyHydrateMovie(QNetworkReply*)));
    QNetworkRequest l_request;
    l_request.setUrl(QUrl("http://www.omdbapi.com/?i="+imdbID+"&plot=full&r=XML&tomatoes=true"));

    m_networkManager->get(l_request);
    m_app->debug("[MetadataFetcher] Exit getMetadata");
}

void MetadataFetcher::replyHydrateMovie(QNetworkReply *reply)
{
    m_app->debug("[MetadataFetcher] Enter replyHydrateMovie");
    QByteArray l_receivedData = reply->readAll();

    QDomDocument l_stream;
    l_stream.setContent(l_receivedData);

    QDomElement l_root;
    QDomNode l_node;
    QDomElement l_element;

    l_root = l_stream.documentElement();
    l_node = l_root.firstChild();

    int l_moviesNumber = l_root.childNodes().count();

    // If l_moviesNumber = 1, we can directly edit the Metadata
    // Else we ask the user to choose
    if (l_moviesNumber == 1)
    {
        l_element = l_node.toElement();

        if (l_element.tagName() == "movie")
        {
            m_movie.setTitle(l_element.attribute("title"));
            m_movie.setOriginalTitle(l_element.attribute("title"));
            m_movie.setCountry(l_element.attribute("country"));
            QLocale locale(QLocale::English, QLocale::UnitedStates);
            QDate l_releaseDate = locale.toDate(l_element.attribute("released"), "dd MMM yyyy");
            m_movie.setReleaseDate(l_releaseDate);

            m_movie.setSynopsis(l_element.attribute("plot"));

            // Typical Answer:
            /*
             *<root response="True">
             * <movie
             *    title="Star Wars: Episode I - The Phantom Menace"
             *    year="1999"
             *    rated="PG"
             *    released="19 May 1999"
             *    runtime="136 min"
             *    genre="Action, Adventure, Fantasy"
             *    director="George Lucas"
             *    writer="George Lucas"
             *    actors="Liam Neeson, Ewan McGregor, Natalie Portman, Jake Lloyd"
             *    plot="The evil Trade Federation, led by Nute Gunray is planning to take over the peaceful
             *  world of Naboo. Jedi Knights Qui-Gon Jinn and Obi-Wan Kenobi are sent to confront the leaders. But not
             *  everything goes to plan. The two Jedi escape, and along with their new Gungan friend, Jar Jar Binks head
             *  to Naboo to warn Queen Amidala, but droids have already started to capture Naboo and the Queen is not
             *  safe there. Eventually, they land on Tatooine, where they become friends with a young boy known as
             *  Anakin Skywalker. Qui-Gon is curious about the boy, and sees a bright future for him. The group must
             *  now find a way of getting to Coruscant and to finally solve this trade dispute, but there is someone
             *  else hiding in the shadows. Are the Sith really extinct? Is the Queen really who she says she is? And
             *  what's so special about this young boy?"
             *    language="English"
             *    country="USA"
             *    awards="Nominated for 3 Oscars. Another 17 wins & 57 nominations."
             *    poster="http://ia.media-imdb.com/images/M/MV5BMTQ4NjEwNDA2Nl5BMl5BanBnXkFtZTcwNDUyNDQzNw@@._V1_SX300.jpg"
             *    metascore="51"
             *    imdbRating="6.6"
             *    imdbVotes="402,037"
             *    imdbID="tt0120915"
             *    type="movie"
             *    tomatoMeter="N/A"
             *    tomatoImage="N/A"
             *    tomatoRating="N/A"
             *    tomatoReviews="N/A"
             *    tomatoFresh="N/A"
             *    tomatoRotten="N/A"
             *    tomatoConsensus="N/A"
             *    tomatoUserMeter="24"
             *    tomatoUserRating="N/A"
             *    tomatoUserReviews="180"
             *    DVD="14 May 2002"
             *    BoxOffice="N/A"
             *    Production="N/A"
             *    Website="N/A"
             * />
             *</root>
             */

            // We send a signal to tell the movie is hydrated
            emit(movieHydrated(m_movie));
        }
        // else: error!
    }
    m_app->debug("[MetadataFetcher] Exit replyHydrateMovie");
}


void MetadataFetcher::fetchMetadata(QString title)
{
    m_app->debug("[MetadataFetcher] Enter fetchMetadata, " + title);
    this->getRelatedMovies(title);
    connect(this, SIGNAL(movieHydrated(Movie&)), this, SLOT(updateMovieInDatabase(Movie&)));
    m_app->debug("[MetadataFetcher] Exit fetchMetadata");
}

bool MetadataFetcher::updateMovieInDatabase(Movie &movie)
{
    m_app->debug("[MetadataFetcher] Enter updateMovieInDatabase");
    return m_app->getDatabaseManager()->updateMovie(movie);
}
