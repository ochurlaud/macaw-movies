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
    l_splittedTitle = l_splittedTitle.filter(l_alphaOnly); QNetworkRequest l_request;

    l_request.setUrl(QUrl("http://api.themoviedb.org/3/search/movie?api_key="+m_key+"&query="+title));
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

        if (l_numberMovies == 1)
        {
            QString l_id = l_jsonResults.at(0).toObject().value("id").toString();
            this->getMetadata(l_id);
        }

        else
        {
            QStringList l_printableStrings;
            QStringList l_tmdbIDs;
            m_app->debug("[MetadataFetcher] replyRelatedMovies: "
                         + QString::number(l_numberMovies)
                         + " movies found");
            for (int i = 0 ; i < l_jsonResults.size() ; i++)
            {
                QJsonObject l_currentObject = l_jsonResults.at(i).toObject();
                l_printableStrings << l_currentObject.value("title").toString()
                                 + " [" + l_currentObject.value("release_date").toString() + "]";
                l_tmdbIDs << QString::number(l_currentObject.value("id").toInt());
            }

            // This should go in another class
            QDialog *l_selectWindow = new QDialog;
            QListWidget *l_listWidget = new QListWidget(l_selectWindow);
            for (int i = 0 ; i < l_printableStrings.size() ; i++)
            {
                QListWidgetItem *l_item = new QListWidgetItem(l_printableStrings.at(i));
                l_item->setData(Qt::UserRole, l_tmdbIDs.at(i));
                l_listWidget->addItem(l_item);
            }
            connect(l_listWidget,SIGNAL(doubleClicked(QModelIndex)),
                    this, SLOT(on_doubleClickedMovie(QModelIndex)));
            l_selectWindow->show();

        }
    }
    m_app->debug("[MetadataFetcher] Exit replyRelatedMovie");
}

void MetadataFetcher::getMetadata(QString tmdbID)
{
    m_app->debug("[MetadataFetcher] Enter getMetadata, " + tmdbID);
    connect(m_networkManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyHydrateMovie(QNetworkReply*)));
    QNetworkRequest l_request;
    l_request.setUrl(QUrl("http://api.themoviedb.org/3/movie/"+tmdbID+"?api_key="+m_key+"&language=en"));

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

void MetadataFetcher::on_doubleClickedMovie(QModelIndex index)
{
    m_app->debug("[MetadataFetcher] Movie double-clicked");
    QString l_tmdbID = index.data(Qt::UserRole).toString();
    this->getMetadata(l_tmdbID);
}
