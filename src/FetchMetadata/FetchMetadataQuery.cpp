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

#include "FetchMetadata/FetchMetadataQuery.h"

FetchMetadataQuery::FetchMetadataQuery(QObject *parent) :
    QObject(parent)
{
    m_app = qobject_cast<Application *>(qApp);
    m_networkManager = new QNetworkAccessManager;
}

FetchMetadataQuery::~FetchMetadataQuery()
{

}

void FetchMetadataQuery::sendPrimaryRequest(QString title)
{
    connect(m_networkManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(on_primaryRequestResponse(QNetworkReply*)));

    QNetworkRequest l_request;

    l_request.setUrl(QUrl("http://api.themoviedb.org/3/search/movie?api_key="+ m_app->tmdbkey() +"&query="+title));
    m_networkManager->get(l_request);

    m_app->debug("[FetchMetadataQuery] Primary request sent");

    /*connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotError(QNetworkReply::NetworkError)));
    connect(m_reply, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(slotSslErrors(QList<QSslError>)));*/

}

void FetchMetadataQuery::on_primaryRequestResponse(QNetworkReply *reply)
{
    m_app->debug("[FetchMetadataQuery] Primary Request response received");
    disconnect(m_networkManager, SIGNAL(finished(QNetworkReply*)),
               this, SLOT(on_primaryRequestResponse(QNetworkReply*)));

    QList<Movie> l_moviesPropositionList;

    QByteArray l_receivedData = reply->readAll();
    QJsonDocument l_stream = QJsonDocument::fromJson(l_receivedData);

    if (!l_stream.isEmpty())
    {
        QJsonObject l_jsonObject = l_stream.object();
        int l_numberMovies = l_jsonObject.value("total_results").toInt();

        QJsonArray l_jsonResults = l_jsonObject.value("results").toArray();

        m_app->debug("[FetchMetadataQuery] "+QString::number(l_numberMovies) + " Movie(s) found");

        for (int i = 0 ; i < l_jsonResults.size() ; i++)
        {
            QJsonObject l_currentObject = l_jsonResults.at(i).toObject();
            Movie l_movieProposition;
            l_movieProposition.setId(l_currentObject.value("id").toInt());
            l_movieProposition.setTitle(l_currentObject.value("title").toString());
            l_movieProposition.setReleaseDate(QDate::fromString(l_currentObject.value("release_date").toString(),"yyyy-MM-dd"));
            l_moviesPropositionList.append(l_movieProposition);
        }
        m_app->debug("[FetchMetadataQuery] Signal to be emitted to FetchMetadata for primary request");
        emit(primaryResponse(l_moviesPropositionList));

    } else {
        // error !!
    }
}

void FetchMetadataQuery::sendFinalRequest(int tmdbID)
{
    connect(m_networkManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(on_finalRequestResponse(QNetworkReply*)));

    QNetworkRequest l_request;
    l_request.setUrl(QUrl("http://api.themoviedb.org/3/movie/"+QString::number(tmdbID)+"?api_key=" + m_app->tmdbkey() + "&language=en"));

    m_networkManager->get(l_request);

    m_app->debug("[FetchMetadataQuery] Final Request response sent");
}


void FetchMetadataQuery::on_finalRequestResponse(QNetworkReply *reply)
{
    m_app->debug("[FetchMetadataQuery] Final Request response received");
    disconnect(m_networkManager, SIGNAL(finished(QNetworkReply*)),
               this, SLOT(on_primaryRequestResponse(QNetworkReply*)));

    Movie l_movie;
    QByteArray l_receivedData = reply->readAll();
    QJsonDocument l_stream = QJsonDocument::fromJson(l_receivedData);

    if (!l_stream.isEmpty())
    {
        QJsonObject l_jsonObject = l_stream.object();

        if (!l_jsonObject.isEmpty())
        {
            l_movie.setTitle(l_jsonObject.value("title").toString());
            l_movie.setOriginalTitle(l_jsonObject.value("original_title").toString());
            l_movie.setCountry(l_jsonObject.value("production_countries").toArray().at(1).toObject().value("name").toString());
            QLocale locale(QLocale::English, QLocale::UnitedStates);
            QDate l_releaseDate = locale.toDate(l_jsonObject.value("release_date").toString(),"yyyy-MM-dd");

            l_movie.setReleaseDate(l_releaseDate);

            l_movie.setSynopsis(l_jsonObject.value("overview").toString());

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
            emit(finalResponse(l_movie));
            m_app->debug("[FetchMetadataQuery] Signal emit to FetchMetadata for final response");
        } else {
            m_app->debug("[FetchMetadataQuery] Error");
        }
        // else: error!
    }

}
