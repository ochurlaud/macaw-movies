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

enum typePeople {Director, Producer, Actor};

#include <QObject>
#include <QtCore>
#include <QtSql>

#include "Entities/Movie.h"

class DatabaseManager : public QObject
{
    public:
        DatabaseManager();
        // Database management
        bool openDB();
        bool createTables();
        bool closeDB();
        bool deleteDB();
        QSqlError lastError();

        // Getters for paths, config
        QStringList getMoviesPath();
        QSqlQuery getMovies(QString, QVariant);

        // Insertions for paths, config
        bool saveMoviesPath(QString);

        // Getters
        QVector<Movie> getAllMovies(QString fieldOrder = "title");
        Movie getOneMovieById(int);
        QVector<Movie> getMoviesByPeople(int id, int type, QString fieldOrder = "title");
        QVector<Movie> getMoviesByPeople(People const&, int type, QString fieldOrder = "title");
        QVector<Movie> getMoviesByTag(int id, QString fieldOrder = "title");
        QVector<Movie> getMoviesByTag(Tag const&, QString fieldOrder = "title");
        QVector<Movie> getMoviesWithoutTag(QString fieldOrder = "title");
        QVector<Movie> getMoviesWithoutPeople(int type, QString fieldOrder = "title");
        QVector<Movie> getMoviesByAny(QVariant, QString fieldOrder = "title");
        bool existMovie(QString);
        bool existTag(QString);
        bool existPeople(QString);
        QVector<Tag> getAllTags(QString fieldOrder = "name");
        Tag getOneTagById(int);
        People getOnePeopleById(int);
        People getOnePeopleById(int, int);
        QVector<People> getPeopleByFullname(QString fullname, QString fieldOrder = "lastname");
        QVector<People> getAllPeople(int type, QString fieldOrder = "lastname");
        QVector<People> getAllDirectors(QString fieldOrder = "lastname");
        People getOneDirectorById(int);
        QVector<People> getAllActors(QString fieldOrder = "lastname");
        People getOneActorById(int);
        People getOneProducerById(int);
        void setTagsToMovie(Movie&);
        void setPeopleToMovie(Movie&);

        // Insertion
        bool insertNewMovie(Movie&);
        bool addPeople(People&);
        bool addPeopleToMovie(People&, Movie&, int);
        bool addDirectorToMovie(People&, Movie&);
        bool addProducerToMovie(People&, Movie&);
        bool addActorToMovie(People&, Movie&);
        bool addTagToMovie(Tag&, Movie&);


        // Update
        bool updateMovie(Movie&);
        bool updatePeople(People&);
        bool updatePeopleInMovie(People&, Movie&, int);
        bool updateTag(Tag&);
        bool updateTagInMovie(Tag&, Movie&);

        // Delete
        bool deleteMovie(Movie&);
        bool removePeopleFromMovie(People&, Movie&, int);
        bool removeTagFromMovie(Tag&, Movie&);

        // Models
        QStringListModel *getMoviesPathModel() {return this->m_moviesPathModel;}

    private:
        QSqlDatabase m_db;
        QStringListModel *m_moviesPathModel;
        QString m_movieFields;
        QString m_peopleFields;
        bool deletePeople(People&);
        bool deleteTag(Tag&);
        Movie hydrateMovie(QSqlQuery&);
        People hydratePeople(QSqlQuery&);
};

#endif // DATABASEMANAGER_H
