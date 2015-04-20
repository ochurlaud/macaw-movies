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
    Macaw::DEBUG("[FetchMetadataQuery] Constructor");
    m_initialized = false;
    m_networkManager = new QNetworkAccessManager;
    m_networkManager2 = new QNetworkAccessManager;

    this->sendInitRequest();
    connect(this, SIGNAL(peopleResponse()),
            this, SLOT(on_peopleResponse()));
}

FetchMetadataQuery::~FetchMetadataQuery()
{
    delete m_networkManager;
    delete m_networkManager2;
}

void FetchMetadataQuery::sendInitRequest()
{
    connect(m_networkManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(on_initRequestResponse(QNetworkReply*)));

    QNetworkRequest l_request;
    l_request.setUrl(QUrl("GEThttp://api.themoviedb.org/3/configuration"
                          "?api_key="+ m_app->tmdbkey()
                          , QUrl::TolerantMode));
    m_networkManager->get(l_request);

    Macaw::DEBUG("[FetchMetadataQuery] Init request sent");
}

void FetchMetadataQuery::sendPrimaryRequest(QString title)
{
    connect(m_networkManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(on_primaryRequestResponse(QNetworkReply*)));

    QNetworkRequest l_request;
    l_request.setUrl(QUrl("http://api.themoviedb.org/3/search/movie"
                          "?api_key="+ m_app->tmdbkey() +"&query="+ title
                          , QUrl::TolerantMode));
    m_networkManager->get(l_request);

    Macaw::DEBUG("[FetchMetadataQuery] Primary request sent");
}

void FetchMetadataQuery::sendMovieRequest(int tmdbID)
{
    connect(m_networkManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(on_movieRequestResponse(QNetworkReply*)));

    QNetworkRequest l_request;
    l_request.setUrl(QUrl("http://api.themoviedb.org/3/movie/" +
                          QString::number(tmdbID) +
                          "?api_key=" + m_app->tmdbkey() +
                          "&append_to_response=credits&language=en"
                          , QUrl::TolerantMode));
    m_networkManager->get(l_request);

    Macaw::DEBUG("[FetchMetadataQuery] Movie Request sent");
}

void FetchMetadataQuery::sendPeopleRequest(int tmdbID)
{
    connect(m_networkManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(on_peopleRequestResponse(QNetworkReply*)));

    QNetworkRequest l_request;
    l_request.setUrl(QUrl("http://api.themoviedb.org/3/person/" +
                          QString::number(tmdbID) +
                          "?api_key="+ m_app->tmdbkey()
                          , QUrl::StrictMode));
    m_networkManager->get(l_request);

    Macaw::DEBUG("[FetchMetadataQuery] People request sent, id="+QString::number(tmdbID));
}

void FetchMetadataQuery::sendPosterRequest(QString poster_path) {
    connect(m_networkManager2, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(on_posterRequestResponse(QNetworkReply*)));

    QNetworkRequest l_request;
    l_request.setUrl(QUrl(m_posterUrl+ "/w396" + poster_path
                          , QUrl::StrictMode));
    m_networkManager2->get(l_request);

    Macaw::DEBUG("[FetchMetadataQuery] Poster request sent");
}

void FetchMetadataQuery::on_initRequestResponse(QNetworkReply* reply)
{
    Macaw::DEBUG("[FetchMetadataQuery] Init Request response received");
    disconnect(m_networkManager, SIGNAL(finished(QNetworkReply*)),
               this, SLOT(on_initRequestResponse(QNetworkReply*)));

    QByteArray l_receivedData = reply->readAll();
    reply->deleteLater();
    QJsonDocument l_stream = QJsonDocument::fromJson(l_receivedData);

    if (!l_stream.isEmpty()) {
        QJsonObject l_jsonObject = l_stream.object();
        if (!l_jsonObject.isEmpty()) {
            QJsonObject l_imagesObject = l_jsonObject.value("images").toObject();
            m_posterUrl = l_imagesObject.value("secure_base_url").toString();
        }
    }
    m_initialized = true;
}

void FetchMetadataQuery::on_primaryRequestResponse(QNetworkReply* reply)
{
    Macaw::DEBUG("[FetchMetadataQuery] Primary Request response received");
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

        Macaw::DEBUG("[FetchMetadataQuery] "+QString::number(l_numberMovies) + " Movie(s) found");

        for (int i = 0 ; i < l_jsonResults.size() ; i++)
        {
            QJsonObject l_currentObject = l_jsonResults.at(i).toObject();
            Movie l_movieProposition;
            l_movieProposition.setId(l_currentObject.value("id").toInt());
            l_movieProposition.setTitle(l_currentObject.value("title").toString());
            l_movieProposition.setReleaseDate(QDate::fromString(l_currentObject.value("release_date").toString(),"yyyy-MM-dd"));
            l_moviesPropositionList.append(l_movieProposition);
        }
        Macaw::DEBUG("[FetchMetadataQuery] Signal to be emitted to FetchMetadata for primary request");
        emit primaryResponse(l_moviesPropositionList);
    } else {
        Macaw::DEBUG("[FetchMetadataQuery] Error in on_primaryRequestResponse, stream empty !");
        // error !!
    }
}

void FetchMetadataQuery::on_movieRequestResponse(QNetworkReply *reply)
{
    Macaw::DEBUG("[FetchMetadataQuery] Movie Request response received");
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
            m_movie.setPosterPath(l_jsonObject.value("poster_path").toString());

            QLocale locale(QLocale::English, QLocale::UnitedStates);
            QDate l_releaseDate = locale.toDate(l_jsonObject.value("release_date").toString(),"yyyy-MM-dd");
            m_movie.setReleaseDate(l_releaseDate);

            m_movie.setSynopsis(l_jsonObject.value("overview").toString());

            sendPosterRequest(l_jsonObject.value("poster_path").toString());

            People l_people;
            QString l_personName;
            int l_personId;
            QJsonArray l_jsonCastArray = l_jsonObject.value("credits").toObject().value("cast").toArray();
            for (int i = 0 ; i < l_jsonCastArray.size() ; i++) {
                l_personId = l_jsonCastArray.at(i).toObject().value("id").toInt();
                l_personName = l_jsonCastArray.at(i).toObject().value("name").toString();
                l_people.setId(l_personId);
                l_people.setType(People::Actor);
                m_movie.addPeople(l_people);
                m_peopleRequestQueue.append(l_personId);
                Macaw::DEBUG("[FetchMetadataQuery] new Actor: "+ l_personName);
            }

            QJsonArray l_jsonCrewArray = l_jsonObject.value("credits").toObject().value("crew").toArray();
            for (int i = 0 ; i < l_jsonCrewArray.size() ; i++) {
                QString l_job = l_jsonCrewArray.at(i).toObject().value("job").toString();
                if (l_job == "Director") {
                    l_personId = l_jsonCrewArray.at(i).toObject().value("id").toInt();
                    l_personName = l_jsonCrewArray.at(i).toObject().value("name").toString();
                    l_people.setId(l_personId);
                    l_people.setType(People::Director);
                    m_movie.addPeople(l_people);
                    m_peopleRequestQueue.append(l_personId);
                    Macaw::DEBUG("[FetchMetadataQuery] new Director: "+ l_personName);
                } else if (l_job ==  "Producer") {
                    l_personId = l_jsonCrewArray.at(i).toObject().value("id").toInt();
                    l_personName = l_jsonCrewArray.at(i).toObject().value("name").toString();
                    l_people.setId(l_personId);
                    l_people.setType(People::Producer);
                    m_movie.addPeople(l_people);
                    m_peopleRequestQueue.append(l_personId);
                    Macaw::DEBUG("[FetchMetadataQuery] new Producer: "+ l_personName);
                }
            }
        } else {
            Macaw::DEBUG("[FetchMetadataQuery] Error on MovieRequestResponse: l_jsonObject empty");
        }
    }
    if(m_movie.peopleList().isEmpty()) {
        Macaw::DEBUG("[FetchMetadataQuery] No people: send movieResponse Signal");
        emit(movieResponse(m_movie));
    } else {
        Macaw::DEBUG("[FetchMetadataQuery] Send peopleResponse Signal");
        emit(peopleResponse());
    }
}

void FetchMetadataQuery::on_peopleRequestResponse(QNetworkReply *reply)
{
    Macaw::DEBUG("[FetchMetadataQuery] People Request response received");

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
            int l_tmdbID = l_jsonObject.value("id").toInt();
            Macaw::DEBUG("[FetchMetadataQuery] Processing response for id=" +QString::number(l_tmdbID));
            l_people.setName(l_jsonObject.value("name").toString());
            l_people.setBiography(l_jsonObject.value("biography").toString());
            QLocale locale(QLocale::English, QLocale::UnitedStates);
            QDate l_birthday = locale.toDate(l_jsonObject.value("birthday").toString(),"yyyy-MM-dd");
            l_people.setBirthday(l_birthday);

            QList<People> l_peopleList = m_movie.peopleList();
            for (int i = 0 ; i < m_movie.peopleList().count(); i++) {
                if (m_movie.peopleList().at(i).id() == l_tmdbID)
                {
                    l_people.setType(l_peopleList.at(i).type());
                    l_peopleList.replace(i, l_people);
                    // Do not break in case 1 person has 2 roles
                }
            }
            m_movie.setPeopleList(l_peopleList);
            // Processing done, id removed from the list
            m_peopleRequestProcessing.removeOne(l_tmdbID);
        }
    }    
    // If all request are done, the object is fully constructed => send to FetchMetadata
    if (m_peopleRequestProcessing.isEmpty() && m_peopleRequestQueue.isEmpty()) {
        // We send a signal to tell the movie is hydrated
        emit(movieResponse(m_movie));
    }
}

void FetchMetadataQuery::on_posterRequestResponse(QNetworkReply *reply) {
    Macaw::DEBUG("[FetchMetadataQuery] Poster Request response received");

    disconnect(m_networkManager2, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(on_posterRequestResponse(QNetworkReply*)));


    QString l_postersPath = qApp->property("postersPath").toString();
    QFile l_posterFile(l_postersPath + reply->url().fileName());
    if (!l_posterFile.open(QIODevice::WriteOnly)) {
        Macaw::DEBUG("[FetchMetadataQuery] Error opening/creating: " + l_postersPath + reply->url().fileName());
    }

    QByteArray l_receivedData = reply->readAll();
    Macaw::DEBUG("[FetchMetadataQuery] read: "+QString::number(l_receivedData.size()));
    reply->deleteLater();

    l_posterFile.write(l_receivedData);
    l_posterFile.close();
}

void FetchMetadataQuery::slotError(QNetworkReply::NetworkError error)
{
    Macaw::DEBUG("[FetchMetadataQuery] Error " + QString::number(error));
    emit(networkError(QString::number(error)));
}

void FetchMetadataQuery::on_peopleResponse()
{
    // If all people are fetched, send next request
    if (!m_peopleRequestQueue.isEmpty()) {
        int l_peopleToProcess = m_peopleRequestQueue.takeFirst();
        m_peopleRequestProcessing.append(l_peopleToProcess);
        this->sendPeopleRequest(l_peopleToProcess);
    }
}
