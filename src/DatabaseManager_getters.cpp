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
        Macaw::DEBUG("In getOneMovieById(int):");
        Macaw::DEBUG(l_query.lastError().text());
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
        Macaw::DEBUG("In getAllMovies():");
        Macaw::DEBUG(l_query.lastError().text());
    }

    while(l_query.next())
    {
        Movie l_movie = hydrateMovie(l_query);
        l_movieList.append(l_movie);
    }

    return l_movieList;

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
        Macaw::DEBUG("In getMoviesByPeople():");
        Macaw::DEBUG(l_query.lastError().text());
    }

    while(l_query.next())
    {
        Movie l_movie = hydrateMovie(l_query);
        l_movieList.append(l_movie);
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
    QList<Movie> l_movieList = getMoviesByPeople(people.id(), type, fieldOrder);

    return l_movieList;
}

/**
 * @brief Gets all the movies tagged by the tag having the id `id`
 *
 * @param id id
 * @return QList<Movie>
 */
QList<Movie> DatabaseManager::getMoviesByTag(const int id,
                                             const QString fieldOrder)
{
    QList<Movie> l_movieList;
    QSqlQuery l_query(m_db);
    l_query.prepare("SELECT " + m_movieFields + " "
                    "FROM movies AS m "
                    "WHERE id IN (SELECT id_movie "
                                 "FROM movies_tags "
                                 "WHERE id_tag = :id) "
                                 "ORDER BY " + fieldOrder);
    l_query.bindValue(":id", id);

    if (!l_query.exec())
    {
        Macaw::DEBUG("In getMoviesByTag(Tag):");
        Macaw::DEBUG(l_query.lastError().text());
    }

    while(l_query.next())
    {
        Movie l_movie = hydrateMovie(l_query);
        l_movieList.append(l_movie);
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
    QList<Movie> l_movieList = getMoviesByTag(tag.id(), fieldOrder);

    return l_movieList;
}

/**
 * @brief Gets all the movies from a playlist determined by its id
 *
 * @param int id
 * @return QList<Movie>
 */
QList<Movie> DatabaseManager::getMoviesByPlaylist(const int id,
                                                  const QString fieldOrder)
{
    QList<Movie> l_movieList;
    QSqlQuery l_query(m_db);
    l_query.prepare("SELECT " + m_movieFields + " "
                    "FROM movies AS m "
                    "WHERE m.id IN (SELECT id_movie "
                                 "FROM movies_playlists "
                                 "WHERE id_playlist = :id) "
                                 "ORDER BY " + fieldOrder);
    l_query.bindValue(":id", id);

    if (!l_query.exec())
    {
        Macaw::DEBUG("In getMoviesByPlaylist(Playlist):");
        Macaw::DEBUG(l_query.lastError().text());
    }

    while(l_query.next())
    {
        Movie l_movie = hydrateMovie(l_query);
        l_movieList.append(l_movie);
    }

    return l_movieList;
}

/**
 * @brief Gets all the movies from a playlist
 *
 * @param Playlist
 * @return QList<Movie>
 */
QList<Movie> DatabaseManager::getMoviesByPlaylist(const Playlist &playlist,
                                             const QString fieldOrder)
{
    QList<Movie> l_movieList = getMoviesByPlaylist(playlist.id(), fieldOrder);

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
        Macaw::DEBUG("In getMoviesWithoutPeople():");
        Macaw::DEBUG(l_query.lastError().text());
    }

    while(l_query.next())
    {
        Movie l_movie = hydrateMovie(l_query);
        l_movieList.append(l_movie);
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
        Macaw::DEBUG("In getMoviesWithoutTag():");
        Macaw::DEBUG(l_query.lastError().text());
    }

    while(l_query.next())
    {
        Movie l_movie = hydrateMovie(l_query);
        l_movieList.append(l_movie);
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
                            "WHERE p.name LIKE '%'||:text"+ QString::number(i) +"||'%' "
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
        Macaw::DEBUG("In getMoviesByAny():");
        Macaw::DEBUG(l_query.lastError().text());
    }

    while(l_query.next())
    {
        Movie l_movie = hydrateMovie(l_query);
        l_movieList.append(l_movie);
    }

    return l_movieList;
}

QList<Movie> DatabaseManager::getMoviesNotImported(const QString fieldOrder)
{
    QList<Movie> l_movieList;
    QSqlQuery l_query(m_db);
    l_query.prepare("SELECT " + m_movieFields + " "
                    "FROM movies AS m "
                    "WHERE m.imported = :imported "
                    "ORDER BY " + fieldOrder);
    l_query.bindValue(":imported", false);

    if (!l_query.exec())
    {
        Macaw::DEBUG("In getMoviesNotImported():");
        Macaw::DEBUG(l_query.lastError().text());
    }

    while(l_query.next())
    {
        Movie l_movie = hydrateMovie(l_query);
        l_movieList.append(l_movie);
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
        Macaw::DEBUG("In getOnePeopleById(int):");
        Macaw::DEBUG(l_query.lastError().text());
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
        Macaw::DEBUG("In getOnePeopleById(int, type):");
        Macaw::DEBUG(l_query.lastError().text());
    }

    if(l_query.next())
    {
        l_people = hydratePeople(l_query);
    }

    return l_people;
}

/**
 * @brief Gets the one person that has the name `name`
 *
 * @param QString name of the person
 * @return People
 */
People DatabaseManager::getOnePeopleByName(const QString name)
{
    People l_people;
    QSqlQuery l_query(m_db);

    l_query.prepare("SELECT " + m_peopleFields + " "
                    "FROM people AS p "
                    "WHERE p.name = :name ");
    l_query.bindValue(":name", name);

    if (!l_query.exec())
    {
        Macaw::DEBUG("In getOnePeopleByName():");
        Macaw::DEBUG(l_query.lastError().text());
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
QList<People> DatabaseManager::getPeopleUsedByType(const int type,
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
        Macaw::DEBUG("In getPeopleUsedByType():");
        Macaw::DEBUG(l_query.lastError().text());
    }

    while(l_query.next())
    {
        People l_people = hydratePeople(l_query);
        l_peopleList.append(l_people);
    }

    Macaw::DEBUG("[DatabaseManager] peopleByType returns "
          + QString::number(l_peopleList.count()) + " people");

    return l_peopleList;
}

/**
 * @brief Gets the people whose name contains 'name'
 *
 * @param QString name, the string searched
 * @return QList<People>
 */
QList<People> DatabaseManager::getPeopleByName(const QString name,
                                               const QString fieldOrder)
{
    QList<People> l_peopleList;
    QSqlQuery l_query(m_db);

    l_query.prepare("SELECT " + m_peopleFields + " "
                    "FROM people AS p "
                    "WHERE p.name LIKE '%'||:name||'%' "
                    "ORDER BY " + fieldOrder);
    l_query.bindValue(":name", name);

    if (!l_query.exec())
    {
        Macaw::DEBUG("In peopleByName(QString):");
        Macaw::DEBUG(l_query.lastError().text());
    }

    while(l_query.next())
    {
        People l_people = hydratePeople(l_query);
        l_peopleList.append(l_people);
    }

    Macaw::DEBUG("[DatabaseManager] peopleByName returns "
          + QString::number(l_peopleList.count()) + " people");

    return l_peopleList;
}

QList<People> DatabaseManager::getPeopleByAny(QString text, int type, QString fieldOrder)
{
    Macaw::DEBUG("[DatabaseManager] Enters getPeopleByAny");
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
                    "p.name LIKE '%'||:text"+ QString::number(i) +"||'%' "
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
        Macaw::DEBUG("In peopleByAny():");
        Macaw::DEBUG(l_query.lastError().text());
    }

    while(l_query.next())
    {
        People l_people = hydratePeople(l_query);
        l_peopleList.append(l_people);
    }

    Macaw::DEBUG("[DatabaseManager] peopleByAny returns "
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
    Macaw::DEBUG("[DatabaseManager] Enters getOneTagById");
    Tag l_tag;
    QSqlQuery l_query(m_db);
    l_query.prepare("SELECT id, name "
                    "FROM tags "
                    "WHERE id = :id");
    l_query.bindValue(":id", id);

    if (!l_query.exec())
    {
        Macaw::DEBUG("In getOneTagBy(int):");
        Macaw::DEBUG(l_query.lastError().text());
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
Tag DatabaseManager::getOneTagByName(QString tagName)
{
    Tag l_tag;
    QSqlQuery l_query(m_db);
    l_query.prepare("SELECT id, name "
                    "FROM tags "
                    "WHERE name = :name");
    l_query.bindValue(":name", tagName);

    if (!l_query.exec())
    {
        Macaw::DEBUG("In tagByName(QString):");
        Macaw::DEBUG(l_query.lastError().text());
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
    Macaw::DEBUG("[DatabaseManager] Enters getAllTags");
    QList<Tag> l_tagList;
    QSqlQuery l_query(m_db);
    l_query.prepare("SELECT id, name "
                    "FROM tags "
                    "ORDER BY " + fieldOrder);

    if (!l_query.exec())
    {
        Macaw::DEBUG("In getAllTags():");
        Macaw::DEBUG(l_query.lastError().text());
    }

    while(l_query.next())
    {
        Tag l_tag;
        l_tag.setId(l_query.value(0).toInt());
        l_tag.setName(l_query.value(1).toString());
        l_tagList.append(l_tag);
    }

    Macaw::DEBUG("[DatabaseManager] getAllTags returns "
          + QString::number(l_tagList.count()) + " tags");

    return l_tagList;
}

/**
 * @brief Gets the tags which are used
 *
 * @return QList<Tag>
 */
QList<Tag> DatabaseManager::getTagsUsed(const QString fieldOrder)
{
    Macaw::DEBUG("[DatabaseManager] Enters getTagsUsed");
    QList<Tag> l_tagList;
    QSqlQuery l_query(m_db);
    l_query.prepare("SELECT " +m_tagFields+ " "
                    "FROM tags AS t "
                    "WHERE ( "
                        "SELECT COUNT (*) "
                        "FROM movies_tags "
                        "WHERE id_tag = t.id "
                    " ) > 0 "
                    "ORDER BY " + fieldOrder);

    if (!l_query.exec())
    {
        Macaw::DEBUG(m_tagFields);
        Macaw::DEBUG("In getTagsUsed():");
        Macaw::DEBUG(l_query.lastError().text());
    }

    while(l_query.next())
    {
        Tag l_tag;
        l_tag.setId(l_query.value(0).toInt());
        l_tag.setName(l_query.value(1).toString());
        l_tagList.append(l_tag);
    }

    Macaw::DEBUG("[DatabaseManager] getTagsUsed returns "
          + QString::number(l_tagList.count()) + " tags");

    return l_tagList;
}

QList<Tag> DatabaseManager::getTagsByAny(const QString text, const QString fieldOrder)
{
    Macaw::DEBUG("[DatabaseManager] Enters tagsByAny");
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
                      "WHERE p.name LIKE '%'||:text"+ QString::number(i) +"||'%' "
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
        Macaw::DEBUG("In tagsByAny():");
        Macaw::DEBUG(l_query.lastError().text());
    }

    while(l_query.next())
    {
        Tag l_tag = hydrateTag(l_query);
        l_tagList.append(l_tag);
    }

    Macaw::DEBUG("[DatabaseManager] tagsByAny returns "
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
        Macaw::DEBUG("In getOnePlaylistById():");
        Macaw::DEBUG(l_query.lastError().text());
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
        Macaw::DEBUG("In getAllPlaylist():");
        Macaw::DEBUG(l_query.lastError().text());
    }

    while(l_query.next())
    {
        Playlist l_playlist = hydratePlaylist(l_query);
        l_playlistList.append(l_playlist);
    }

    return l_playlistList;
}

bool DatabaseManager::isMovieInPlaylist(int movieId, int playlistId)
{
    QSqlQuery l_query(m_db);
    l_query.prepare("SELECT id "
                    "FROM movies_playlists "
                    "WHERE id_movie = :id_movie "
                        "AND id_playlist = :id_playlist");
    l_query.bindValue(":id_movie", movieId);
    l_query.bindValue(":id_playlist", playlistId);

    if (!l_query.exec())
    {
        Macaw::DEBUG("In isMovieInPlaylist():");
        Macaw::DEBUG(l_query.lastError().text());
    }

    return l_query.next();
}

bool DatabaseManager::isMovieInPlaylist(Movie &movie, int playlistId)
{
    return isMovieInPlaylist(movie.id(), playlistId);
}

bool DatabaseManager::isMovieInPlaylist(Movie &movie, Playlist &playlist)
{
    return isMovieInPlaylist(movie.id(), playlist.id());
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
        Macaw::DEBUG("In existMovie():");
        Macaw::DEBUG(l_query.lastError().text());
    }

    return l_query.next();
}

/**
 * @brief Retuns whether a person is known by the database or not
 *
 * @param QString fullname of the person
 * @return bool
 */
bool DatabaseManager::existPeople(const QString name)
{
    QSqlQuery l_query(m_db);
    l_query.prepare("SELECT id FROM people AS p "
                    "WHERE p.name = :name");
    l_query.bindValue(":name", name);

    if (!l_query.exec())
    {
        Macaw::DEBUG("In existPeople():");
        Macaw::DEBUG(l_query.lastError().text());
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
        Macaw::DEBUG("In existTag():");
        Macaw::DEBUG(l_query.lastError().text());
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
    l_query.bindValue(":id_movie", movie.id());

    if (!l_query.exec())
    {
        Macaw::DEBUG("In setPeopleToMovie(Movie):");
        Macaw::DEBUG(l_query.lastError().text());
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
    l_query.prepare("SELECT " +m_tagFields+ " "
                    "FROM tags AS t, movies_tags AS tm "
                    "WHERE tm.id_movie = :id_movie AND tm.id_tag = t.id");
    l_query.bindValue(":id_movie", movie.id());

    if (!l_query.exec())
    {
        Macaw::DEBUG("In setTagToMovie(Movie):");
        Macaw::DEBUG(l_query.lastError().text());
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
    l_query.bindValue(":id_playlist", playlist.id());

    if (!l_query.exec())
    {
        Macaw::DEBUG("In setMoviesToPlaylist():");
        Macaw::DEBUG(l_query.lastError().text());
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
    l_movie.setPosterPath(query.value(8).toString());
    l_movie.setColored(query.value(9).toBool());
    l_movie.setFormat(query.value(10).toString());
    l_movie.setSuffix(query.value(11).toString());
    l_movie.setRank(query.value(12).toInt());
    l_movie.setImported(query.value(13).toBool());
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
    l_people.setName(query.value(1).toString());
    l_people.setBirthday(QDate::fromString(query.value(2).toString(), DATE_FORMAT));
    l_people.setBiography(query.value(3).toString());
    if (query.value(4).isValid())
    {
        l_people.setType(query.value(4).toInt());
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
