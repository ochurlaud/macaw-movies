/* Copyright (C) 2014 Macaw-Movies
 * (Olivier CHURLAUD, Sébastien TOUZÉ)
 *
 * This file is part of Macaw-Movies.
 *
 * Macaw-Movies is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * Macaw-Movies is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with Macaw-Movies.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "DatabaseManager.h"

/**
 * @brief Constructor.
 * Opens the Database. If empty, create the schema.
 */
DatabaseManager::DatabaseManager()
{
    openDB();
    createTables();
    m_movieFields = "m.id, "
                    "m.title, "
                    "m.original_title, "
                    "m.release_date, "
                    "m.country, "
                    "m.duration, "
                    "m.synopsis, "
                    "m.file_path, "
                    "m.poster_path, "
                    "m.colored, "
                    "m.format, "
                    "m.suffix, "
                    "m.rank, "
                    "m.imported, "
                    "m.series";

    m_episodeFields = "e.id, "
                      "e.number, "
                      "e.season, "
                      "e.id_series, "
                      "e.id_movie";

    m_seriesFields =  "s.id, "
                      "s.name, "
                      "s.finished";

    m_peopleFields = "p.id, "
                     "p.name, "
                     "p.birthday, "
                     "p.biography";

    m_tagFields = "t.id, "
                  "t.name";

    Macaw::DEBUG("[DatabaseManager] object created");
}


/**
 * @brief Opens (or create the file of) the database
 *
 * @return bool db.open()
 */
bool DatabaseManager::openDB()
{
    Macaw::DEBUG("[DatabaseManager] openDB");
    if (QSqlDatabase::contains("Movies-database"))
    {
        m_db = QSqlDatabase::database("Movies-database");        
    }
    else
    {
        m_db = QSqlDatabase::addDatabase("QSQLITE", "Movies-database");
    }

    QString l_dbPath = qApp->property("filesPath").toString() + "database.sqlite";
    m_db.setDatabaseName(l_dbPath);

    if (!m_db.open()) {
        return false;
    }

    QSqlQuery l_query(m_db);
    l_query.prepare("PRAGMA foreign_keys = ON");

    if (!l_query.exec()) {
        Macaw::DEBUG("In openDB:");
        Macaw::DEBUG(l_query.lastError().text());
        return false;
    }

    return true;
}

/**
 * @brief Closes the database.
 *
 * @return bool
 */
bool DatabaseManager::closeDB()
{
    Macaw::DEBUG("[DatabaseManager] Close database");
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
    Macaw::DEBUG("[DatabaseManager] deleteDB");
    closeDB();

    return QFile::remove(m_db.databaseName());
}

/**
 * @brief Upgrades DB between diffent DB versions
 * This function needs to be updated each time a change to the DB structure is made
 *
 * @return bool
 */
bool DatabaseManager::upgradeDB(int fromVersion, int toVersion)
{
    //At the moment this function can only handle move from
    //version 1 to version 3.

    Macaw::DEBUG_IN("[DatabaseManager] upgradeDB");
    bool  l_ret = false;
    int l_fromVersion(fromVersion);

    if (m_db.isOpen())
    {
        // We need to have a clean version of the db instance.
        //There might be a better way to do this.
        m_db.close();
        m_db.open();

        QSqlQuery l_query(m_db);

        l_ret = l_query.exec("PRAGMA foreign_keys = ON");

        //switch from DB_VERSION 1 to DB_VERSION 2
        if (l_fromVersion == 1 && toVersion >= 2) {
            //add media player table
            Macaw::DEBUG_IN("[DatabaseManager] upgrade from v1 to v2");

            l_ret &= l_query.exec("CREATE TABLE IF NOT EXISTS media_player("
                                  "id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE, "
                                  "media_player_path VARCHAR(255) UNIQUE"
                                  ")");
            if(!l_ret)
            {
                Macaw::DEBUG(l_query.lastError().text());
            }

            // Update the database version if media player table was inserted successfully
            if(l_ret) {
                l_ret &= l_query.exec("UPDATE config "
                                              "SET db_version = 2");
            l_fromVersion = 2;
            }
            Macaw::DEBUG_OUT("[DatabaseManager] exits upgrade from v1 to v2");
        }

        //switch from DB_VERSION 2 to DB_VERSION 3
        if (l_fromVersion == 2 && toVersion >= 3) {

            Macaw::DEBUG_IN("[DatabaseManager] upgrade from v2 to v3");
            l_query.finish();
            l_query.clear();
            l_ret &= l_query.exec("DROP TABLE media_player");
            if(!l_ret)
            {
                Macaw::DEBUG(l_query.lastError().text());
            }

            l_ret &= l_query.exec("ALTER TABLE config ADD media_player VARCHAR(255)");
            if(!l_ret)
            {
                Macaw::DEBUG(l_query.lastError().text());
            }

            l_ret &= l_query.exec("ALTER TABLE movies ADD series BOOLEAN");
            l_ret &= l_query.exec("UPDATE movies SET series = 0");
            if(!l_ret)
            {
                Macaw::DEBUG(l_query.lastError().text());
            }

            l_ret &= createTableSeries(l_query);
            if(!l_ret)
            {
                Macaw::DEBUG(l_query.lastError().text());
            }
            l_ret &= createTableEpisodes(l_query);
            if(!l_ret)
            {
                Macaw::DEBUG(l_query.lastError().text());
            }

            if(l_ret) {
                l_ret &= l_query.exec("UPDATE config "
                                      "SET db_version = 3");
                l_fromVersion = 3;
            }
            Macaw::DEBUG_OUT("[DatabaseManager] exits upgrade from v2 to v3");
        }
    }
    Macaw::DEBUG_OUT("[DatabaseManager] exits upgradeDB");

    return l_ret;
}

/**
 * @brief Creates all the tables
 *
 * @return bool
 */
bool DatabaseManager::createTables()
{
    Macaw::DEBUG("[DatabaseManager] createTables");
    bool l_ret = false;

    if (m_db.isOpen())
    {
        QSqlQuery l_query(m_db);

        if(m_db.tables().contains("config"))
        {
            Macaw::DEBUG("[DatabaseManager.createTable] config table exists");
            l_query.exec("SELECT db_version FROM config");
            l_query.next();
            if(l_query.value(0) != DB_VERSION)
            {
                l_ret = upgradeDB(l_query.value(0).toInt(), DB_VERSION);
            }
        }
        else    //if config table do not exists then the db is empty...
        {
            Macaw::DEBUG("[DatabaseManager.createTable] configTable does not exist");

            l_ret = l_query.exec("PRAGMA foreign_keys = ON");

            l_ret &= createTableMovies(l_query);
            l_ret &= createTablePeople(l_query);
            l_ret &= createTableMoviesPeople(l_query);
            l_ret &= createTablePlaylists(l_query);
            l_ret &= createTableMoviesPlaylists(l_query);
            l_ret &= createTableTags(l_query);
            l_ret &= createTableMoviesTags(l_query);
            l_ret &= createTableSeries(l_query);
            l_ret &= createTableEpisodes(l_query);
            l_ret &= createTablePathsList(l_query);
            if (l_ret) {
                l_ret &= createTableConfig(l_query);
            }
        }
    }

    return l_ret;
}

/**
 * @brief Create the table `movies`
 * @param query
 * @return
 */
bool DatabaseManager::createTableMovies(QSqlQuery &query)
{
    query.prepare("CREATE TABLE IF NOT EXISTS movies("
                  "id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE, "
                  "title VARCHAR(255) NOT NULL, "
                  "original_title VARCHAR(255), "
                  "release_date VARCHAR(10), "
                  "country VARCHAR(50), "
                  "duration INTEGER, "
                  "synopsis TEXT, "
                  "file_path VARCHAR(255) UNIQUE NOT NULL, "
                  "poster_path VARCHAR(255), "
                  "colored BOOLEAN, "
                  "format VARCHAR(10), "
                  "suffix VARCHAR(10), "
                  "rank INTEGER, "
                  "imported BOOLEAN, "
                  "series BOOLEAN"
                  ")");

    if (!query.exec()) {
        Macaw::DEBUG("In createTableMovies:");
        Macaw::DEBUG(query.lastError().text());

        return false;
    }

    return true;
}

/**
 * @brief Create the table `people`
 * @param query
 * @return
 */
bool DatabaseManager::createTablePeople(QSqlQuery &query)
{
    query.prepare("CREATE TABLE IF NOT EXISTS people("
                  "id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE, "
                  "name VARCHAR(200) NOT NULL, "
                  "birthday VARCHAR(10), "
                  "biography TEXT"
                  ")");

    if (!query.exec()) {
        Macaw::DEBUG("In createTablePeople:");
        Macaw::DEBUG(query.lastError().text());

        return false;
    }

    return true;
}
/**
 * @brief Create the table `movies_people` which links between people and movies (a type of person is given here)
 * @param query
 * @return
 */
bool DatabaseManager::createTableMoviesPeople(QSqlQuery &query)
{
    query.prepare("CREATE TABLE IF NOT EXISTS movies_people("
                  "id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE, "
                  "id_movie INTEGER NOT NULL, "
                  "id_people INTEGER NOT NULL, "
                  "type INTEGER NOT NULL, "
                  "UNIQUE (id_people, id_movie, type) ON CONFLICT IGNORE, "
                  "FOREIGN KEY(id_movie) REFERENCES movies ON DELETE CASCADE, "
                  "FOREIGN KEY(id_people) REFERENCES people ON DELETE CASCADE"
                  ")");

    if (!query.exec()) {
        Macaw::DEBUG("In createTableMoviesPeople:");
        Macaw::DEBUG(query.lastError().text());

        return false;
    }

    return true;
}

/**
 * @brief Create the table `playlists` and the "To Watch" default playlist
 * @param query
 * @return
 */
bool DatabaseManager::createTablePlaylists(QSqlQuery &query)
{
    query.prepare("CREATE TABLE IF NOT EXISTS playlists("
                  "id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE, "
                  "name VARCHAR(255) UNIQUE NOT NULL, "
                  "rate INTEGER, "
                  "creation_date INT"
                  ")");

    if (!query.exec()) {
        Macaw::DEBUG("In createTablePlaylists:");
        Macaw::DEBUG(query.lastError().text());

        return false;
    }

    // Default Playlist: To Watch
    query.prepare("INSERT INTO playlists "
                  "VALUES(1, 'To Watch', 0, 0)");

    if (!query.exec()) {
        Macaw::DEBUG("In createTablePlaylists:");
        Macaw::DEBUG(query.lastError().text());

        return false;
    }

    return true;
}

/**
 * @brief Create the table `movies_playlists` which links movies to playlists
 * @param query
 * @return
 */
bool DatabaseManager::createTableMoviesPlaylists(QSqlQuery &query)
{
    query.prepare("CREATE TABLE IF NOT EXISTS movies_playlists("
                  "id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE, "
                  "id_movie INTEGER NOT NULL, "
                  "id_playlist INTEGER NOT NULL, "
                  "UNIQUE (id_playlist, id_movie) ON CONFLICT IGNORE, "
                  "FOREIGN KEY(id_movie) REFERENCES movies ON DELETE CASCADE, "
                  "FOREIGN KEY(id_playlist) REFERENCES playlists ON DELETE CASCADE"
                  ")");

    if (!query.exec()) {
        Macaw::DEBUG("In createTableMoviesPlaylists:");
        Macaw::DEBUG(query.lastError().text());

        return false;
    }

    return true;
}

/**
 * @brief Create the table `tags`, to store the tags that can be attributed to the movies
 * @param query
 * @return
 */
bool DatabaseManager::createTableTags(QSqlQuery &query)
{
    query.prepare("CREATE TABLE IF NOT EXISTS tags("
                  "id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE, "
                  "name VARCHAR(255) UNIQUE NOT NULL"
                  ")");

    if (!query.exec()) {
        Macaw::DEBUG("In createTableTags:");
        Macaw::DEBUG(query.lastError().text());

        return false;
    }

    return true;
}

/**
 * @brief Create the table `movies_tags` where are the links between tags and movies
 * @param query
 * @return
 */
bool DatabaseManager::createTableMoviesTags(QSqlQuery &query)
{
    query.prepare("CREATE TABLE IF NOT EXISTS movies_tags("
                  "id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE, "
                  "id_movie INTEGER NOT NULL, "
                  "id_tag INTEGER NOT NULL, "
                  "UNIQUE (id_tag, id_movie) ON CONFLICT IGNORE, "
                  "FOREIGN KEY(id_movie) REFERENCES movies ON DELETE CASCADE, "
                  "FOREIGN KEY(id_tag) REFERENCES tags ON DELETE CASCADE"
                  ")");

    if (!query.exec()) {
        Macaw::DEBUG("In createTableMoviesTags:");
        Macaw::DEBUG(query.lastError().text());

        return false;
    }

    return true;
}

bool DatabaseManager::createTableSeries(QSqlQuery &query)
{
    query.prepare("CREATE TABLE IF NOT EXISTS series("
                  "id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE, "
                  "name VARCHAR(255), "
                  "finished BOOLEAN"
                  ")");

    if (!query.exec()) {
        Macaw::DEBUG("In createTableSeries:");
        Macaw::DEBUG(query.lastError().text());

        return false;
    }

    return true;
}

bool DatabaseManager::createTableEpisodes(QSqlQuery &query)
{
    query.prepare("CREATE TABLE IF NOT EXISTS episodes("
                  "id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE, "
                  "number INTEGER, "
                  "season INTEGER, "
                  "id_series INTEGER NOT NULL, "
                  "id_movie INTEGER UNIQUE NOT NULL, "
                  "FOREIGN KEY(id_movie) REFERENCES movies ON DELETE CASCADE, "
                  "FOREIGN KEY(id_series) REFERENCES series ON DELETE CASCADE "
                  ")");

    if (!query.exec()) {
        Macaw::DEBUG("In createTableEpisodes:");
        Macaw::DEBUG(query.lastError().text());

        return false;
    }

    return true;
}

/**
 * @brief Create table `paths_lists`, where the paths to import are stored
 * @param query
 * @return
 */
bool DatabaseManager::createTablePathsList(QSqlQuery &query)
{
    query.prepare("CREATE TABLE IF NOT EXISTS paths_list("
                  "id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE, "
                  "movies_path VARCHAR(255) UNIQUE,"
                  "imported BOOLEAN DEFAULT 0"
                  ")");

    if (!query.exec()) {
        Macaw::DEBUG("In createTablePathsList:");
        Macaw::DEBUG(query.lastError().text());

        return false;
    }

    return true;
}

/**
 * @brief Create table `config`, for update purpose and app configuration
 * Then set the version of the db
 * @param query
 * @return
 */
bool DatabaseManager::createTableConfig(QSqlQuery &query)
{
    query.prepare("CREATE TABLE IF NOT EXISTS config("
                  "db_version INTEGER,"
                  "media_player VARCHAR(255))");

    if (!query.exec()) {
        Macaw::DEBUG("In createTableConfig:");
        Macaw::DEBUG(query.lastError().text());

        return false;
    }

    // Set the database version
    query.prepare("INSERT INTO config (`db_version`) "
                  "VALUES ('" + QString::number(DB_VERSION) + "')");

    if (!query.exec()) {
        Macaw::DEBUG("In createTableConfig:");
        Macaw::DEBUG(query.lastError().text());

        return false;
    }

    return true;
}

/**
 * @brief add a new tag with specifed name to the database.
 * Returns the id of created tag, -1 if an error occured.
 * @param name new tag's name
 * @returns the id of newly created tag in the db, -1 if an error occurs
 */
int DatabaseManager::createTag(QString name)
{
    QSqlQuery l_query(m_db);
    l_query.prepare("INSERT INTO `tags` (name) VALUES (:name)");
    l_query.bindValue(":name", name);

    if (!l_query.exec())
    {
        Macaw::DEBUG("In createTag():");
        Macaw::DEBUG(l_query.lastError().text());

        return -1;
    }

    if(l_query.exec("SELECT last_insert_rowid()"))
    {
        l_query.next();
        return l_query.value(0).toInt();
    }

    return -1;
}

/**
 * @brief Adds a movies directory
 *
 * @param QString moviesPath: containing the path to the movies directory
 * @return true if the paths list have been updated correctly
 */
bool DatabaseManager::addMoviesPath(QString moviesPath)
{
    QSqlQuery l_query(m_db);
    l_query.prepare("INSERT INTO paths_list (movies_path) VALUES (:movies_path)");
    l_query.bindValue(":movies_path", moviesPath);

    if(!l_query.exec())
    {
        Macaw::DEBUG("[DatabaseManager] In addMoviesPath():");
        Macaw::DEBUG(l_query.lastError().text());

        return false;
    }

    return true;
}

/**
 * @brief Adds a path to the media player
 *
 * @param QString mediaPlayerPath: containing the path to the media player
 * @return true if the path has been updated correctly
 */
bool DatabaseManager::addMediaPlayerPath(QString mediaPlayerPath)
{
    QSqlQuery l_query(m_db);
    l_query.prepare("DELETE FROM media_player");

    if(!l_query.exec()) {
        Macaw::DEBUG("[DatabaseManager] In addMediaPlayerPath():");
        Macaw::DEBUG(l_query.lastError().text());

        return false;
    }

    if (!mediaPlayerPath.trimmed().isEmpty()) {
        l_query.prepare("INSERT INTO media_player (media_player_path) VALUES (:media_player_path)");
        l_query.bindValue(":media_player_path", mediaPlayerPath);

        if(!l_query.exec()) {
            Macaw::DEBUG("[DatabaseManager] In addMediaPlayerPath():");
            Macaw::DEBUG(l_query.lastError().text());

            return false;
        }
    }

    return true;
}

/**
 * @brief Set the imported boolean of the movie path
 *
 * @param QString moviesPath: containing the path to the movies directory
 * @param bool imported: true if the movies have been imported, false else
 * @return true if the request succeed
 */
bool DatabaseManager::setMoviesPathImported(QString moviesPath, bool imported)
{
    QSqlQuery l_query(m_db);
    l_query.prepare("UPDATE paths_list SET imported=:imported WHERE movies_path = :movies_path");
    l_query.bindValue(":movies_path", moviesPath);
    l_query.bindValue(":imported", imported);

    if(!l_query.exec())
    {
        Macaw::DEBUG("In setMoviesPathImported():");
        Macaw::DEBUG(l_query.lastError().text());

        return false;
    }

    return true;
}

/**
 * @brief Get the movies directories
 * @param imported boolean telling if the movies of this path have already been imported
 * @return QStringList containing the paths of these directories
 */
QStringList DatabaseManager::getMoviesPaths(bool imported)
{
    QSqlQuery l_query(m_db);
    l_query.prepare("SELECT movies_path FROM paths_list where imported=:imported");
    l_query.bindValue(":imported", imported);

    if(!l_query.exec())
    {
        Macaw::DEBUG("In getMoviesPaths():");
        Macaw::DEBUG(l_query.lastError().text());
    }

    QStringList l_moviesPathsList;

    while(l_query.next())
    {
        l_moviesPathsList.append(l_query.value(0).toString());
    }

    return l_moviesPathsList;
}

bool DatabaseManager::deleteMoviesPath(QString moviesPath)
{
    QSqlQuery l_query(m_db);
    l_query.prepare("DELETE FROM movies WHERE file_path LIKE :movies_path||'%'");
    l_query.bindValue(":movies_path", moviesPath);

    if(!l_query.exec())
    {
        Macaw::DEBUG("In removeMoviesPath():");
        Macaw::DEBUG(l_query.lastError().text());

        return false;
    }

    l_query.prepare("DELETE FROM paths_list WHERE movies_path LIKE :movies_path||'%'");
    l_query.bindValue(":movies_path", moviesPath);
    if(!l_query.exec())
    {
        Macaw::DEBUG("In removeMoviesPath():");
        Macaw::DEBUG(l_query.lastError().text());

        return false;
    }

    return true;
}

/**
 * @brief Get path to media player
 * @return QStringList containing the path to the media player
 */
QString DatabaseManager::getMediaPlayerPath()
{
    QSqlQuery l_query(m_db);
    l_query.prepare("SELECT media_player FROM config");

    if(!l_query.exec())
    {
        Macaw::DEBUG("In getMediaPlayerPath():");
        Macaw::DEBUG(l_query.lastError().text());
    }

    QString l_mediaPlayerPath("");

    while(l_query.next())
    {
        l_mediaPlayerPath.append(l_query.value(0).toString());
    }

    return l_mediaPlayerPath;
}
