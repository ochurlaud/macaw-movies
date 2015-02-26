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
    m_app->debug("[FetchMetadataQuery] Constructor");

    m_networkManager = new QNetworkAccessManager;
    connect(this, SIGNAL(peopleResponse()),
            this, SLOT(on_peopleResponse()));
}

FetchMetadataQuery::~FetchMetadataQuery()
{
    delete m_networkManager;
    delete m_reply;
}

void FetchMetadataQuery::sendPrimaryRequest(QString title)
{
    connect(m_networkManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(on_primaryRequestResponse(QNetworkReply*)));

    QNetworkRequest l_request;

    l_request.setUrl(QUrl("http://api.themoviedb.org/3/search/movie?api_key="+ m_app->tmdbkey() +"&query="+ title));

    m_reply = m_networkManager->get(l_request);
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotError(QNetworkReply::NetworkError)));

    m_app->debug("[FetchMetadataQuery] Primary request sent");
}

void FetchMetadataQuery::sendMovieRequest(int tmdbID)
{
    connect(m_networkManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(on_movieRequestResponse(QNetworkReply*)));

    QNetworkRequest l_request;
    l_request.setUrl(QUrl("http://api.themoviedb.org/3/movie/"+QString::number(tmdbID)+"?api_key=" + m_app->tmdbkey() + "&append_to_response=credits&language=en"));

    m_reply = m_networkManager->get(l_request);
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(slotError(QNetworkReply::NetworkError)));

    m_app->debug("[FetchMetadataQuery] Movie Request sent");
}

void FetchMetadataQuery::sendPeopleRequest(int tmdbID)
{
    connect(m_networkManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(on_peopleRequestResponse(QNetworkReply*)));

    QNetworkRequest l_request;
    l_request.setUrl(QUrl("http://api.themoviedb.org/3/person/"+QString::number(tmdbID)+"/credits?api_key="+ m_app->tmdbkey()));

    m_reply = m_networkManager->get(l_request);
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(slotError(QNetworkReply::NetworkError)));

    m_app->debug("[FetchMetadataQuery] People request sent");
}

void FetchMetadataQuery::on_primaryRequestResponse(QNetworkReply* reply)
{
    m_app->debug("[FetchMetadataQuery] Primary Request response received");
    disconnect(m_networkManager, SIGNAL(finished(QNetworkReply*)),
               this, SLOT(on_primaryRequestResponse(QNetworkReply*)));

    QList<Movie> l_moviesPropositionList;

    QByteArray l_receivedData = reply->readAll();
    reply->deleteLater();
    QJsonDocument l_stream = QJsonDocument::fromJson(l_receivedData);

    if (!l_stream.isEmpty()) {
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

void FetchMetadataQuery::on_movieRequestResponse(QNetworkReply *reply)
{
    m_app->debug("[FetchMetadataQuery] Movie Request response received");
    disconnect(m_networkManager, SIGNAL(finished(QNetworkReply*)),
               this, SLOT(on_movieRequestResponse(QNetworkReply*)));

    QByteArray l_receivedData = reply->readAll();
    reply->deleteLater();
    QJsonDocument l_stream = QJsonDocument::fromJson(l_receivedData);

    if (!l_stream.isEmpty()) {
        QJsonObject l_jsonObject = l_stream.object();
        if (!l_jsonObject.isEmpty()) {
            m_movie.setId(l_jsonObject.value("id").toInt());
            m_movie.setTitle(l_jsonObject.value("title").toString());
            m_movie.setOriginalTitle(l_jsonObject.value("original_title").toString());
            m_movie.setCountry(l_jsonObject.value("production_countries").toArray().at(1).toObject().value("name").toString());

            QLocale locale(QLocale::English, QLocale::UnitedStates);
            QDate l_releaseDate = locale.toDate(l_jsonObject.value("release_date").toString(),"yyyy-MM-dd");
            m_movie.setReleaseDate(l_releaseDate);

            m_movie.setSynopsis(l_jsonObject.value("overview").toString());

            People l_people;
            int l_personId;

            QJsonArray l_jsonCastArray = l_jsonObject.value("credits").toObject().value("cast").toArray();
            for (int i = 0 ; i < l_jsonCastArray.size() ; i++) {
                l_personId = l_jsonCastArray.at(i).toObject().value("id").toInt();
                l_people.setId(l_personId);
                l_people.setType(People::Actor);
                m_movie.addPeople(l_people);
                m_peopleRequestList.append(l_personId);
            }

            emit(peopleResponse());

            QJsonArray l_jsonCrewArray = l_jsonObject.value("credits").toObject().value("crew").toArray();
            for (int i = 0 ; i < l_jsonCrewArray.size() ; i++) {
                QString l_job = l_jsonCrewArray.at(i).toObject().value("job").toString();
                if (l_job == "Director") {
                    l_personId = l_jsonCastArray.at(i).toObject().value("id").toInt();
                    l_people.setId(l_personId);
                    l_people.setType(People::Director);
                    m_movie.addPeople(l_people);
                    m_peopleRequestList.append(l_personId);
                } else if (l_job ==  "Producer") {
                    l_personId = l_jsonCastArray.at(i).toObject().value("id").toInt();
                    l_people.setId(l_personId);
                    l_people.setType(People::Producer);
                    m_movie.addPeople(l_people);
                    m_peopleRequestList.append(l_personId);
                }
            }
        } else {
            m_app->debug("[FetchMetadataQuery] Error");
        }
        // else: error!
    }
}

void FetchMetadataQuery::on_peopleRequestResponse(QNetworkReply *reply)
{
    m_app->debug("[FetchMetadataQuery] People Request response received");

    disconnect(m_networkManager, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(on_peopleRequestResponse(QNetworkReply*)));

    People l_people;

    QByteArray l_receivedData = reply->readAll();
    reply->deleteLater();
    emit(peopleResponse());

    QJsonDocument l_stream = QJsonDocument::fromJson(l_receivedData);
    if (!l_stream.isEmpty()) {
        QJsonObject l_jsonObject = l_stream.object();

        if (!l_jsonObject.isEmpty()) {
            int tmdbID = l_jsonObject.value("id").toInt();
            l_people.setLastname(l_jsonObject.value("name").toString());
            l_people.setBiography(l_jsonObject.value("biography").toString());

            QLocale locale(QLocale::English, QLocale::UnitedStates);
            QDate l_birthday = locale.toDate(l_jsonObject.value("birthday").toString(),"yyyy-MM-dd");
            l_people.setBirthday(l_birthday);
            for (int i = 0 ; i < m_movie.peopleList().count(); i++) {
                if (m_movie.peopleList().at(i).id() == tmdbID)
                {
                    l_people.setType(m_movie.peopleList().at(i).type());
                    m_movie.peopleList().replace(i, l_people);
                    // Do not break in case 1 person has 2 roles
                }
            }
        }
    }
    // If all request are done, the object is construct, send to FetchMetadata
    if (m_peopleRequestList.count() == 0) {
        // We send a signal to tell the movie is hydrated
        m_app->debug("[FetchMetadataQuery] Signal emitted to FetchMetadata for movie response");
        emit(movieResponse(m_movie));
    }
}

void FetchMetadataQuery::slotError(QNetworkReply::NetworkError error)
{
    m_app->debug("[FetchMetadataQuery] Error " + QString::number(error));
    emit(networkError(QString::number(error)));
}

void FetchMetadataQuery::on_peopleResponse()
{
    // If all people are fetched, send next request
    if (m_peopleRequestList.count() > 0) {
        sendPeopleRequest(m_peopleRequestList.takeFirst());
    }
}
