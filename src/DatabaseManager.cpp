/* Copyright (C) 2014 Movie-Project
 * (Olivier CHURLAUD)
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

#include <QtDebug>

/**
 * @brief DatabaseManager::DatabaseManager
 * Constructor.
 * Opens the Database. If empty, create the schema.
 */
DatabaseManager::DatabaseManager()
{
    openDB();
    if (m_db.tables().isEmpty())
    {
        createTables();
    }
}

/**
 * @brief DatabaseManager::openDB
 * Opens the database
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

QSqlError DatabaseManager::lastError()
{
    // If opening database has failed user can ask
    // error description by QSqlError::text()
    return m_db.lastError();
}

/**
 * @brief DatabaseManager::closeDB
 * Closes the database.
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
 * @brief DatabaseManager::deleteDB
 * Deletes the database.
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
 * @brief DatabaseManager::createTables
 * Creates all the tables
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
        l_ret = l_query.exec("CREATE TABLE IF NOT EXISTS movies("
                  "id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE, "
                  "title VARCHAR(50), "
                  "original_title VARCHAR(50), "
                  "director VARCHAR(30), "
                  "producer VARCHAR(30), "
                  "year INTEGER, "
                  "country VARCHAR(30), "
                  "duration INTEGER, "
                  "synopsis TEXT, "
                  "file_path VARCHAR(255), "
                  "colored BOOLEAN, "
                  "format VARCHAR(10) "
                  ")");
        l_ret = l_ret && l_query.exec("CREATE TABLE IF NOT EXISTS config("
                                      "id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE, "
                                      "movies_path VARCHAR(255) )");

        l_ret = l_ret && l_query.exec("INSERT INTO config (movies_path) VALUES ('path')");

    }

    return l_ret;
}

/**
 * @brief DatabaseManager::getMovies
 * Gets the movies with a defined parameter.
 *
 * @param parameter_name
 * @param parameter_value
 * @return QSqlQuery query
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
 * @brief DatabaseManager::getAllMovies
 * Gets all the movies
 *
 * @return QSqlQuery query
 */
QSqlQuery DatabaseManager::getAllMovies()
{
    QSqlQuery l_query(m_db);
    l_query.prepare("SELECT title, director, year, format, file_path FROM movies");
    l_query.exec();

    return l_query;
}

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

    QString request = "INSERT into movies (";

    // Even elements are names of fields
    for (int i = 0 ; i < value.size()-1 ; i=i+2)
    {
        request += value.at(i);
        if (i != value.size()-2)
        {
            request += ", ";
        }
    }

    request += ") VALUES (";

    // Odd elements are values of fields
    for (int i = 1 ; i < value.size() ; i=i+2)
    {
        request += "'" + value.at(i) + "'";
        if (i != value.size()-1)
        {
            request += ", ";
        }
    }
    request += ")";


    QSqlQuery l_query(m_db);
    l_query.prepare(request);
    l_query.exec();

    qDebug()<< "******\n" + request;

    return true;
}

/**
 *
 * @brief save the movies' directory
 *
 * @param moviePath a QString containing the path to the movies directory
 *
 * @return true if the config hav been updated correctly
 *
 */
bool DatabaseManager::saveMoviesPath(QString moviePath)
{
    //TODO : is this test really usefull ? this value "sould" be validate before...
    if(moviePath.isEmpty())
    {
        return false;
    }
    else if (!QDir(moviePath).exists())
    {
        return false;
    }


    QSqlQuery query(m_db);
    query.prepare("UPDATE config SET movies_path = :path WHERE id = 1");
    query.bindValue(":path", moviePath);

    return query.exec();


}

QString DatabaseManager::getMoviesPath()
{
    QSqlQuery query(m_db);
    query.prepare("SELECT movies_path FROM config WHERE id = 1");

    query.exec();

    query.first();
    QString result = query.value(0).toString();
    qDebug()<<result;
    return result;
}
