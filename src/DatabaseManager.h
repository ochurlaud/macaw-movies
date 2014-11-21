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

enum typePeople {None, Director, Producer, Actor};

#include <QObject>
#include <QtCore>
#include <QtSql>

#include "Entities/Movie.h"
#include "MoviesDebug.h"

class DatabaseManager : public QObject
{

#define DATE_FORMAT "yyyy.MM.dd"

public:
    DatabaseManager(MoviesDebug *moviesDebug);
    // Database management
    bool openDB();
    bool createTables();
    bool closeDB();
    bool deleteDB();
    void setDebug(MoviesDebug*);
    QSqlError lastError();

    // Getters for paths, config
    QStringList getMoviesPath();
    QStringList getTags();

    // Insertions for paths, config
    bool saveMoviesPath(QString);
    bool createTag(QString name);


//// Getters - in DatabaseManager_getters.cpp
public:
    // Movies
    Movie getOneMovieById(const int id);
    QList<Movie> getAllMovies(const QString fieldOrder = "title");
    QList<Movie> getMoviesByPeople(const int id, const int type, const QString fieldOrder = "title");
    QList<Movie> getMoviesByPeople(const People &people, const int type, const QString fieldOrder = "title");
    QList<Movie> getMoviesByTag(const int id, const QString fieldOrder = "title");
    QList<Movie> getMoviesByTag(const Tag &tag, const QString fieldOrder = "title");
    QList<Movie> getMoviesWithoutPeople(const int type, const QString fieldOrder = "title");
    QList<Movie> getMoviesWithoutTag(const QString fieldOrder = "title");
    QList<Movie> getMoviesByAny(const QString text, const QString fieldOrder = "title");

    // People
    People getOnePeopleById(const int id);
    People getOnePeopleById(const int id , const int type);
    QList<People> getAllPeople(const int type, const QString fieldOrder = "lastname");
    QList<People> getPeopleByFullname(const QString fullname, const QString fieldOrder = "lastname");
    QList<People> getPeopleByAny(const QString text, const int type, const QString fieldOrder = "lastname");

    // Tags
    Tag getOneTagById(const int id);
    QList<Tag> getAllTags(const QString fieldOrder = "name");

    // Does element exist ?
    bool existMovie(const QString);
    bool existTag(const QString);
    bool existPeople(const QString);

private:
    // Other functions for getters
    void setPeopleToMovie(Movie &movie);
    void setTagsToMovie(Movie &movie);
    Movie hydrateMovie(QSqlQuery &query);
    People hydratePeople(QSqlQuery &query);

//// Inserts - in DatabaseManager_insert.cpp
public:
    bool insertNewMovie(Movie &movie);
    bool addTagToMovie(Tag &tag, Movie &movie);
    bool addPeopleToMovie(People &people, Movie &movie, const int type);

private:
    bool insertNewPeople(People &people);
    bool insertNewTag(Tag &tag);


//// Updates - in DatabaseManager_update.cpp
public:
    bool updateMovie(Movie &movie);
    bool updatePeople(People &people);
    bool updatePeopleInMovie(People &people, Movie &movie, const int type);
    bool updateTag(Tag &tag);
    bool updateTagInMovie(Tag &tag, Movie &movie);

//// Delete - in DatabaseManager_delete.cpp
public:
    bool deleteMovie(Movie &movie);
    bool removePeopleFromMovie(People &people, Movie &movie, const int type);
    bool removeTagFromMovie(Tag &tag, Movie &movie);

private:
    bool deletePeople(const People &people);
    bool deleteTag(const Tag &tag);

//// Models
public:
    QStringListModel *getMoviesPathModel() {return this->m_moviesPathModel;}
    QStringListModel *getTagListModel() {return this->m_tagListModel;}

private:
    void debug(QString text) { m_debug->print(text);}
    QSqlDatabase m_db;
    MoviesDebug *m_debug;
    QStringListModel *m_moviesPathModel;
    QStringListModel *m_tagListModel;
    QString m_movieFields;
    QString m_peopleFields;

};

#endif // DATABASEMANAGER_H
