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

#include "MetadataFetcher.h"
#include <QLocale>

MetadataFetcher::MetadataFetcher(Movie movie, QObject *parent) :
    QObject(parent)
{
    m_app = qobject_cast<Application *>(qApp);
    m_app->debug("[MetadataFetcher] Constructor");
    m_movie = movie;
    m_networkManager = new QNetworkAccessManager(this);
    m_key = loadKey(); // The key is key.txt at the root of the executable
    m_app->debug("[MetadataFetcher] Construction done");
}

QString MetadataFetcher::loadKey()
{
    QFile keyfile("key.txt");
    if (!keyfile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        m_app->debug("[ERROR] A key.txt file should be near the executable !");
        QDialog *l_errorDialog = new QDialog();
        QLabel *l_msg = new QLabel(l_errorDialog);
        l_msg->setText("[ERROR] A key.txt file should be near the executable !");
        l_errorDialog->show();
        l_errorDialog->setModal(true);
        l_errorDialog->setWindowTitle("ERROR");
        l_errorDialog->resize(QSize(600,50));
        return "0";
    }

    QTextStream in(&keyfile);

    return in.readLine();
}

void MetadataFetcher::getRelatedMovies(QString title)
{
    m_app->debug("[MetadataFetcher] Enter getRelatedMovie");
    connect(m_networkManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyRelatedMovies(QNetworkReply*)));

    QRegExp l_sep("(\\_|\\-|\\ |\\,|\\.|\\!|\\?)");
    QStringList l_splittedTitle = title.split(l_sep);

    QRegExp l_alphaOnly("(^[À-Ÿà-ÿA-Za-z]*$)");
    l_splittedTitle = l_splittedTitle.filter(l_alphaOnly);
    QString l_filteredTitle = l_splittedTitle.join("%20");
    QNetworkRequest l_request;

    l_request.setUrl(QUrl("http://api.themoviedb.org/3/search/movie?api_key="+m_key+"&query="+l_filteredTitle));
    m_networkManager->get(l_request);

    /*connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotError(QNetworkReply::NetworkError)));
    connect(m_reply, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(slotSslErrors(QList<QSslError>)));*/
    m_app->debug("[MetadataFetcher] Exit getRelatedMovie");
}

void MetadataFetcher::replyRelatedMovies(QNetworkReply *reply)
{
    m_app->debug("[MetadataFetcher] Enter replyRelatedMovie");
    disconnect(m_networkManager, SIGNAL(finished(QNetworkReply*)),
               this, SLOT(replyRelatedMovies(QNetworkReply*)));
    QByteArray l_receivedData = reply->readAll();
    QJsonDocument l_stream = QJsonDocument::fromJson(l_receivedData);

    if (!l_stream.isEmpty())
    {
        QJsonObject l_jsonObject = l_stream.object();
        int l_numberMovies = l_jsonObject.value("total_results").toInt();

        QJsonArray l_jsonResults = l_jsonObject.value("results").toArray();

        if (l_numberMovies == 0)
        {
            m_app->debug("0 Movie found");
            emit(noMovieFound());
        }
        else if (l_numberMovies == 1)
        {
            m_app->debug("1 Movie found, metadata being updated");
            int l_id = l_jsonResults.at(0).toObject().value("id").toInt();
            this->getMetadata(l_id);
        }
        else
        {
            m_app->debug("[MetadataFetcher] replyRelatedMovies: "
                         + QString::number(l_numberMovies)
                         + " movies found");

            QList<Movie> l_moviesPropositionList;

            for (int i = 0 ; i < l_jsonResults.size() ; i++)
            {
                QJsonObject l_currentObject = l_jsonResults.at(i).toObject();
                Movie l_movieProposition;
                l_movieProposition.setId(l_currentObject.value("id").toInt());
                l_movieProposition.setTitle(l_currentObject.value("title").toString());
                l_movieProposition.setReleaseDate(QDate::fromString(l_currentObject.value("release_date").toString(),"yyyy-MM-dd"));
                l_moviesPropositionList.append(l_movieProposition);
            }

            MetadataFetcherDialog *l_selectWindow = new MetadataFetcherDialog(m_movie, l_moviesPropositionList);
            l_selectWindow->show();
            connect(l_selectWindow, SIGNAL(selectedMovie(int)), this, SLOT(callGetMetadata(int)));
        }
    }
    m_app->debug("[MetadataFetcher] Exit replyRelatedMovie");
}

void MetadataFetcher::getMetadata(int tmdbID)
{
    m_app->debug("[MetadataFetcher] Enter getMetadata, " + QString::number(tmdbID));
    connect(m_networkManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyHydrateMovie(QNetworkReply*)));
    QNetworkRequest l_request;
    l_request.setUrl(QUrl("http://api.themoviedb.org/3/movie/"+QString::number(tmdbID)+"?api_key="+m_key+"&language=en"));

    m_networkManager->get(l_request);
    m_app->debug("[MetadataFetcher] Exit getMetadata");
}

void MetadataFetcher::replyHydrateMovie(QNetworkReply *reply)
{
    m_app->debug("[MetadataFetcher] Enter replyHydrateMovie");
    QByteArray l_receivedData = reply->readAll();
    QJsonDocument l_stream = QJsonDocument::fromJson(l_receivedData);

    if (!l_stream.isEmpty())
    {
        QJsonObject l_jsonObject = l_stream.object();

        if (!l_jsonObject.isEmpty())
        {
            m_movie.setTitle(l_jsonObject.value("title").toString());
            m_movie.setOriginalTitle(l_jsonObject.value("original_title").toString());
            m_movie.setCountry(l_jsonObject.value("production_countries").toArray().at(1).toObject().value("name").toString());
            QLocale locale(QLocale::English, QLocale::UnitedStates);
            QDate l_releaseDate = locale.toDate(l_jsonObject.value("release_date").toString(),"yyyy-MM-dd");

            m_movie.setReleaseDate(l_releaseDate);

            m_movie.setSynopsis(l_jsonObject.value("overview").toString());

            // Typical Answer:
            /*
             * {
             *      "adult":false,
             *      "backdrop_path":"/uK831vMfGfvZIAmam29IozN0XDe.jpg",
             *      "belongs_to_collection": {
             *              "id":239766,
             *              "name":"The Klapisch Trilogy",
             *              "poster_path":"/woD0gFHbZgwi9RpLL0uSQ56hnJA.jpg",
             *              "backdrop_path":"/3ahNzWAv20FlJ0i2wrrgKPOjQCT.jpg"
             *          },
             *      "budget":0,
             *      "genres":   [
             *              {
             *                  "id":35,
             *                  "name":"Comedy"
             *              },
             *              {
             *                  "id":18,
             *                  "name":"Drama"},
             *              {
             *                  "id":10749,
             *                  "name":"Romance"
             *              }
             *          ],
             *      "homepage":"",
             *      "id":1555,
             *      "imdb_id":"tt0283900",
             *      "original_language":"en",
             *      "original_title":"L'Auberge Espagnole",
             *      "overview":"A strait-laced French.... of love and friendship.",
             *      "popularity":0.407367422824576,
             *      "poster_path":"/j8WacqRgpvsWU9oFSi0nfJf1pWt.jpg",
             *      "production_companies": [
             *              {
             *                  "name":"Mate Producciones S.A.",
             *                  "id":732
             *              }
             *          ],
             *      "production_countries":  [
             *              {
             *                  "iso_3166_1":"FR",
             *                  "name":"France"
             *              }
             *          ],
             *      "release_date":"2002-05-17",
             *      "revenue":0,
             *      "runtime":122,
             *      "spoken_languages": [
             *              {
             *                  "iso_639_1":"fr",
             *                  "name":"Français"
             *              }
             *          ],
             *      "status":"Released",
             *      "tagline":"",
             *      "title":"The Spanish Apartment",
             *      "video":false,
             *      "vote_average":6.8,
             *      "vote_count":49
             *  }
             */

            // For people: /movie/{id}/credits
            // for cover:  /movie/{id}/images

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
    connect(this, SIGNAL(movieHydrated(Movie&)),
            this, SLOT(updateMovieInDatabase(Movie&)));

    QEventLoop l_loop;
    connect(this, SIGNAL(movieHydrated(Movie&)),
            &l_loop, SLOT(quit()));
    connect(this, SIGNAL(noMovieFound()),
            &l_loop, SLOT(quit()));

    l_loop.exec();
    m_app->debug("[MetadataFetcher] Exit fetchMetadata");
}

bool MetadataFetcher::updateMovieInDatabase(Movie &movie)
{
    m_app->debug("[MetadataFetcher] Enter updateMovieInDatabase");
    return m_app->getDatabaseManager()->updateMovie(movie);
}

void MetadataFetcher::callGetMetadata(int tmdbID)
{
    m_app->debug("[MetadataFetcher] callGetMetada");

    this->getMetadata(tmdbID);
}
