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
 * @brief Remove a Movie from the database
 *
 * @param Movie to remove
 * @return boolean
 */
bool DatabaseManager::deleteMovie(Movie &movie)
{
    foreach(People l_people, movie.getPeopleList())
    {
        if (!removePeopleFromMovie(l_people, movie, l_people.getType()))
        {
            return false;
        }
    }
    foreach(Tag l_tag, movie.getTagList())
    {
        if (!removeTagFromMovie(l_tag, movie))
        {
            return false;
        }
    }
    QList<Playlist> l_playlistList;
    foreach(Playlist l_playlist, l_playlistList)
    {
        if (!removeMovieFromPlaylist(movie, l_playlist))
        {
            return false;
        }
    }

    QSqlQuery l_query(m_db);
    l_query.prepare("DELETE FROM movies WHERE id = :id");
    l_query.bindValue(":id", movie.getId());

    if(!l_query.exec())
    {
        qDebug() << "In deleteMovie():";
        qDebug() << l_query.lastError().text();

        return false;
    }

    return true;
}

/**
 * @brief Removes the link between a person and a movie
 * If there is no more link with the person, it is deleted
 *
 * @param People to remove
 * @param Movie concerned by the deletion
 * @param int type of the person to remove
 * @return boolean
 */
bool DatabaseManager::removePeopleFromMovie(People &people,
                                            Movie &movie,
                                            const int type)
{
    QSqlQuery l_query(m_db);
    l_query.prepare("DELETE FROM movies_people "
                   "WHERE id_people = :id_people "
                     "AND id_movie = :id_movie "
                     "AND type = :type");
    l_query.bindValue(":id_people", people.getId());
    l_query.bindValue(":id_movie", movie.getId());
    l_query.bindValue(":type", type);

    if(!l_query.exec())
    {
        qDebug() << "In removePeopleFromMovie():";
        qDebug() << l_query.lastError().text();

        return false;
    }

    // Checks if this people is still used, if not deletes it.
    l_query.prepare("SELECT id FROM movies_people WHERE id_people = :id_people");
    l_query.bindValue(":id_people", people.getId());
    if(!l_query.exec())
    {
        qDebug() << "In removePeopleFromMovie():";
        qDebug() << l_query.lastError().text();

        return false;
    }
    if(!l_query.next())
    {
        deletePeople(people);
    }

    return true;
}

/**
 * @brief Removes the link between a tag and a movie
 * If there is no more link with the tag, it is deleted
 *
 * @param Tag to remove
 * @param Movie concerned by the deletion
 * @return boolean
 */
bool DatabaseManager::removeTagFromMovie(Tag &tag, Movie &movie)
{
    QSqlQuery l_query(m_db);
    l_query.prepare("DELETE FROM movies_tags "
                   "WHERE id_tag = :id_tag "
                     "AND id_movie = :id_movie");
    l_query.bindValue(":id_tag", tag.getId());
    l_query.bindValue(":id_movie", movie.getId());
    if(!l_query.exec())
    {
        qDebug() << "In removeTagFromMovie():";
        qDebug() << l_query.lastError().text();

        return false;
    }

    // Checks if this tag is still used, if not deletes it.
    l_query.prepare("SELECT id FROM movies_tags WHERE id_tag = :id_tag");
    l_query.bindValue(":id_tag", tag.getId());
    if(!l_query.exec())
    {
        qDebug() << "In removeTagFromMovie():";
        qDebug() << l_query.lastError().text();

        return false;
    }
    if(!l_query.next())
    {
        deleteTag(tag);
    }

    return true;
}

/**
 * @brief Removes the link between a movie and a playlist
 *
 * @param Movie to remove from the playlist
 * @param Playlist concerned by the deletion
 * @return boolean
 */
bool DatabaseManager::removeMovieFromPlaylist(Movie &movie, Playlist &playlist)
{
    QSqlQuery l_query(m_db);
    l_query.prepare("DELETE FROM movies_playlists "
                   "WHERE id_playlist = :id_playlist "
                     "AND id_movie = :id_movie");
    l_query.bindValue(":id_playlist", playlist.getId());
    l_query.bindValue(":id_movie", movie.getId());
    if(!l_query.exec())
    {
        qDebug() << "In removeMovieFromPlaylist():";
        qDebug() << l_query.lastError().text();

        return false;
    }

    return true;
}

/**
 * @brief Deletes a playlist
 *
 * @param Playlist to delete
 * @return boolean
 */
bool DatabaseManager::deletePlaylist(Playlist &playlist)
{
    if (playlist.getId() == 0)
    {
        debug("ToWatch cannot be deleted");
        return false;
    }

    foreach (Movie l_movie, playlist.getMovieList())
    {
        removeMovieFromPlaylist(l_movie, playlist);
    }

    QSqlQuery l_query(m_db);
    l_query.prepare("DELETE FROM playlists "
                   "WHERE id = :id");
    l_query.bindValue(":id", playlist.getId());

    if(!l_query.exec())
    {
        qDebug() << "In deletePlaylist():";
        qDebug() << l_query.lastError().text();

        return false;
    }

    return true;
}

/**
 * @brief Remove a People from the database
 * /\  Should be called only by removePeopleFromMovie() !
 *
 * @param People to remove
 * @return boolean
 */
bool DatabaseManager::deletePeople(const People &people)
{
    QSqlQuery l_query(m_db);

    // This should not do anything, because deletePeople()
    // must be called only by removePeopleFromMovie()
    l_query.prepare("DELETE FROM movies_people WHERE id_people = :id");
    l_query.bindValue(":id", people.getId());

    if(!l_query.exec())
    {
        qDebug() << "In deletePeople():";
        qDebug() << l_query.lastError().text();

        return false;
    }

    l_query.prepare("DELETE FROM people WHERE id = :id");
    l_query.bindValue(":id", people.getId());

    if(!l_query.exec())
    {
        qDebug() << "In deletePeople():";
        qDebug() << l_query.lastError().text();

        return false;
    }

    return true;
}

/**
 * @brief Remove a Tag from the database
 * /\  Should be called only by removeTagFromMovie() !
 *
 * @param Tag to remove
 * @return boolean
 */
bool DatabaseManager::deleteTag(const Tag &tag)
{
    QSqlQuery l_query(m_db);

    // This should not do anything, because deleteTag()
    // must be called only by removeTagFromMovie()
    l_query.prepare("DELETE FROM movies_tags WHERE id_tag = :id");
    l_query.bindValue(":id", tag.getId());

    if(!l_query.exec())
    {
        qDebug() << "In deleteTag():";
        qDebug() << l_query.lastError().text();

        return false;
    }

    l_query.prepare("DELETE FROM tags WHERE id = :id");
    l_query.bindValue(":id", tag.getId());

    if(!l_query.exec())
    {
        qDebug() << "In deleteTag():";
        qDebug() << l_query.lastError().text();

        return false;
    }

    return true;
}
