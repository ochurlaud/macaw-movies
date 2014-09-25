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
                      "realname VARCHAR(255), "
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
                      "name VARCHAR(255)"
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
        qDebug() << "In getOneMovieById(int):";
        qDebug() << l_query.lastError().text();
    }

    if(l_query.next())
    {
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

        setTagsToMovie(l_movie);
        setPeopleToMovie(l_movie);
    }

    return l_movie;
}

/**
 * @brief Gets the tags of a movie and adds it to the object
 * @param Movie
 */
void DatabaseManager::setTagsToMovie(Movie &movie)
{
    QSqlQuery l_query(m_db);
    l_query.prepare("SELECT tag.id, tag.name "
                    "FROM tags AS tag, tags_movies AS tm "
                    "WHERE tm.id_movie = :id_movie AND tm.id_tag = tag.id");
    l_query.bindValue(":id_movie", movie.getId());

    if (!l_query.exec())
    {
        qDebug() << "In setTagToMovie(Movie):";
        qDebug() << l_query.lastError().text();
    }
    while (l_query.next())
    {
        Tag l_tag;
        l_tag.setId(l_query.value(0).toInt());
        l_tag.setName(l_query.value(1).toString());
        movie.addTag(l_tag);
    }
}

/**
 * @brief Gets the people of a movie and adds it to the object
 * @param Movie
 */
void DatabaseManager::setPeopleToMovie(Movie &movie)
{
    QSqlQuery l_query(m_db);
    l_query.prepare("SELECT p.id, p.lastname, p.firstname, p.realname, p.birthday, p.biography, pm.type "
                    "FROM people AS p, people_movies AS pm "
                    "WHERE pm.id_movie = :id_movie AND pm.id_people = p.id");
    l_query.bindValue(":id_movie", movie.getId());

    if (!l_query.exec())
    {
        qDebug() << "In setPeopleToMovie(Movie):";
        qDebug() << l_query.lastError().text();
    }
    while (l_query.next())
    {
        People l_people;
        l_people.setId(l_query.value(0).toInt());
        l_people.setFirstname(l_query.value(1).toString());
        l_people.setLastname(l_query.value(2).toString());
        l_people.setRealname(l_query.value(3).toString());
        l_people.setBirthday(l_query.value(4).toInt());
        l_people.setBiography(l_query.value(5).toString());
        int l_type = l_query.value(6).toInt();

        switch (l_type)
        {
            case TYPE_DIRECTOR:
                movie.addDirector(l_people);
                break;
            case TYPE_PRODUCER:
                movie.addProducer(l_people);
                break;
            case TYPE_ACTOR:
                movie.addActor(l_people);
                break;
        }
    }
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
        qDebug() << "In getAllMovies():";
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

        setTagsToMovie(l_movie);
        setPeopleToMovie(l_movie);

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
        qDebug() << "In getMoviesByDirector(People):";
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

        setTagsToMovie(l_movie);
        setPeopleToMovie(l_movie);

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
        qDebug() << "In getMoviesByTag(Tag):";
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

        setTagsToMovie(l_movie);
        setPeopleToMovie(l_movie);

        l_moviesVector.push_back(l_movie);
    }

    return l_moviesVector;
}

QVector<Movie> getMoviesByAny(QVariant value)
{
    QVector<Movie> l_moviesVector;

    return l_moviesVector;
}

bool DatabaseManager::existMovie(QString filePath)
{
    QSqlQuery l_query(m_db);
    l_query.prepare("SELECT id FROM movies WHERE file_path = :file_path ");
    l_query.bindValue(":file_path", filePath);
    l_query.exec();

    return l_query.next();
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

    l_query.prepare("SELECT id, firstname, lastname, realname, birthday, biography "
                    "FROM people "
                    "WHERE id = (SELECT id_people FROM people_movies WHERE type = :type) ");
    l_query.bindValue(":type", TYPE_DIRECTOR);

    if (!l_query.exec())
    {
        qDebug() << "In getAllDirectors():";
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
 * @brief Gets the one person that has the id `id`
 *
 * @param int id of the person
 * @param int type of the person
 * @return People
 */
People DatabaseManager::getOnePeopleById(int id, int type)
{
    People l_people;
    QSqlQuery l_query(m_db);

    l_query.prepare("SELECT p.id, p.firstname, p.lastname, p.realname, p.birthday, p.biography "
                    "FROM people AS p, people_movies AS pm "
                    "WHERE p.id = :id AND pm.id_people = p.id AND pm.type = :type ");
    l_query.bindValue(":id", id);
    l_query.bindValue(":type", type);

    if (!l_query.exec())
    {
        qDebug() << "In getOnePeopleBy(int, type):";
        qDebug() << l_query.lastError().text();
    }

    if(l_query.next())
    {
        l_people.setId(l_query.value(0).toInt());
        l_people.setFirstname(l_query.value(1).toString());
        l_people.setLastname(l_query.value(2).toString());
        l_people.setRealname(l_query.value(3).toString());
        l_people.setBirthday(l_query.value(4).toInt());
        l_people.setBiography(l_query.value(5).toString());
    }

    return l_people;
}

/**
 * @brief Gets the people whose 'lastname firstname' or 'firstname lastname' is `fullname`
 *
 * @param QString fullname, the string searched
 * @param int type of the person
 * @return QVector<People>
 */
QVector<People> DatabaseManager::getPeopleByFullname(QString fullname, int type)
{
    QVector<People> l_peopleVector;
    QSqlQuery l_query(m_db);

    l_query.prepare("SELECT p.id, p.firstname, p.lastname, p.realname, p.birthday, p.biography "
                    "FROM people AS p "
                    "WHERE (p.lastname || ' ' || p.firstname LIKE '%'||:fullname||'%' "
                        "OR p.firstname || ' ' ||  p.lastname LIKE '%'||:fullname||'%' "
                        "OR p.realname LIKE '%'||:fullname||'%') "
                        "AND (SELECT COUNT(*) "
                             "FROM people_movies AS pm "
                             "WHERE pm.id_people = p.id AND type = :type) > 0");
    l_query.bindValue(":fullname", fullname);
    l_query.bindValue(":type", type);

    if (!l_query.exec())
    {
        qDebug() << "In getPeopleByFullname(int, type):";
        qDebug() << l_query.lastError().text();
    }

    while(l_query.next())
    {
        People l_people;
        l_people.setId(l_query.value(0).toInt());
        l_people.setFirstname(l_query.value(1).toString());
        l_people.setLastname(l_query.value(2).toString());
        l_people.setRealname(l_query.value(3).toString());
        l_people.setBirthday(l_query.value(4).toInt());
        l_people.setBiography(l_query.value(5).toString());
        l_peopleVector.push_back(l_people);
    }

    return l_peopleVector;
}

/**
* @brief Gets the one director that has the id `id`
*
* @param int id of the director
* @param int type of the director
* @return People
*/
People DatabaseManager::getOneDirectorById(int id)
{
   return getOnePeopleById(id, TYPE_DIRECTOR);
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

    l_query.prepare("SELECT id, firstname, lastname, realname, birthday, biography "
                    "FROM people "
                    "WHERE id = (SELECT id_people FROM people_movies WHERE type = :type) ");
    l_query.bindValue(":type", TYPE_ACTOR);

    if (!l_query.exec())
    {
        qDebug() << "In getAllActors():";
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
* @brief Gets the one director that has the id `id`
*
* @param int id of the producer
* @param int type of the producer
* @return People
*/
People DatabaseManager::getOneProducerById(int id)
{
   return getOnePeopleById(id, TYPE_PRODUCER);
}


/**
* @brief Gets the one director that has the id `id`
*
* @param int id of the actor
* @param int type of the actor
* @return People
*/
People DatabaseManager::getOneActorById(int id)
{
   return getOnePeopleById(id, TYPE_ACTOR);
}


/**
 * @brief Gets all the tags
 *
 * @return QVector<Tag>
 */
QVector<Tag> DatabaseManager::getAllTags()
{
    QVector<Tag> l_tagsVector;
    QSqlQuery l_query(m_db);
    l_query.prepare("SELECT id, name "
                    "FROM tags");

    if (!l_query.exec())
    {
        qDebug() << l_query.lastError().text();
    }

    while(l_query.next())
    {
        Tag l_tag;
        l_tag.setId(l_query.value(0).toInt());
        l_tag.setName(l_query.value(1).toString());
        l_tagsVector.push_back(l_tag);
    }

    return l_tagsVector;
}

/**
 * @brief Gets the tag which id is `id`
 *
 * @param int id
 * @return Tag
 */
Tag DatabaseManager::getOneTagById(int id)
{
    Tag l_tag;
    QSqlQuery l_query(m_db);
    l_query.prepare("SELECT id, name "
                    "FROM tags "
                    "WHERE id = :id");
    l_query.bindValue(":id", id);

    if (!l_query.exec())
    {
        qDebug() << "In getOneTagBy(int):";
        qDebug() << l_query.lastError().text();
    }

    if(l_query.next())
    {
        l_tag.setId(l_query.value(0).toInt());
        l_tag.setName(l_query.value(1).toString());
    }

    return l_tag;
}

/**
 * @brief Adds a movie to the database
 *
 * @param Movie
 * @return bool
 */
bool DatabaseManager::insertNewMovie(Movie &movie)
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
        qDebug() << "In insertNewMovie():";
        qDebug() << l_query.lastError().text();

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
    l_query.prepare("INSERT INTO people (lastname, firstname, realname, birthday, biography) "
                    "VALUES (:lastname, :firstname, :realname, :birthday, :biography)");
    l_query.bindValue(":lastname", people.getLastname());
    l_query.bindValue(":firstname", people.getFirstname());
    l_query.bindValue(":realname", people.getRealname());
    l_query.bindValue(":birthday", people.getBirthday());
    l_query.bindValue(":biography", people.getBiography());

    if (!l_query.exec())
    {
        qDebug() << "In addPeopleToMovie():";
        qDebug() << l_query.lastError().text();

        return false;
    }

    people.setId(l_query.lastInsertId().toInt());
    l_query.prepare("INSERT INTO people_movies (id_people, id_movie, type) "
                    "VALUES (:id_people, :id_movie, :type)");
    l_query.bindValue(":id_people", people.getId());
    l_query.bindValue(":id_movie", movie.getId());
    l_query.bindValue(":type", type);

    if (!l_query.exec())
    {
        qDebug() << "In addPeopleToMovie():";
        qDebug() << l_query.lastError().text();

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
    bool l_ret = addPeopleToMovie(people, movie, TYPE_DIRECTOR);
    qDebug() << "[DatabaseManager] One director added";

    return l_ret;
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
    bool l_ret = addPeopleToMovie(people, movie, TYPE_PRODUCER);
    qDebug() << "[DatabaseManager] One producer added";

    return l_ret;
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
    bool l_ret = addPeopleToMovie(people, movie, TYPE_ACTOR);
    qDebug() << "[DatabaseManager] One actor added";

    return l_ret;
}

/**
 * @brief Adds a tag to the database and links it to a movie
 *
 * @param Tag
 * @param Movie
 * @return bool
 */
bool DatabaseManager::addTagToMovie(Tag &tag, Movie &movie)
{
    QSqlQuery l_query(m_db);
    l_query.prepare("INSERT INTO tag (name) "
                    "VALUES (:name)");
    l_query.bindValue(":name", tag.getName());

    if (!l_query.exec())
    {
        qDebug() << "In addTagToMovie():";
        qDebug() << l_query.lastError().text();

        return false;
    }

    tag.setId(l_query.lastInsertId().toInt());
    l_query.prepare("INSERT INTO tags_movies (id_tag, id_movie) "
                    "VALUES (:id_tag, :id_movie)");
    l_query.bindValue(":id_tag", tag.getId());
    l_query.bindValue(":id_movie", movie.getId());

    if (!l_query.exec())
    {
        qDebug() << "In addTagToMovie():";
        qDebug() << l_query.lastError().text();

        return false;
    }

    return true;
}

bool DatabaseManager::updatePeople(People &people)
{
    QSqlQuery l_query(m_db);
    l_query.prepare("UPDATE people "
                    "SET firstname = :firstname, "
                        "lastname = :lastname, "
                        "realname = :realname, "
                        "birthday = :birthday, "
                        "biography = :biography "
                    "WHERE id = :id");
    l_query.bindValue(":firstname", people.getFirstname());
    l_query.bindValue(":lastname", people.getLastname());
    l_query.bindValue(":realname",  people.getRealname());
    l_query.bindValue(":birthday",  people.getBirthday());
    l_query.bindValue(":biography", people.getBiography());
    l_query.bindValue(":id", people.getId());

    if (!l_query.exec())
    {
        qDebug() << "In updateMovie():";
        qDebug() << l_query.lastError().text();

        return false;
    }

    return true;
}

bool DatabaseManager::updatePeopleInMovie(People &people, Movie &movie, int type)
{
    // If the id is 0, then the director doesn't exist
    if (people.getId() == 0)
    {
        qDebug() << "People not known";
        addPeopleToMovie(people, movie, type);
    }
    // This means that the director exists, so we upgrade
    else
    {
        qDebug() << "People known";
        updatePeople(people);

        // Checks if the people and the movie are connected, if not connects them
        QSqlQuery l_query(m_db);
        l_query.prepare("SELECT id "
                        "FROM people_movies "
                        "WHERE id_movie = :id_movie AND id_people = :id_people");
        l_query.bindValue(":id_movie", movie.getId());
        l_query.bindValue(":id_people", people.getId());
        if (!l_query.exec())
        {
            qDebug() << "In updatePeopleInMovie():";
            qDebug() << l_query.lastError().text();

            return false;
        }

        if (!l_query.next())
        {
            qDebug() << "People not connected to the movie";
            l_query.prepare("INSERT INTO people_movies(id_movie, id_people, type) "
                            "VALUES(:id_movie, :id_people, :type)");
            l_query.bindValue(":id_movie", movie.getId());
            l_query.bindValue(":id_people", people.getId());
            l_query.bindValue(":type", type);
            if (!l_query.exec())
            {
                qDebug() << "In updatePeopleInMovie():";
                qDebug() << l_query.lastError().text();

                return false;
            }
        }
    }

    return true;
}

bool DatabaseManager::updateTag(Tag &tag)
{
    QSqlQuery l_query(m_db);
    l_query.prepare("UPDATE tags "
                    "SET name = :name "
                    "WHERE id = :id");
    l_query.bindValue(":name", tag.getName());
    l_query.bindValue(":id", tag.getId());

    if (!l_query.exec())
    {
        qDebug() << "In updateTag():";
        qDebug() << l_query.lastError().text();

        return false;
    }

    return true;
}

bool DatabaseManager::updateTagInMovie(Tag &tag, Movie &movie)
{
    // If the id is 0, then the tag doesn't exist
    if (tag.getId() == 0)
    {
        qDebug() << "Tag not known";
        addTagToMovie(tag, movie);
    }
    // This means that the tag exists, so we upgrade
    else
    {
        qDebug() << "Tag known";
        updateTag(tag);

        // Checks if the people and the movie are connected, if not connects them
        QSqlQuery l_query(m_db);
        l_query.prepare("SELECT id "
                        "FROM tags_movies "
                        "WHERE id_movie = :id_movie AND id_tag = :id_tag");
        l_query.bindValue(":id_movie", movie.getId());
        l_query.bindValue(":id_tag", tag.getId());
        if (!l_query.exec())
        {
            qDebug() << "In updateTagInMovie():";
            qDebug() << l_query.lastError().text();

            return false;
        }

        if (!l_query.next())
        {
            qDebug() << "Tag not connected to the movie";
            l_query.prepare("INSERT INTO tags_movies(id_movie, id_tag) "
                            "VALUES(:id_movie, :id_tag)");
            l_query.bindValue(":id_movie", movie.getId());
            l_query.bindValue(":id_tag", tag.getId());
            if (!l_query.exec())
            {
                qDebug() << "In updateTagInMovie():";
                qDebug() << l_query.lastError().text();

                return false;
            }
        }
    }

    return true;
}

/**
 * @brief Updates a movie from database
 *
 * @param Movie
 * @return bool
 */
bool DatabaseManager::updateMovie(Movie &movie)
{
    QSqlQuery l_query(m_db);
    l_query.prepare("UPDATE movies "
                    "SET title = :title, "
                        "original_title = :original_title, "
                        "year = :year, "
                        "country = :country, "
                        "synopsis = :synopsis, "
                        "colored = :colored, "
                        "format = :format, "
                        "rank = :rank "
                    "WHERE id = :id");
    l_query.bindValue(":title", movie.getTitle());
    l_query.bindValue(":original_title", movie.getOriginalTitle());
    l_query.bindValue(":year", movie.getYear());
    l_query.bindValue(":country", movie.getCountry());
    l_query.bindValue(":synopsis", movie.getSynopsis());
    l_query.bindValue(":colored", movie.isColored());
    l_query.bindValue(":format", movie.getFormat());
    l_query.bindValue(":rank", movie.getRank());
    l_query.bindValue(":id", movie.getId());


    if (!l_query.exec())
    {
        qDebug() << "In updateMovie():";
        qDebug() << l_query.lastError().text();

        return false;
    }

    People l_director;
    foreach (l_director, movie.getDirectors())
    {
        updatePeopleInMovie(l_director, movie, TYPE_DIRECTOR);
    }
    People l_producer;
    foreach (l_producer, movie.getProducers())
    {
        updatePeopleInMovie(l_director, movie, TYPE_PRODUCER);
    }
    People l_actor;
    foreach (l_actor, movie.getActors())
    {
        updatePeopleInMovie(l_director, movie, TYPE_ACTOR);
    }
    Tag l_tag;
    foreach (l_tag, movie.getTags())
    {
        updateTagInMovie(l_tag, movie);
    }
    qDebug() << "[DatabaseManager] Movie updated";

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

    //Updates the dataBase
    QSqlQuery l_query(m_db);
    l_query.prepare("INSERT INTO paths_list (movies_path) VALUES (:path)");
    l_query.bindValue(":path", moviePath);

    //updates the movies path list in the settings window
    m_moviesPathModel->insertRow(m_moviesPathModel->rowCount());
    m_moviesPathModel->setData(m_moviesPathModel->index(m_moviesPathModel->rowCount()-1), moviePath);

    if(!l_query.exec())
    {
        qDebug() << "In saveMoviesPath():";
        qDebug() << l_query.lastError().text();

        return false;
    }

    return true;
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

    if(!l_query.exec())
    {
        qDebug() << "In getMoviesPath():";
        qDebug() << l_query.lastError().text();
    }

    QStringList l_result;

    while(l_query.next())
    {
        l_result.append(l_query.value(0).toString());
    }

    m_moviesPathModel->setStringList(l_result);

    return l_result;
}
