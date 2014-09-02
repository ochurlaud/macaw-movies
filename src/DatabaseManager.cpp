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
    m_db.close();

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
            qDebug() << "config";
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
                      "year INTEGER, "
                      "country VARCHAR(50), "
                      "duration INTEGER, "
                      "synopsis TEXT, "
                      "file_path VARCHAR(255), "
                      "colored BOOLEAN, "
                      "format VARCHAR(10), "
                      "rank INTEGER"
                      ")");

            // Peoples (directors, actor, music...)
            l_ret = l_ret && l_query.exec("CREATE TABLE IF NOT EXISTS people("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE, "
                      "name VARCHAR(100), "
                      "first_name VARCHAR(100), "
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
 * @brief Gets the movies with a defined parameter.
 *
 * @param QString parameter_name
 * @param QVariant parameter_value
 * @return QSqlQuery
 */
QSqlQuery DatabaseManager::getMovies(QString parameter_name, QVariant parameter_value)
{
    QSqlQuery l_query(m_db);
    l_query.prepare("SELECT * FROM movies WHERE " + parameter_name + "=:parameter_value");
    l_query.bindValue(":parameter_value", parameter_value);
    l_query.exec();

    return l_query;
}

/**
 * @brief Gets all the movies
 *
 * @return QSqlQuery
 */
QSqlQuery DatabaseManager::getAllMovies()
{
    QSqlQuery l_query(m_db);
    l_query.prepare("SELECT title, year, format, file_path FROM movies");
    l_query.exec();

    return l_query;
}

/**
 * @brief Opens the database
 *
 * @return QSqlQueryModel
 */
QSqlQueryModel *DatabaseManager::createModel()
{
   QSqlQueryModel *l_model = new QSqlQueryModel;


   return l_model;
}

QSqlQuery DatabaseManager::getAllTitles()
{
    QSqlQuery l_query(m_db);
    l_query.prepare("SELECT title || ifnull(' (' || year || ')','') as value FROM movies");
    l_query.exec();

    return l_query;
}

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
