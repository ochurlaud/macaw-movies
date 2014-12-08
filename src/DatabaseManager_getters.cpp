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

#include "DatabaseManager.h"

/**
 * @brief Gets the one movie that has the id `id`
 *
 * @param int id of the movie
 * @return Movie
 */
Movie DatabaseManager::getOneMovieById(const int id)
{
    Movie l_movie;
    QSqlQuery l_query(m_db);
    l_query.prepare("SELECT " + m_movieFields + " "
                    "FROM movies AS m "
                    "WHERE id = :id");
    l_query.bindValue(":id", id);

    if (!l_query.exec())
    {
        qDebug() << "In getOneMovieById(int):";
        qDebug() << l_query.lastError().text();
    }

    if(l_query.next())
    {
        l_movie = hydrateMovie(l_query);
    }

    return l_movie;
}

/**
 * @brief Gets all the movies
 *
 * @return QList<Movie>
 */
QList<Movie> DatabaseManager::getAllMovies(const QString fieldOrder)
{
    QList<Movie> l_movieList;
    QSqlQuery l_query(m_db);
    l_query.prepare("SELECT " + m_movieFields + " "
                    "FROM movies AS m "
                    "ORDER BY " + fieldOrder);

    if (!l_query.exec())
    {
        qDebug() << "In getAllMovies():";
        qDebug() << l_query.lastError().text();
    }

    while(l_query.next())
    {
        Movie l_movie = hydrateMovie(l_query);
        l_movieList.push_back(l_movie);
    }

    return l_movieList;    Movie getOneMovieById(int);

}

/**
 * @brief Gets all the movies directed by people having the id `id` and of type `type`
 *
 * @param int id of the people
 * @param int type of the people
 * @return QList<Movie>
 */
QList<Movie> DatabaseManager::getMoviesByPeople(const int id,
                                                const int type,
                                                const QString fieldOrder)
{
    QList<Movie> l_movieList;
    QSqlQuery l_query(m_db);
    l_query.prepare("SELECT " + m_movieFields + " "
                    "FROM movies AS m "
                    "WHERE id IN (SELECT id_movie "
                                 "FROM movies_people "
                                 "WHERE id_people = :id AND type = :type) "
                                 "ORDER BY " + fieldOrder);
    l_query.bindValue(":id", id);
    l_query.bindValue(":type", type);

    if (!l_query.exec())
    {
        qDebug() << "In getMoviesByPeople():";
        qDebug() << l_query.lastError().text();
    }

    while(l_query.next())
    {
        Movie l_movie = hydrateMovie(l_query);

        l_movieList.push_back(l_movie);
    }

    return l_movieList;
}

/**
 * @brief Gets all the movies directed by people having the id `id` and of type `type`
 *
 * @param People director
 * @param int type of the people
 * @param QString upon which field we order the request
 * @return QList<Movie>
 */
QList<Movie> DatabaseManager::getMoviesByPeople(const People &people,
                                                const int type,
                                                const QString fieldOrder)
{
    QList<Movie> l_movieList = getMoviesByPeople(people.getId(), type, fieldOrder);

    return l_movieList;
}

/**
 * @brief Gets all the movies tagged by the tag having the id `id`
 *
 * @param Tag tag
 * @return QList<Movie>
 */
QList<Movie> DatabaseManager::getMoviesByTag(const int id,
                                             const QString fieldOrder)
{
    QList<Movie> l_movieList;
    QSqlQuery l_query(m_db);
    l_query.prepare("SELECT " + m_movieFields + " "
                    "FROM movies AS m"
                    "WHERE id IN (SELECT id_movie "
                                 "FROM movies_tags "
                                 "WHERE id_tag = :id) "
                                 "ORDER BY " + fieldOrder);
    l_query.bindValue(":id", id);

    if (!l_query.exec())
    {
        qDebug() << "In getMoviesByTag(Tag):";
        qDebug() << l_query.lastError().text();
    }

    while(l_query.next())
    {
        Movie l_movie = hydrateMovie(l_query);
        l_movieList.push_back(l_movie);
    }

    return l_movieList;
}

/**
 * @brief Gets all the movies tagged by 'tag'
 *
 * @param Tag tag
 * @return QList<Movie>
 */
QList<Movie> DatabaseManager::getMoviesByTag(const Tag &tag,
                                             const QString fieldOrder)
{
    QList<Movie> l_movieList = getMoviesByTag(tag.getId(), fieldOrder);

    return l_movieList;
}

QList<Movie> DatabaseManager::getMoviesWithoutPeople(const int type,
                                                     const QString fieldOrder)
{
    QList<Movie> l_movieList;
    QSqlQuery l_query(m_db);
    l_query.prepare("SELECT " + m_movieFields + " "
                    "FROM movies AS m "
                    "WHERE (SELECT COUNT(*) "
                                "FROM movies_people AS mp "
                                "WHERE mp.id_movie = m.id AND mp.type = :type) = 0 "
                    "ORDER BY " + fieldOrder);
    l_query.bindValue(":type", type);

    if (!l_query.exec())
    {
        qDebug() << "In getMoviesWithoutPeople():";
        qDebug() << l_query.lastError().text();
    }

    while(l_query.next())
    {
        Movie l_movie = hydrateMovie(l_query);
        l_movieList.push_back(l_movie);
    }

    return l_movieList;
}

QList<Movie> DatabaseManager::getMoviesWithoutTag(const QString fieldOrder)
{
    QList<Movie> l_movieList;
    QSqlQuery l_query(m_db);
    l_query.prepare("SELECT " + m_movieFields + " "
                    "FROM movies AS m "
                    "WHERE (SELECT COUNT(*) "
                                "FROM movies_tags AS mt "
                                "WHERE mt.id_movie = m.id) = 0 "
                    "ORDER BY " + fieldOrder);

    if (!l_query.exec())
    {
        qDebug() << "In getMoviesWithoutTag():";
        qDebug() << l_query.lastError().text();
    }

    while(l_query.next())
    {
        Movie l_movie = hydrateMovie(l_query);
        l_movieList.push_back(l_movie);
    }

    return l_movieList;
}

QList<Movie> DatabaseManager::getMoviesByAny(const QString text,
                                             const QString fieldOrder)
{
    QList<Movie> l_movieList;
    QSqlQuery l_query(m_db);
    QStringList l_splittedText = text.split(" ");

    QString l_queryText = "SELECT " + m_movieFields + " FROM movies AS m WHERE ";
    for( int i = 0 ; i < l_splittedText.size() ; i++)
    {
        if (i != 0)
        {
            l_queryText = l_queryText+ "AND ";
        }
        l_queryText = l_queryText
                + "("
                     "("
                           "m.title LIKE '%'||:text"+QString::number(i)+"||'%' OR m.original_title LIKE '%'||:text"+ QString::number(i) +"||'%' "
                      ") "
                  "OR ( "
                            "SELECT COUNT(*) "
                            "FROM people AS p "
                            "WHERE ( p.lastname LIKE '%'||:text"+ QString::number(i) +"||'%' "
                                "OR p.firstname LIKE '%'||:text"+ QString::number(i) +"||'%' ) "
                                "AND ( SELECT COUNT(*) FROM movies_people WHERE id_people = p.id AND id_movie = m.id) > 0 "
                     ") > 0 "
                  "OR ( "
                            "SELECT COUNT(*) "
                            "FROM tags AS t "
                            "WHERE ( t.name LIKE '%'||:text"+ QString::number(i) +"||'%' ) "
                            "AND ( SELECT COUNT(*) FROM movies_tags WHERE id_tag = t.id AND id_movie = m.id) > 0 "
                     ") > 0 "
                  ") ";
    }
    l_queryText = l_queryText+ "ORDER BY m." + fieldOrder;
    l_query.prepare(l_queryText);
    for( int i = 0 ; i < l_splittedText.size() ; i++)
    {
        l_query.bindValue(":text"+ QString::number(i), l_splittedText.at(i));
    }

    if (!l_query.exec())
    {
        qDebug() << "In getMoviesByAny():";
        qDebug() << l_query.lastError().text();
    }

    while(l_query.next())
    {
        Movie l_movie = hydrateMovie(l_query);
        l_movieList.push_back(l_movie);
    }

    return l_movieList;
}

/**
 * @brief Gets the one person that has the id `id`
 *
 * @param int id of the person
 * @return People
 */
People DatabaseManager::getOnePeopleById(const int id)
{
    People l_people;
    QSqlQuery l_query(m_db);

    l_query.prepare("SELECT " + m_peopleFields + " "
                    "FROM people AS p "
                    "WHERE p.id = :id ");
    l_query.bindValue(":id", id);

    if (!l_query.exec())
    {
        qDebug() << "In getOnePeopleBy(int, type):";
        qDebug() << l_query.lastError().text();
    }

    if(l_query.next())
    {
        l_people = hydratePeople(l_query);
    }

    return l_people;
}

/**
 * @brief Gets the one person that has the id `id`
 *
 * @param int id of the person
 * @param int type of the person
 * @return People
 */
People DatabaseManager::getOnePeopleById(const int id, const int type)
{
    People l_people;
    QSqlQuery l_query(m_db);

    l_query.prepare("SELECT " + m_peopleFields + " "
                    "FROM people AS p, movies_people AS pm "
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
        l_people = hydratePeople(l_query);
    }

    return l_people;
}

/**
 * @brief Gets all the people of type `type`
 *
 * @param int type
 * @param QString fieldOrder
 * @return QList<People>
 */
QList<People> DatabaseManager::getPeopleByType(const int type,
                                            const QString fieldOrder)
{
    QList<People> l_peopleList;
    QSqlQuery l_query(m_db);

    l_query.prepare("SELECT " + m_peopleFields + " "
                    "FROM people AS p "
                    "WHERE id IN (SELECT id_people FROM movies_people WHERE type = :type) "
                    "ORDER BY " + fieldOrder);
    l_query.bindValue(":type", type);

    if (!l_query.exec())
    {
        qDebug() << "In getPeopleByType():";
        qDebug() << l_query.lastError().text();
    }

    while(l_query.next())
    {
        People l_people = hydratePeople(l_query);
        l_peopleList.push_back(l_people);
    }

    debug("[DatabaseManager] getPeopleByType returns "
          + QString::number(l_peopleList.count()) + " people");

    return l_peopleList;
}

/**
 * @brief Gets the people whose 'lastname firstname' or 'firstname lastname' is `fullname`
 *
 * @param QString fullname, the string searched
 * @return QList<People>
 */
QList<People> DatabaseManager::getPeopleByFullname(const QString fullname,
                                                   const QString fieldOrder)
{
    QList<People> l_peopleList;
    QSqlQuery l_query(m_db);

    l_query.prepare("SELECT " + m_peopleFields + " "
                    "FROM people AS p "
                    "WHERE p.lastname || ' ' || p.firstname LIKE '%'||:fullname||'%' "
                       "OR p.firstname || ' ' || p.lastname LIKE '%'||:fullname||'%' "
                    "ORDER BY " + fieldOrder);
    l_query.bindValue(":fullname", fullname);

    if (!l_query.exec())
    {
        qDebug() << "In getPeopleByFullname(QString):";
        qDebug() << l_query.lastError().text();
    }

    while(l_query.next())
    {
        People l_people = hydratePeople(l_query);
        l_peopleList.push_back(l_people);
    }

    debug("[DatabaseManager] getPeopleByFullname returns "
          + QString::number(l_peopleList.count()) + " people");

    return l_peopleList;
}

QList<People> DatabaseManager::getPeopleByAny(QString text, int type, QString fieldOrder)
{
    debug("[DatabaseManager] Enters getOnePeopleByAny");
    QList<People> l_peopleList;
    QSqlQuery l_query(m_db);
    QStringList l_splittedText = text.split(" ");

    QString l_queryText = "SELECT " + m_peopleFields + " FROM people AS p WHERE ";
    for( int i = 0 ; i < l_splittedText.size() ; i++)
    {
        if (i != 0)
        {
            l_queryText += "AND ";
        }
    l_queryText = l_queryText
            + "("
                    "("
                           "p.lastname LIKE '%'||:text"+ QString::number(i) +"||'%' "
                        "OR p.firstname LIKE '%'||:text"+ QString::number(i) +"||'%'"
                    ") "
                 "OR ( "
                      "SELECT COUNT(*) "
                      "FROM movies AS m "
                      "WHERE (m.title LIKE '%'||:text"+ QString::number(i) +"||'%' OR m.original_title LIKE '%'||:text"+ QString::number(i) +"||'%' ) "
                             "AND ( SELECT COUNT(*) FROM movies_people WHERE id_people = p.id AND id_movie = m.id ) > 0 "
                     ") > 0 "
                 "OR ( "
                      "SELECT COUNT(*) "
                      "FROM movies AS m, tags AS t "
                      "WHERE (t.name LIKE '%'||:text"+ QString::number(i) +"||'%' ) "
                            "AND ( SELECT COUNT(*) FROM movies_people WHERE id_people = p.id AND id_movie = m.id ) > 0 "
                            "AND ( SELECT COUNT(*) FROM movies_tags WHERE id_tag = t.id AND id_movie = m.id ) > 0 "
                    ") > 0 "
               ") ";
    }
    l_queryText = l_queryText + "AND (SELECT COUNT(*) FROM movies_people AS mp WHERE mp.type = :type) > 0 "
                                "ORDER BY p." + fieldOrder;
    l_query.prepare(l_queryText);

    for( int i = 0 ; i < l_splittedText.size() ; i++)
    {
        l_query.bindValue(":text"+ QString::number(i), l_splittedText.at(i));
    }
    l_query.bindValue(":type", type);

    if (!l_query.exec())
    {
        qDebug() << "In getPeopleByAny():";
        qDebug() << l_query.lastError().text();
    }

    while(l_query.next())
    {
        People l_people = hydratePeople(l_query);
        l_peopleList.push_back(l_people);
    }

    debug("[DatabaseManager] getPeopleByAny returns "
          + QString::number(l_peopleList.count()) + " people");

    return l_peopleList;
}

/**
 * @brief Gets the tag which id is `id`
 *
 * @param int id
 * @return Tag
 */
Tag DatabaseManager::getOneTagById(const int id)
{
    debug("[DatabaseManager] Enters getOneTagById");
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
 * @brief Gets the first tag with name 'tagName'
 *
 * @param QString tagName
 * @return Tag
 */
Tag DatabaseManager::getTagByName(QString tagName)
{
    Tag l_tag;
    QSqlQuery l_query(m_db);
    l_query.prepare("SELECT id, name "
                    "FROM tags "
                    "WHERE name = :name");
    l_query.bindValue(":name", tagName);

    if (!l_query.exec())
    {
        qDebug() << "In getTagByName(QString):";
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
 * @brief Gets all the tags
 *
 * @return QList<Tag>
 */
QList<Tag> DatabaseManager::getAllTags(const QString fieldOrder)
{
    debug("[DatabaseManager] Enters getAllTags");
    QList<Tag> l_tagList;
    QSqlQuery l_query(m_db);
    l_query.prepare("SELECT id, name "
                    "FROM tags "
                    "ORDER BY " + fieldOrder);

    if (!l_query.exec())
    {
        qDebug() << l_query.lastError().text();
    }

    while(l_query.next())
    {
        Tag l_tag;
        l_tag.setId(l_query.value(0).toInt());
        l_tag.setName(l_query.value(1).toString());
        l_tagList.push_back(l_tag);
    }

    debug("[DatabaseManager] getAllTags returns "
          + QString::number(l_tagList.count()) + " tags");

    return l_tagList;
}

QList<Tag> DatabaseManager::getTagsByAny(const QString text, const QString fieldOrder)
{
    debug("[DatabaseManager] Enters getTagsByAny");
    QList<Tag> l_tagList;
    QSqlQuery l_query(m_db);
    QStringList l_splittedText = text.split(" ");

    QString l_queryText = "SELECT " + m_tagFields + " FROM tags AS t WHERE ";
    for (int i = 0 ; i < l_splittedText.size() ; i++)
    {
        if (i != 0)
        {
            l_queryText += "AND ";
        }
    l_queryText = l_queryText
            + "("
                    "("
                       "t.name LIKE '%'||:text"+ QString::number(i) +"||'%' "
                    ") "
                 "OR ( "
                      "SELECT COUNT(*) "
                      "FROM movies AS m "
                      "WHERE (m.title LIKE '%'||:text"+ QString::number(i) +"||'%' OR m.original_title LIKE '%'||:text"+ QString::number(i) +"||'%' ) "
                             "AND ( SELECT COUNT(*) FROM movies_tags WHERE id_tag = t.id AND id_movie = m.id ) > 0 "
                     ") > 0 "
                 "OR ( "
                      "SELECT COUNT(*) "
                      "FROM movies AS m, people AS p "
                      "WHERE (  p.lastname LIKE '%'||:text"+ QString::number(i) +"||'%' "
                            "OR p.firstname LIKE '%'||:text"+ QString::number(i) +"||'%' )"
                        "AND ( SELECT COUNT(*) FROM movies_people WHERE id_people = p.id AND id_movie = m.id ) > 0 "
                        "AND ( SELECT COUNT(*) FROM movies_tags WHERE id_tag = t.id AND id_movie = m.id ) > 0 "
                    ") > 0 "
               ") ";
    }
    l_queryText = l_queryText + "ORDER BY t." + fieldOrder;
    l_query.prepare(l_queryText);

    for( int i = 0 ; i < l_splittedText.size() ; i++)
    {
        l_query.bindValue(":text"+ QString::number(i), l_splittedText.at(i));
    }

    if (!l_query.exec())
    {
        qDebug() << "In getTagsByAny():";
        qDebug() << l_query.lastError().text();
    }

    while(l_query.next())
    {
        Tag l_tag = hydrateTag(l_query);
        l_tagList.push_back(l_tag);
    }

    debug("[DatabaseManager] getTagsByAny returns "
          + QString::number(l_tagList.count()) + " tags");

    return l_tagList;
}

/**
 * @brief Get the playlist having the id `id`
 *
 * @param int id of the playlist
 * @return Playlist
 */
Playlist DatabaseManager::getOnePlaylistById(const int id)
{
    Playlist l_playlist;
    QSqlQuery l_query(m_db);
    l_query.prepare("SELECT pl.id, pl.name, pl.rate, pl.creation_date "
                    "FROM playlists AS pl "
                    "WHERE pl.id = :id");
    l_query.bindValue(":id", id);

    if(!l_query.exec())
    {
        qDebug() << "In getOnePlaylistById():";
        qDebug() << l_query.lastError().text();
    }

    if(l_query.next())
    {
        l_playlist = hydratePlaylist(l_query);
    }

    return l_playlist;
}

/**
 * @brief Get the playlists in the database
 *
 * @return QList<Playlist>
 */
QList<Playlist> DatabaseManager::getAllPlaylists(QString fieldOrder)
{
    QList<Playlist> l_playlistList;
    QSqlQuery l_query(m_db);
    l_query.prepare("SELECT pl.id, pl.name, pl.rate, pl.creation_date "
                    "FROM playlists AS pl "
                    "WHERE pl.id != 1 "
                    "ORDER BY pl." +fieldOrder);

    if(!l_query.exec())
    {
        qDebug() << "In getAllPlaylist():";
        qDebug() << l_query.lastError().text();
    }

    while(l_query.next())
    {
        Playlist l_playlist = hydratePlaylist(l_query);
        l_playlistList.append(l_playlist);
    }

    return l_playlistList;
}

/**
 * @brief Retuns whether a movie is known by the database or not
 *
 * @param QString path of the movie
 * @return bool
 */
bool DatabaseManager::existMovie(const QString filePath)
{
    QSqlQuery l_query(m_db);
    l_query.prepare("SELECT id FROM movies WHERE file_path = :file_path ");
    l_query.bindValue(":file_path", filePath);

    if (!l_query.exec())
    {
        qDebug() << "In existMovie():";
        qDebug() << l_query.lastError().text();
    }

    return l_query.next();
}

/**
 * @brief Retuns whether a person is known by the database or not
 *
 * @param QString fullname of the person
 * @return bool
 */
bool DatabaseManager::existPeople(const QString fullname)
{
    QSqlQuery l_query(m_db);
    l_query.prepare("SELECT id FROM people AS p "
                    "WHERE p.lastname || ' ' || p.firstname  = :fullname "
                    "OR p.firstname || ' ' || p.lastname = :fullname");
    l_query.bindValue(":fullname", fullname);

    if (!l_query.exec())
    {
        qDebug() << "In existPeople():";
        qDebug() << l_query.lastError().text();
    }

    return l_query.next();
}

/**
 * @brief Retuns whether a tag is known by the database or not
 *
 * @param QString name of the tag
 * @return bool
 */
bool DatabaseManager::existTag(const QString name)
{
    QSqlQuery l_query(m_db);
    l_query.prepare("SELECT id FROM tags WHERE name = :name ");
    l_query.bindValue(":name", name);

    if (!l_query.exec())
    {
        qDebug() << "In existTag():";
        qDebug() << l_query.lastError().text();
    }

    return l_query.next();
}

/**
 * @brief Gets the people of a movie and adds it to the object
 * @param Movie
 */
void DatabaseManager::setPeopleToMovie(Movie &movie)
{
    QSqlQuery l_query(m_db);
    l_query.prepare("SELECT " + m_peopleFields + ", pm.type "
                    "FROM people AS p, movies_people AS pm "
                    "WHERE pm.id_movie = :id_movie AND pm.id_people = p.id");
    l_query.bindValue(":id_movie", movie.getId());

    if (!l_query.exec())
    {
        qDebug() << "In setPeopleToMovie(Movie):";
        qDebug() << l_query.lastError().text();
    }
    while (l_query.next())
    {
        People l_people = hydratePeople(l_query);
        movie.addPeople(l_people);

    }
}

/**
 * @brief Gets the tags of a movie and adds it to the object
 * @param Movie
 */
void DatabaseManager::setTagsToMovie(Movie &movie)
{
    QSqlQuery l_query(m_db);
    l_query.prepare("SELECT tag.id, tag.name "
                    "FROM tags AS tag, movies_tags AS tm "
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
 * @brief Gets the movies of a playlist and adds it to the object
 * @param Playlist
 */
void DatabaseManager::setMoviesToPlaylist(Playlist &playlist)
{
    QSqlQuery l_query(m_db);
    l_query.prepare("SELECT " +m_movieFields + " "
                    "FROM movies AS m, movies_playlists AS plm "
                    "WHERE plm.id_movie = m.id AND plm.id_playlist = :id_playlist");
    l_query.bindValue(":id_playlist", playlist.getId());

    if (!l_query.exec())
    {
        qDebug() << "In setMoviesToPlaylist():";
        qDebug() << l_query.lastError().text();
    }
    while (l_query.next())
    {
        Movie l_movie = hydrateMovie(l_query);
        playlist.addMovie(l_movie);
    }
}

/**
 * @brief Hydrates a movie from the database
 *
 * @param QSqlQuery containing the data
 * @return Movie hydrated object
 */
Movie DatabaseManager::hydrateMovie(QSqlQuery &query)
{
    Movie l_movie;
    l_movie.setId(query.value(0).toInt());
    l_movie.setTitle(query.value(1).toString());
    l_movie.setOriginalTitle(query.value(2).toString());
    l_movie.setReleaseDate(QDate::fromString(query.value(3).toString(), DATE_FORMAT));
    l_movie.setCountry(query.value(4).toString());
    l_movie.setDuration(QTime::fromMSecsSinceStartOfDay(query.value(5).toInt()));
    l_movie.setSynopsis(query.value(6).toString());
    l_movie.setFilePath(query.value(7).toString());
    l_movie.setColored(query.value(8).toBool());
    l_movie.setFormat(query.value(9).toString());
    l_movie.setSuffix(query.value(10).toString());
    l_movie.setRank(query.value(11).toInt());
    setTagsToMovie(l_movie);
    setPeopleToMovie(l_movie);

    return l_movie;
}

/**
 * @brief Hydrates a person from the database
 *
 * @param QSqlQuery containing the data
 * @return People hydrated object
 */
People DatabaseManager::hydratePeople(QSqlQuery &query)
{
    People l_people;
    l_people.setId(query.value(0).toInt());
    l_people.setFirstname(query.value(1).toString());
    l_people.setLastname(query.value(2).toString());
    l_people.setRealname(query.value(3).toString());
    l_people.setBirthday(QDate::fromString(query.value(4).toString(), DATE_FORMAT));
    l_people.setBiography(query.value(5).toString());
    if (query.value(6).isValid())
    {
        l_people.setType(query.value(6).toInt());
    }

    return l_people;
}

/**
 * @brief Hydrates a tag from the database
 *
 * @param QSqlQuery containing the data
 * @return Tag hydrated object
 */
Tag DatabaseManager::hydrateTag(QSqlQuery &query)
{
    Tag l_tag;
    l_tag.setId(query.value(0).toInt());
    l_tag.setName(query.value(1).toString());

    return l_tag;
}

/**
 * @brief Hydrates a playlist from the database
 *
 * @param QSqlQuery containing the data
 * @return Playlist hydrated object
 */
Playlist DatabaseManager::hydratePlaylist(QSqlQuery &query)
{
    Playlist l_playlist;
    l_playlist.setId(query.value(0).toInt());
    l_playlist.setName(query.value(1).toString());
    l_playlist.setRate(query.value(2).toInt());
    l_playlist.setCreationDate(QDateTime::fromTime_t(query.value(3).toInt()));
    setMoviesToPlaylist(l_playlist);

    return l_playlist;
}
