/* Copyright (C) 2014 Movie-Project
 * (Olivier CHURLAUD, Sébastien TOUZÉ)
 *
 * This file is part of Movie-Project.
 *
 * Movie-Project is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * Movie-Project is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with Movie-Project.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "DatabaseManager.h"

#include "main.cpp" //needed for the DB_VERSION

#include <QtDebug>

/**
 * @brief Constructor.
 * Opens the Database. If empty, create the schema.
 */
DatabaseManager::DatabaseManager()
{
    openDB();

    createTables();

    m_moviesPathModel = new QStringListModel();

}

/**
 * @brief Opens the database
 *
 * @return bool db.open()
 */
bool DatabaseManager::openDB()
{

    if (QSqlDatabase::contains("Movies-database"))
    {
        m_db = QSqlDatabase::database("Movies-database");        
    }
    else
    {
        m_db = QSqlDatabase::addDatabase("QSQLITE", "Movies-database");
    }

#ifdef Q_OS_LINUX
    // NOTE: We have to store database file into user home folder in Linux
    QString path(QDir::home().path());
    path.append(QDir::separator()).append("movie-project.sqlite");
    path = QDir::toNativeSeparators(path);

    m_db.setDatabaseName(path);
#else
    // NOTE: File exists in the application private folder, in Symbian Qt implementation
    m_db.setDatabaseName("movie-project.sqlite");
#endif

    // Open databasee
    return m_db.open();
}

/**
 * @brief Closes the database.
 *
 * @return bool
 */
bool DatabaseManager::closeDB()
{
    // Close database
    m_db.close();

    return true;
}

/**
 * @brief Deletes the database.
 *
 * @return bool
 */
bool DatabaseManager::deleteDB()
{
    // Close database
    closeDB();

    #ifdef Q_OS_LINUX
    // NOTE: We have to store database file into user home folder in Linux
    QString path(QDir::home().path());
    path.append(QDir::separator()).append("movie-project.sqlite");
    path = QDir::toNativeSeparators(path);
    return QFile::remove(path);
    #else
    // Remove created database binary file
    return QFile::remove("movie-project.sqlite");
    #endif
}

/**
 * @brief Creates all the tables
 *
 * @return bool
 */
bool DatabaseManager::createTables()
{
    // Create table "movies"
    bool l_ret = false;

    if (m_db.isOpen())
    {
        QSqlQuery l_query(m_db);

        if(m_db.tables().contains("config"))
        {
            l_query.exec("SELECT db_version FROM config");
            l_query.next();
            if(l_query.value(0) != DB_VERSION) //TODO : make en intelligent upgrade of the database
            {
                QMessageBox msgBox;
                msgBox.setText("Error your database version is "+ l_query.value(0).toString() +" which is too old.\n"+
                               "This program is not currently able to update the database and will close.\n"+
                               "Please delete your old database. ");
                msgBox.setIcon(QMessageBox::Critical);
                msgBox.exec();
                exit(1);
            }
        }
        else    //if config table do not exists then the db is empty...
        {
            // Movies
            l_ret = l_query.exec("CREATE TABLE IF NOT EXISTS movies("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE, "
                      "title VARCHAR(255), "
                      "original_title VARCHAR(255), "
                      "year INTEGER(4), "
                      "country VARCHAR(50), "
                      "duration INTEGER, "
                      "synopsis TEXT, "
                      "file_path VARCHAR(255), "
                      "colored BOOLEAN, "
                      "format VARCHAR(10), "
                      "suffix VARCHAR(10), "
                      "rank INTEGER"
                      ")");

            // Peoples (directors, actor, music...)
            l_ret = l_ret && l_query.exec("CREATE TABLE IF NOT EXISTS people("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE, "
                      "lastname VARCHAR(100), "
                      "firstname VARCHAR(100), "
                      "real_name VARCHAR(255), "
                      "birthday INTEGER, "
                      "biography TEXT"
                      ")");

            // Links between people and movies (a type of person is given here)
            l_ret = l_ret && l_query.exec("CREATE TABLE IF NOT EXISTS people_movies("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE, "
                      "id_people INTEGER, "
                      "id_movie INTEGER, "
                      "type INTEGER"
                      ")");

            // Tags that can be attributed to the movies
            l_ret = l_ret && l_query.exec("CREATE TABLE IF NOT EXISTS tags("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE, "
                      "tag_name VARCHAR(255)"
                      ")");

            // Links between tags and movies
            l_ret = l_ret && l_query.exec("CREATE TABLE IF NOT EXISTS tags_movies("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE, "
                      "id_tag INTEGER, "
                      "id_movie INTEGER"
                      ")");
            // List of paths where the movies are stored
            l_ret = l_ret && l_query.exec("CREATE TABLE IF NOT EXISTS paths_list("
                                          "id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE, "
                                          "movies_path VARCHAR(255))");

            // Config table (for update purposes)
            l_ret = l_ret && l_query.exec("CREATE TABLE IF NOT EXISTS config("
                                          "db_version INTERGER )");

            // Set the database version
            l_ret = l_ret && l_query.exec("INSERT INTO config (`db_version`) VALUES ('" + QString::number(DB_VERSION) + "')");

            //l_ret = l_ret && l_query.exec("INSERT INTO paths_list (movies_path) VALUES ('path')");
        }

    }

    return l_ret;
}

/**
 * @brief Gets the one movie that has the id `id`
 *
 * @param int id of the movie
 * @return Movie
 */
Movie DatabaseManager::getOneMovieById(int id)
{
    Movie l_movie;
    QSqlQuery l_query(m_db);
    l_query.prepare("SELECT id, title, original_title, year, country, duration, synopsis, file_path, colored, format, suffix, rank "
                    "FROM movies "
                    "WHERE id = :id");
    l_query.bindValue(":id", id);

    if (!l_query.exec())
    {
        qDebug() << l_query.lastError().text();
    }

    if(l_query.next())
    {
        Movie l_movie;
        l_movie.setId(l_query.value(0).toInt());
        l_movie.setTitle(l_query.value(1).toString());
        l_movie.setOriginalTitle(l_query.value(2).toString());
        l_movie.setYear(l_query.value(3).toInt());
        l_movie.setCountry(l_query.value(4).toString());
        l_movie.setDuration(l_query.value(5).toInt());
        l_movie.setSynopsis(l_query.value(6).toString());
        l_movie.setFilePath(l_query.value(7).toString());
        l_movie.setColored(l_query.value(8).toBool());
        l_movie.setFormat(l_query.value(9).toString());
        l_movie.setSuffix(l_query.value(10).toString());
        l_movie.setRank(l_query.value(11).toInt());
    }

    return l_movie;
}

/**
 * @brief Gets all the movies
 *
 * @return QVector<Movie>
 */
QVector<Movie> DatabaseManager::getAllMovies()
{
    QVector<Movie> l_moviesVector;
    QSqlQuery l_query(m_db);
    l_query.prepare("SELECT id, title, original_title, year, country, duration, synopsis, file_path, colored, format, suffix, rank "
                    "FROM movies ");

    if (!l_query.exec())
    {
        qDebug() << l_query.lastError().text();
    }

    while(l_query.next())
    {
        Movie l_movie;
        l_movie.setId(l_query.value(0).toInt());
        l_movie.setTitle(l_query.value(1).toString());
        l_movie.setOriginalTitle(l_query.value(2).toString());
        l_movie.setYear(l_query.value(3).toInt());
        l_movie.setCountry(l_query.value(4).toString());
        l_movie.setDuration(l_query.value(5).toInt());
        l_movie.setSynopsis(l_query.value(6).toString());
        l_movie.setFilePath(l_query.value(7).toString());
        l_movie.setColored(l_query.value(8).toBool());
        l_movie.setFormat(l_query.value(9).toString());
        l_movie.setSuffix(l_query.value(10).toString());
        l_movie.setRank(l_query.value(11).toInt());

        l_moviesVector.push_back(l_movie);
    }

    return l_moviesVector;
}

/**
 * @brief Gets all the movies directed by `director`
 *
 * @param People director
 * @return QVector<Movie>
 */
QVector<Movie> DatabaseManager::getMoviesByDirector(People const &director)
{
    QVector<Movie> l_moviesVector;
    QSqlQuery l_query(m_db);
    l_query.prepare("SELECT id, title, original_title, year, country, duration, synopsis, file_path, colored, format, suffix, rank "
                    "FROM movies "
                    "WHERE id = (SELECT id_movie "
                                "FROM people_movies "
                                "WHERE id_people = :id AND type = :type) ");
    l_query.bindValue(":id", director.getId());
    l_query.bindValue(":type", TYPE_DIRECTOR);

    if (!l_query.exec())
    {
        qDebug() << l_query.lastError().text();
    }

    while(l_query.next())
    {
        Movie l_movie;
        l_movie.setId(l_query.value(0).toInt());
        l_movie.setTitle(l_query.value(1).toString());
        l_movie.setOriginalTitle(l_query.value(2).toString());
        l_movie.setYear(l_query.value(3).toInt());
        l_movie.setCountry(l_query.value(4).toString());
        l_movie.setDuration(l_query.value(5).toInt());
        l_movie.setSynopsis(l_query.value(6).toString());
        l_movie.setFilePath(l_query.value(7).toString());
        l_movie.setColored(l_query.value(8).toBool());
        l_movie.setFormat(l_query.value(9).toString());
        l_movie.setSuffix(l_query.value(10).toString());
        l_movie.setRank(l_query.value(11).toInt());

        l_moviesVector.push_back(l_movie);
    }

    return l_moviesVector;
}

/**
 * @brief Gets all the movies tagged by 'tag'
 *
 * @param Tag tag
 * @return QVector<Movie>
 */
QVector<Movie> DatabaseManager::getMoviesByTag(Tag const &tag)
{
    QVector<Movie> l_moviesVector;
    QSqlQuery l_query(m_db);
    l_query.prepare("SELECT id, title, original_title, year, country, duration, synopsis, file_path, colored, format, suffix, rank "
                    "FROM movies "
                    "WHERE id = (SELECT id_movie "
                                "FROM tags_movies "
                                "WHERE id_tag = :id) ");
    l_query.bindValue(":id", tag.getId());

    if (!l_query.exec())
    {
        qDebug() << l_query.lastError().text();
    }

    while(l_query.next())
    {
        Movie l_movie;
        l_movie.setId(l_query.value(0).toInt());
        l_movie.setTitle(l_query.value(1).toString());
        l_movie.setOriginalTitle(l_query.value(2).toString());
        l_movie.setYear(l_query.value(3).toInt());
        l_movie.setCountry(l_query.value(4).toString());
        l_movie.setDuration(l_query.value(5).toInt());
        l_movie.setSynopsis(l_query.value(6).toString());
        l_movie.setFilePath(l_query.value(7).toString());
        l_movie.setColored(l_query.value(8).toBool());
        l_movie.setFormat(l_query.value(9).toString());
        l_movie.setSuffix(l_query.value(10).toString());
        l_movie.setRank(l_query.value(11).toInt());

        l_moviesVector.push_back(l_movie);
    }

    return l_moviesVector;
}

QVector<Movie> getMoviesByAny(QVariant value)
{
    QVector<Movie> l_movies;

    return l_movies;
}

/**
 * @brief Gets all the directors
 *
 * @return QVector<People>
 */
QVector<People> DatabaseManager::getAllDirectors()
{
    QVector<People> l_directorsVector;
    QSqlQuery l_query(m_db);

    l_query.prepare("SELECT id, firstname, lastname, real_name, birthday, biography "
                    "FROM people "
                    "WHERE id = (SELECT id_people FROM people_movies WHERE type = :type) ");
    l_query.bindValue(":type", TYPE_DIRECTOR);

    if (!l_query.exec())
    {
        qDebug() << l_query.lastError().text();
    }

    if(l_query.next())
    {
        People l_director;
        l_director.setId(l_query.value(0).toInt());
        l_director.setFirstname(l_query.value(1).toString());
        l_director.setLastname(l_query.value(2).toString());
        l_director.setRealname(l_query.value(3).toString());
        l_director.setBirthday(l_query.value(4).toInt());
        l_director.setBiography(l_query.value(5).toString());

        l_directorsVector.push_back(l_director);
    }

    return l_directorsVector;
}

/**
 * @brief Gets the one director that has the id `id`
 *
 * @param int id of the director
 * @return People
 */
People DatabaseManager::getOneDirectorById(int id)
{
    People l_director;
    QSqlQuery l_query(m_db);

    l_query.prepare("SELECT id, firstname, lastname, real_name, birthday, biography "
                    "FROM people "
                    "WHERE id = :id AND EXISTS (SELECT type FROM people_movies WHERE id_people = :id2 AND type = :type) ");
    l_query.bindValue(":id", id);
    l_query.bindValue(":id2", id);
    l_query.bindValue(":type", TYPE_DIRECTOR);

    if (!l_query.exec())
    {
        qDebug() << l_query.lastError().text();
    }

    if(l_query.next())
    {
        l_director.setId(l_query.value(0).toInt());
        l_director.setFirstname(l_query.value(1).toString());
        l_director.setLastname(l_query.value(2).toString());
        l_director.setRealname(l_query.value(3).toString());
        l_director.setBirthday(l_query.value(4).toInt());
        l_director.setBiography(l_query.value(5).toString());
    }

    return l_director;
}

/**
 * @brief Gets all the directors
 *
 * @return QVector<People>
 */
QVector<People> DatabaseManager::getAllActors()
{
    QVector<People> l_actorsVector;
    QSqlQuery l_query(m_db);

    l_query.prepare("SELECT id, firstname, lastname, real_name, birthday, biography "
                    "FROM people "
                    "WHERE id = (SELECT id_people FROM people_movies WHERE type = :type) ");
    l_query.bindValue(":type", TYPE_ACTOR);

    if (!l_query.exec())
    {
        qDebug() << l_query.lastError().text();
    }

    if(l_query.next())
    {
        People l_actor;
        l_actor.setId(l_query.value(0).toInt());
        l_actor.setFirstname(l_query.value(1).toString());
        l_actor.setLastname(l_query.value(2).toString());
        l_actor.setRealname(l_query.value(3).toString());
        l_actor.setBirthday(l_query.value(4).toInt());
        l_actor.setBiography(l_query.value(5).toString());

        l_actorsVector.push_back(l_actor);

    }

    return l_actorsVector;
}

/**
 * @brief Adds a movie to the database
 *
 * @return bool
 */
QVector<Tag> DatabaseManagergetAllTags()
{
    QVector<Tag> l_tags;

    return l_tags;
}

/**
 * @brief Adds a movie to the database
 *
 * @return bool
 */
bool DatabaseManager::insertNewMovie(Movie movie)
{
    QSqlQuery l_query(m_db);
    l_query.prepare("INSERT INTO movies(title, original_title, year, country, duration, synopsis, file_path, colored, format, suffix, rank) "
                    "VALUES (:title, :original_title, :year, :country, :duration, :synopsis, :file_path, :colored, :format, :suffix, :rank)");
    l_query.bindValue(":title", movie.getTitle());
    l_query.bindValue(":original_title", movie.getOriginalTitle()   );
    l_query.bindValue(":year", movie.getYear());
    l_query.bindValue(":country", movie.getCountry());
    l_query.bindValue(":duration", movie.getDuration());
    l_query.bindValue(":synopsis", movie.getSynopsis());
    l_query.bindValue(":file_path", movie.getFilePath());
    l_query.bindValue(":colored", movie.isColored());
    l_query.bindValue(":format", movie.getFormat());
    l_query.bindValue(":suffix", movie.getSuffix());
    l_query.bindValue(":rank", movie.getRank());

    if (!l_query.exec())
    {
        qDebug() << m_db.lastError().text();

        return false;
    }

    qDebug() << "[DatabaseManager] Movie added";

    movie.setId(l_query.lastInsertId().toInt());

    for(int i = 0 ; i < movie.getDirectors().size() ; i++)
    {
        People l_director = movie.getDirectors().at(i);
        if (!addDirectorToMovie(l_director, movie))
        {
            return false;
        }
    }

    for(int i = 0 ; i < movie.getProducers().size() ; i++)
    {
        People l_producer = movie.getProducers().at(i);
        if (!addProducerToMovie(l_producer, movie))
        {
            return false;
        }
    }
    for(int i = 0 ; i < movie.getActors().size() ; i++)
    {
        People l_actor = movie.getActors().at(i);
        if (!addActorToMovie(l_actor, movie))
        {
            return false;
        }

    }

    return true;
}

/**
 * @brief Adds a person to the database and links it to a movie
 *
 * @param People
 * @param Movie
 * @param int type: the type of People
 * @return bool
 */
bool DatabaseManager::addPeopleToMovie(People &people, Movie &movie, int type)
{
    QSqlQuery l_query(m_db);
    l_query.prepare("INSERT INTO people (lastname, firstname, real_name, birthday, biography) "
                    "VALUES (:lastname, :firstname, :real_name, :birthday, :biography)");
    l_query.bindValue(":lastname", people.getLastname());
    l_query.bindValue(":firstname", people.getFirstname());
    l_query.bindValue(":real_name", people.getRealname());
    l_query.bindValue(":birthday", people.getBirthday());
    l_query.bindValue(":biography", people.getBiography());

    if (!l_query.exec())
    {
        qDebug() << m_db.lastError().text();

        return false;
    }

    people.setId(l_query.lastInsertId().toInt());
    l_query.prepare("INSERT INTO people_movies (id_people, id_movie, type) "
                    "VALUES (:people, :id_movie, :type)");
    l_query.bindValue(":id_people", people.getId());
    l_query.bindValue(":id_movie", movie.getId());
    l_query.bindValue(":id_movie", type);

    if (!l_query.exec())
    {
        qDebug() << m_db.lastError().text();

        return false;
    }

    return true;
}

/**
 * @brief Adds a director to the database and links it to a movie
 *
 * @param People
 * @param Movie
 * @return bool
 */
bool DatabaseManager::addDirectorToMovie(People &people, Movie &movie)
{
    addPeopleToMovie(people, movie, TYPE_DIRECTOR);
    qDebug() << "[DatabaseManager] One director added";
}

/**
 * @brief Adds a producer to the database and links it to a movie
 *
 * @param People
 * @param Movie
 * @return bool
 */
bool DatabaseManager::addProducerToMovie(People &people, Movie &movie)
{
    addPeopleToMovie(people, movie, TYPE_PRODUCER);
    qDebug() << "[DatabaseManager] One producer added";
}

/**
 * @brief Adds an actor to the database and links it to a movie
 *
 * @param People
 * @param Movie
 * @return bool
 */
bool DatabaseManager::addActorToMovie(People &people, Movie &movie)
{
    addPeopleToMovie(people, movie, TYPE_ACTOR);
    qDebug() << "[DatabaseManager] One actor added";
}

/*
bool DatabaseManager::insertNewTitle(QStringList value)
{
    if (value.size() % 2 == 1)
    {
        return false;
    }

    QString l_request = "INSERT into movies (";

    // Even elements are names of fields
    for (int i = 0 ; i < value.size()-1 ; i=i+2)
    {
        l_request += value.at(i);
        if (i != value.size()-2)
        {
            l_request += ", ";
        }
    }

    l_request += ") VALUES (";

    // Odd elements are values of fields
    for (int i = 1 ; i < value.size() ; i=i+2)
    {
        l_request += "'" + value.at(i) + "'";
        if (i != value.size()-1)
        {
            l_request += ", ";
        }
    }
    l_request += ")";


    QSqlQuery l_query(m_db);
    l_query.prepare(l_request);
    l_query.exec();

    qDebug()<< "******\n" + l_request;

    return true;
}
*/

/**
 * @brief Saves the movies directory
 *
 * @param QString moviePath: containing the path to the movies directory
 *
 * @return true if the paths list have been updated correctly
 */
bool DatabaseManager::saveMoviesPath(QString moviePath)
{
    //TODO : is this test really usefull ? this value "sould" be validated before...
    if(moviePath.isEmpty())
    {
        return false;
    }
    else if (!QDir(moviePath).exists())
    {
        return false;
    }


    QSqlQuery l_query(m_db);
    l_query.prepare("INSERT INTO paths_list (movies_path) VALUES (:path)");
    l_query.bindValue(":path", moviePath);

    return l_query.exec();
}

/**
 * @brief Get the movies directory
 *
 * @return QStringList containing the paths of these directories
 */
QStringList DatabaseManager::getMoviesPath()
{
    QSqlQuery l_query(m_db);
    l_query.prepare("SELECT movies_path FROM paths_list");

    l_query.exec();

    QStringList l_result;

    while(l_query.next())
    {
        l_result.append(l_query.value(0).toString());
    }

    m_moviesPathModel->setStringList(l_result);

    return l_result;
}
