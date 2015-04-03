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

#ifndef FETCHMETADATAQUERY_H
#define FETCHMETADATAQUERY_H

#include <QtNetwork>
#include <QWidget>

#include "Application.h"
#include "Entities/Movie.h"

class Application;

class FetchMetadataQuery : public QObject
{
    Q_OBJECT

public:
    FetchMetadataQuery(QObject *parent = 0);
    ~FetchMetadataQuery();
    void sendPrimaryRequest(QString title);
    void sendMovieRequest(int tmdbID);
    void sendPeopleRequest(int tmdbID);

signals:
    void primaryResponse(QList<Movie>&);
    void movieResponse(Movie&);
    void networkError(QString);
    void peopleResponse();

private slots:
    void on_primaryRequestResponse(QNetworkReply *reply);
    void on_movieRequestResponse(QNetworkReply *reply);
    void on_peopleRequestResponse(QNetworkReply *reply);
    void on_peopleResponse();
    void slotError(QNetworkReply::NetworkError error);

private:
    QNetworkAccessManager *m_networkManager;
    Application *m_app;
    Movie m_movie;
    QList<int> m_peopleRequestList;

};

#endif // FETCHMETADATAQUERY_H
