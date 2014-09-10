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

#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#define TYPE_DIRECTOR 1
#define TYPE_PRODUCER 2
#define TYPE_ACTOR 3

#include <QObject>
#include <QtCore>
#include <QtSql>

#include "Entities/Movie.h"


class DatabaseManager : public QObject
{
    public:
        DatabaseManager();
        bool openDB();
        bool createTables();
        bool closeDB();
        bool deleteDB();
        QSqlQuery getMovies(QString, QVariant);
        QVector<Movie> getAllMovies();
        Movie getMoviesById(int);
        QVector<Movie> getMoviesByDirector(People const&);
        QVector<Movie> getMoviesByTag(Tag const&);
        QVector<Movie> getMoviesByAny(QVariant);
        QVector<Tag> getAllTags();
        QVector<People> getAllDirectors();
        People getDirectorById(int);
        QVector<People> getAllActors();
        bool insertNewMovie(Movie);
        QSqlError lastError();
        QSqlQueryModel *createModel();
        bool saveMoviesPath(QString);
        QStringList getMoviesPath();
        QStringListModel *getMoviesPathModel() {return this->m_moviesPathModel;}


    private:
        QSqlDatabase m_db;
        QStringListModel *m_moviesPathModel;

};

#endif // DATABASEMANAGER_H
