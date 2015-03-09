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
 * @brief Adds a movie to the database
 *
 * @param Movie
 * @return bool
 */
bool DatabaseManager::insertNewMovie(Movie &movie)
{
    QSqlQuery l_query(m_db);
    l_query.prepare("INSERT INTO movies(title, original_title, release_date, country, duration, synopsis, file_path, poster_path, colored, format, suffix, rank, imported) "
                    "VALUES (:title, :original_title, :release_date, :country, :duration, :synopsis, :file_path, :poster_path, :colored, :format, :suffix, :rank, :imported)");
    l_query.bindValue(":title", movie.title());
    l_query.bindValue(":original_title", movie.originalTitle()   );
    l_query.bindValue(":release_date", movie.releaseDate().toString(DATE_FORMAT));
    l_query.bindValue(":country", movie.country());
    l_query.bindValue(":duration", movie.duration().msecsSinceStartOfDay());
    l_query.bindValue(":synopsis", movie.synopsis());
    l_query.bindValue(":file_path", movie.filePath());
    l_query.bindValue(":poster_path", movie.posterPath());
    l_query.bindValue(":colored", movie.isColored());
    l_query.bindValue(":format", movie.format());
    l_query.bindValue(":suffix", movie.suffix());
    l_query.bindValue(":rank", movie.rank());
    l_query.bindValue(":imported", movie.isImported());

    if (!l_query.exec())
    {
        Macaw::DEBUG("In insertNewMovie():");
        Macaw::DEBUG(l_query.lastError().text());

        return false;
    }

    Macaw::DEBUG("[DatabaseManager] Movie added");

    movie.setId(l_query.lastInsertId().toInt());

    for(int i = 0 ; i < movie.peopleList().size() ; i++)
    {
        People l_people = movie.peopleList().at(i);
        if (!addPeopleToMovie(l_people, movie, l_people.type()))
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
 * @return bool
 */
bool DatabaseManager::addPeopleToMovie(People &people, Movie &movie, const int type)
{
    if (!insertNewPeople(people))
    {
        return false;
    }
    QSqlQuery l_query(m_db);
    l_query.prepare("INSERT INTO movies_people (id_people, id_movie, type) "
                    "VALUES (:id_people, :id_movie, :type)");
    l_query.bindValue(":id_people", people.id());
    l_query.bindValue(":id_movie", movie.id());
    l_query.bindValue(":type", type);

    if (!l_query.exec())
    {
        Macaw::DEBUG("In addPeopleToMovie():");
        Macaw::DEBUG(l_query.lastError().text());

        return false;
    }

    movie = getOneMovieById(movie.id());

    return true;
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
    if (!insertNewTag(tag))
    {
        return false;
    }

    QSqlQuery l_query(m_db);
    l_query.prepare("INSERT INTO movies_tags (id_tag, id_movie) "
                    "VALUES (:id_tag, :id_movie)");
    l_query.bindValue(":id_tag", tag.id());
    l_query.bindValue(":id_movie", movie.id());

    if (!l_query.exec())
    {
        Macaw::DEBUG("In addTagToMovie():");
        Macaw::DEBUG(l_query.lastError().text());

        return false;
    }
    movie = getOneMovieById(movie.id());

    return true;
}

/**
 * @brief Adds a person to the database.
 * Should not be called directly.
 *
 * @param People
 * @return bool
 */
bool DatabaseManager::insertNewPeople(People &people)
{
    QSqlQuery l_query(m_db);
    l_query.prepare("INSERT INTO people (lastname, firstname, realname, birthday, biography) "
                    "VALUES (:lastname, :firstname, :realname, :birthday, :biography)");
    l_query.bindValue(":lastname", people.lastname());
    l_query.bindValue(":firstname", people.firstname());
    l_query.bindValue(":realname", people.realname());
    l_query.bindValue(":birthday", people.birthday().toString(DATE_FORMAT));
    l_query.bindValue(":biography", people.biography());

    if (!l_query.exec())
    {
        Macaw::DEBUG("In insertNewPeople():");
        Macaw::DEBUG(l_query.lastError().text());

        return false;
    }
    people.setId(l_query.lastInsertId().toInt());

    return true;
}

/**
 * @brief Adds a tag to the database.
 * Should not be called directly.
 *
 * @param Tag
 * @return bool
 */
bool DatabaseManager::insertNewTag(Tag &tag)
{
    QSqlQuery l_query(m_db);
    l_query.prepare("INSERT INTO tag (name) "
                    "VALUES (:name)");
    l_query.bindValue(":name", tag.name());

    if (!l_query.exec())
    {
        Macaw::DEBUG("In insertNewTag():");
        Macaw::DEBUG(l_query.lastError().text());

        return false;
    }
    tag.setId(l_query.lastInsertId().toInt());

    return true;
}

/**
 * @brief Adds a playlist to the database.
 *
 * @param Playlist
 * @return bool
 */
bool DatabaseManager::insertNewPlaylist(Playlist &playlist)
{
    QSqlQuery l_query(m_db);
    l_query.prepare("INSERT INTO playlists (name, creation_date, rate) "
                    "VALUES (:name, :creation_date, :rate)");
    l_query.bindValue(":name", playlist.name());
    l_query.bindValue(":creation_date", playlist.creationDate().toTime_t());
    l_query.bindValue(":rate", playlist.rate());

    if (!l_query.exec())
    {
        Macaw::DEBUG("In insertNewPlaylist():");
        Macaw::DEBUG(l_query.lastError().text());

        return false;
    }
    playlist.setId(l_query.lastInsertId().toInt());

    return true;
}
