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

#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QtCore>
#include <QtSql>

#include "include_var.h"
#include "MoviesDebug.h"
#include "Entities/Movie.h"
#include "Entities/Playlist.h"

/**
 * @brief Manages all the access to the database
 */
class DatabaseManager : public QObject
{
    Q_OBJECT

#define DATE_FORMAT "yyyy.MM.dd"

public:
    DatabaseManager();
    // Database management
    bool openDB();
    bool createTables();
    bool closeDB();
    bool deleteDB();
    QSqlError lastError();

    // Getters for paths, config
    QStringList getMoviesPaths(bool imported = true);

    // Insertions for paths, config
    bool addMoviesPath(QString moviesPath);
    int createTag(QString name);
    bool setMoviesPathImported(QString moviesPath, bool imported);

    bool deleteMoviesPath(QString moviesPath);

signals:
    void orphanTagDetected(Tag tag);

//// Getters - in DatabaseManager_getters.cpp
public:
    // Movies
    Movie getOneMovieById(const int id);
    QList<Movie> getAllMovies(const QString fieldOrder = "title");
    QList<Movie> getMoviesByPeople(const int id, const int type, const QString fieldOrder = "title");
    QList<Movie> getMoviesByPeople(const People &people, const int type, const QString fieldOrder = "title");
    QList<Movie> getMoviesByTag(const int id, const QString fieldOrder = "title");
    QList<Movie> getMoviesByTag(const Tag &tag, const QString fieldOrder = "title");
    QList<Movie> getMoviesByPlaylist(const int id, const QString fieldOrder = "title");
    QList<Movie> getMoviesByPlaylist(const Playlist &playlist, const QString fieldOrder = "title");
    QList<Movie> getMoviesWithoutPeople(const int type, const QString fieldOrder = "title");
    QList<Movie> getMoviesWithoutTag(const QString fieldOrder = "title");
    QList<Movie> getMoviesByAny(const QString text, const QString fieldOrder = "title");

    // People
    People getOnePeopleById(const int id);
    People getOnePeopleById(const int id , const int type);
    QList<People> getPeopleByType(const int type, const QString fieldOrder = "lastname");
    QList<People> getPeopleByFullname(const QString fullname, const QString fieldOrder = "lastname");
    QList<People> getPeopleByAny(const QString text, const int type, const QString fieldOrder = "lastname");

    // Tags
    Tag getOneTagById(const int id);
    Tag getTagByName(QString tagName);
    QList<Tag> getAllTags(const QString fieldOrder = "name");
    QList<Tag> getTagsByAny(const QString text, const QString fieldOrder = "name");

    // Playlists
    Playlist getOnePlaylistById(const int id);
    QList<Playlist> getAllPlaylists(const QString fieldOrder = "name");
    QList<Playlist> getPlaylistByAny(const QString text, const QString fieldOrder = "name");
    bool isMovieInPlaylist(int movieId, int playlistId);
    bool isMovieInPlaylist(Movie &movie, int playlistId);
    bool isMovieInPlaylist(Movie &movie, Playlist &playlist);

    // Does element exist ?
    bool existMovie(const QString);
    bool existTag(const QString);
    bool existPeople(const QString);

private:
    // Other functions for getters
    void setPeopleToMovie(Movie &movie);
    void setTagsToMovie(Movie &movie);
    void setMoviesToPlaylist(Playlist &playlist);
    Movie hydrateMovie(QSqlQuery &query);
    People hydratePeople(QSqlQuery &query);
    Tag hydrateTag(QSqlQuery &query);
    Playlist hydratePlaylist(QSqlQuery &query);

//// Inserts - in DatabaseManager_insert.cpp
public:
    bool insertNewMovie(Movie &movie);
    bool insertNewPlaylist(Playlist &playlist);
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
    bool updatePlaylist(Playlist &playlist);
    bool updateMovieInPlaylist(Movie &movie, Playlist &playlist);

//// Delete - in DatabaseManager_delete.cpp
public:
    bool deleteMovie(Movie &movie);
    bool removePeopleFromMovie(People &people, Movie &movie, const int type);
    bool removeTagFromMovie(Tag &tag, Movie &movie);
    bool removeMovieFromPlaylist(Movie &movie, Playlist &playlist);
    bool deletePlaylist(Playlist &playlist);
    bool deleteTag(const Tag &tag);

private:
    bool deletePeople(const People &people);

private:
    QSqlDatabase m_db;
    QString m_movieFields;
    QString m_peopleFields;
    QString m_tagFields;

};

#endif // DATABASEMANAGER_H
