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
DatabaseManager::DatabaseManager(MoviesDebug *moviesDebug)
{
    m_debug = moviesDebug;
    openDB();

    createTables();
    m_movieFields = "m.id, m.title, m.original_title, m.release_date, m.country, m.duration, m.synopsis, m.file_path, m.colored, m.format, m.suffix, m.rank";
    m_peopleFields = "p.id, p.firstname, p.lastname, p.realname, p.birthday, p.biography";
    m_tagFields = "t.id, t.name";
    m_moviesPathModel = new QStringListModel();
    m_tagListModel = new QStringListModel();
    debug("[DatabaseManager] object created");
}

/**
 * @brief Opens the database
 *
 * @return bool db.open()
 */
bool DatabaseManager::openDB()
{
    debug("[DatabaseManager] openDB");
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
    debug("[DatabaseManager] Close database");
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
    debug("[DatabaseManager] deleteDB");
    closeDB();

#ifdef Q_OS_LINUX
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
    debug("[DatabaseManager] createTables");
    bool l_ret = false;

    if (m_db.isOpen())
    {
        QSqlQuery l_query(m_db);

        if(m_db.tables().contains("config"))
        {
            debug("[DatabaseManager.createTable] config table exists");
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
            debug("[DatabaseManager.createTable] configTable does not exist");

            // Movies
            l_ret = l_query.exec("CREATE TABLE IF NOT EXISTS movies("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE, "
                      "title VARCHAR(255) NOT NULL, "
                      "original_title VARCHAR(255), "
                      "release_date VARCHAR(10), "
                      "country VARCHAR(50), "
                      "duration INTEGER, "
                      "synopsis TEXT, "
                      "file_path VARCHAR(255) UNIQUE NOT NULL, "
                      "colored BOOLEAN, "
                      "format VARCHAR(10), "
                      "suffix VARCHAR(10), "
                      "rank INTEGER"
                      ")");

            // Peoples (directors, actor, music...)
            l_ret = l_ret && l_query.exec("CREATE TABLE IF NOT EXISTS people("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE, "
                      "lastname VARCHAR(100) NOT NULL, "
                      "firstname VARCHAR(100), "
                      "realname VARCHAR(255), "
                      "birthday VARCHAR(10), "
                      "biography TEXT"
                      ")");

            // Links between people and movies (a type of person is given here)
            l_ret = l_ret && l_query.exec("CREATE TABLE IF NOT EXISTS movies_people("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE, "
                      "id_movie INTEGER NOT NULL, "
                      "id_people INTEGER NOT NULL, "
                      "type INTEGER NOT NULL, "
                      "UNIQUE (id_people, id_movie, type) ON CONFLICT IGNORE "
                      ")");

            // Tags that can be attributed to the movies
            l_ret = l_ret && l_query.exec("CREATE TABLE IF NOT EXISTS tags("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE, "
                      "name VARCHAR(255) UNIQUE NOT NULL"
                      ")");

            // Links between tags and movies
            l_ret = l_ret && l_query.exec("CREATE TABLE IF NOT EXISTS movies_tags("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE, "
                      "id_movie INTEGER NOT NULL, "
                      "id_tag INTEGER NOT NULL, "
                      "UNIQUE (id_tag, id_movie) ON CONFLICT IGNORE "
                      ")");

            // Playlists
            l_ret = l_ret && l_query.exec("CREATE TABLE IF NOT EXISTS playlists("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE, "
                      "name VARCHAR(255) UNIQUE NOT NULL, "
                      "rate INTEGER, "
                      "creation_date INT"
                      ")");

            // Default Playlist: To Watch
            l_ret = l_ret && l_query.exec ("INSERT INTO playlists "
                                           "VALUES(1, 'To Watch', 0, 0)");

            // Links between playlist and movies
            l_ret = l_ret && l_query.exec("CREATE TABLE IF NOT EXISTS movies_playlists("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE, "
                      "id_movie INTEGER NOT NULL, "
                      "id_playlist INTEGER NOT NULL, "
                      "UNIQUE (id_playlist, id_movie) ON CONFLICT IGNORE "
                      ")");

            // List of paths where the movies are stored
            l_ret = l_ret && l_query.exec("CREATE TABLE IF NOT EXISTS paths_list("
                                          "id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE, "
                                          "movies_path VARCHAR(255) UNIQUE"
                                          ")");

            // Config table (for update purposes)
            l_ret = l_ret && l_query.exec("CREATE TABLE IF NOT EXISTS config("
                                          "db_version INTERGER )");

            // Set the database version
            l_ret = l_ret && l_query.exec("INSERT INTO config (`db_version`) VALUES ('" + QString::number(DB_VERSION) + "')");
        }
    }

    return l_ret;
}

void DatabaseManager::setDebug(MoviesDebug* moviesDebug)
{
    m_debug = moviesDebug;
}


bool DatabaseManager::createTag(QString name)
{
    QSqlQuery l_query(m_db);
    l_query.prepare("INSERT INTO `tags` (name) VALUES (:name)");
    l_query.bindValue(":name", name);

    if (!l_query.exec())
    {
        debug("In createTag():");
        debug(l_query.lastError().text());

        return false;
    }

    m_tagListModel->insertRow(m_tagListModel->rowCount());
    m_tagListModel->setData(m_tagListModel->index(m_tagListModel->rowCount()-1), name);

    return true;
}

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
    l_query.prepare("SELECT movies_path FROM paths_list");
    if(!l_query.exec())
    {
        debug("In saveMoviesPath(), getting existing path:");
        debug(l_query.lastError().text());
    }

    while(l_query.next())
    {
        if(l_query.value(0).toString() == moviePath)
        {
            return false;
        }
    }

    l_query.prepare("INSERT INTO paths_list (movies_path) VALUES (:path)");
    l_query.bindValue(":path", moviePath);

    //updates the movies path list in the settings window
    m_moviesPathModel->insertRow(m_moviesPathModel->rowCount());
    m_moviesPathModel->setData(m_moviesPathModel->index(m_moviesPathModel->rowCount()-1), moviePath);

    if(!l_query.exec())
    {
        debug("In saveMoviesPath():");
        debug(l_query.lastError().text());

        return false;
    }

    return true;
}

/**
 * @brief Get the movies directories
 *
 * @return QStringList containing the paths of these directories
 */
QStringList DatabaseManager::getMoviesPath()
{
    QSqlQuery l_query(m_db);
    l_query.prepare("SELECT movies_path FROM paths_list");

    if(!l_query.exec())
    {
        debug("In getMoviesPath():");
        debug(l_query.lastError().text());
    }

    QStringList l_result;

    while(l_query.next())
    {
        l_result.append(l_query.value(0).toString());
    }

    m_moviesPathModel->setStringList(l_result);

    return l_result;
}

/**
 * @brief Get the tags in the database
 *
 * @return QStringList containing all the tags
 */
QStringList DatabaseManager::getTags()
{
    QSqlQuery l_query(m_db);
    l_query.prepare("SELECT name FROM tags");

    if(!l_query.exec())
    {
        debug("In getTags():");
        debug(l_query.lastError().text());
    }

    QStringList l_result;

    while(l_query.next())
    {
        l_result.append(l_query.value(0).toString());
    }

    m_tagListModel->setStringList(l_result);

    return l_result;
}
