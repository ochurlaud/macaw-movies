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

#ifndef METADATAFETCHER_H
#define METADATAFETCHER_H

#include <QObject>
#include <QtNetwork>
#include <QtXml>
#include <QLocale>
#include <Application.h>

class MetadataFetcher : public QObject
{
Q_OBJECT

public:
    explicit MetadataFetcher(Movie movie, QObject *parent = 0);
    void fetchMetadata(QString title);

signals:
    void movieHydrated(Movie&);
    void noMovieFound();

public slots:
    void replyRelatedMovies(QNetworkReply *reply);
    void replyHydrateMovie(QNetworkReply *reply);
    bool updateMovieInDatabase(Movie &movie);
    void on_doubleClickedMovie(QModelIndex);

private:
    QNetworkAccessManager *m_networkManager;
    Application *m_app;
    Movie m_movie;
    QString m_key;
    void getRelatedMovies(QString title);
    void getMetadata(QString tmdbID);
    QString loadKey();
};

#endif // METADATAFETCHER_H
