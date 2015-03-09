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
 * @brief Updates a movie from database
 *
 * @param Movie
 * @return bool
 */
bool DatabaseManager::updateMovie(Movie &movie)
{
    Macaw::DEBUG("[DatabaseManager] Enters updateMovie()");
    QSqlQuery l_query(m_db);
    l_query.prepare("UPDATE movies "
                    "SET title = :title, "
                        "original_title = :original_title, "
                        "release_date = :release_date, "
                        "country = :country, "
                        "synopsis = :synopsis, "
                        "colored = :colored, "
                        "format = :format, "
                        "rank = :rank "
                        "poster_path = :poster_path "
                        "imported = :imported "
                    "WHERE id = :id");
    l_query.bindValue(":title", movie.title());
    l_query.bindValue(":original_title", movie.originalTitle());
    l_query.bindValue(":release_date", movie.releaseDate().toString(DATE_FORMAT));
    l_query.bindValue(":country", movie.country());
    l_query.bindValue(":synopsis", movie.synopsis());
    l_query.bindValue(":colored", movie.isColored());
    l_query.bindValue(":format", movie.format());
    l_query.bindValue(":rank", movie.rank());
    l_query.bindValue(":poster_path", movie.posterPath());
    l_query.bindValue(":imported", movie.isImported());
    l_query.bindValue(":id", movie.id());


    if (!l_query.exec())
    {
        Macaw::DEBUG("In updateMovie():");
        Macaw::DEBUG(l_query.lastError().text());

        return false;
    }

    // Insertions/Updates of the linked elements
    foreach (People l_people, movie.peopleList())
    {
        updatePeopleInMovie(l_people, movie, l_people.type());
    }

    foreach (Tag l_tag, movie.tagList())
    {
        updateTagInMovie(l_tag, movie);
    }

    // Deletion of the old linked elements
    l_query.prepare("SELECT t.id, t.name "
                    "FROM tags AS t, movies_tags AS mt "
                    "WHERE mt.id_movie = :id_movie AND mt.id_tag = t.id");
    l_query.bindValue(":id_movie", movie.id());
    if (!l_query.exec())
    {
        Macaw::DEBUG("In updateMovie():");
        Macaw::DEBUG(l_query.lastError().text());

        return false;
    }
    while(l_query.next())
    {
        Tag l_tag;
        l_tag.setId(l_query.value(0).toInt());
        l_tag.setName(l_query.value(1).toString());
        if(movie.tagList().indexOf(l_tag) < 0)
        {
            removeTagFromMovie(l_tag, movie);
        }
    }

    QList<int> types;
    types << People::Director << People::Producer << People::Actor;
    foreach (int type, types)
    {
        l_query.prepare("SELECT " + m_peopleFields + ", mp.type "
                        "FROM people AS p, movies_people AS mp "
                        "WHERE mp.id_movie = :id_movie "
                          "AND mp.type = :type "
                          "AND mp.id_people = p.id");
        l_query.bindValue(":id_movie", movie.id());
        l_query.bindValue(":type", type);
        if (!l_query.exec())
        {
            Macaw::DEBUG("In updateMovie():");
            Macaw::DEBUG(l_query.lastError().text());

            return false;
        }
        while(l_query.next())
        {
            People l_people = hydratePeople(l_query);
            if(movie.peopleList().indexOf(l_people) < 0)
            {
                removePeopleFromMovie(l_people, movie, l_people.type());
            }
        }
    }

    Macaw::DEBUG("[DatabaseManager] Movie updated");

    return true;
}

/**
 * @brief Updates a people in database
 *
 * @param People people to update
 * @return bool
 */
bool DatabaseManager::updatePeople(People &people)
{
    Macaw::DEBUG("[DatabaseManager] Enters updatePeople()");

    QSqlQuery l_query(m_db);
    l_query.prepare("UPDATE people "
                    "SET firstname = :firstname, "
                        "lastname = :lastname, "
                        "realname = :realname, "
                        "birthday = :birthday, "
                        "biography = :biography "
                    "WHERE id = :id");
    l_query.bindValue(":firstname", people.firstname());
    l_query.bindValue(":lastname", people.lastname());
    l_query.bindValue(":realname",  people.realname());
    l_query.bindValue(":birthday",  people.birthday().toString(DATE_FORMAT));
    l_query.bindValue(":biography", people.biography());
    l_query.bindValue(":id", people.id());

    if (!l_query.exec())
    {
        Macaw::DEBUG("In updatePeople():");
        Macaw::DEBUG(l_query.lastError().text());

        return false;
    }

    return true;
}

/**
 * @brief Updates a people linked to a movie
 *
 * @param People people
 * @param Movie movie
 * @param int type
 * @return bool
 */
bool DatabaseManager::updatePeopleInMovie(People &people,
                                          Movie &movie,
                                          const int type)
{
    // If the id is 0, then the director doesn't exist
    if (people.id() == 0)
    {
        Macaw::DEBUG("People not known");
        addPeopleToMovie(people, movie, type);
    }
    // This means that the people exists, so we upgrade
    else
    {
        Macaw::DEBUG("People known");
        updatePeople(people);

        // Checks if the people and the movie are connected, if not connects them
        QSqlQuery l_query(m_db);
        l_query.prepare("SELECT id "
                        "FROM movies_people "
                        "WHERE id_movie = :id_movie AND id_people = :id_people AND type = :type");
        l_query.bindValue(":id_movie", movie.id());
        l_query.bindValue(":id_people", people.id());
        l_query.bindValue(":type", type);

        if (!l_query.exec())
        {
            Macaw::DEBUG("In updatePeopleInMovie():");
            Macaw::DEBUG(l_query.lastError().text());

            return false;
        }

        if (!l_query.next())
        {
            Macaw::DEBUG("People not connected to the movie");
            l_query.prepare("INSERT INTO movies_people(id_movie, id_people, type) "
                            "VALUES(:id_movie, :id_people, :type)");
            l_query.bindValue(":id_movie", movie.id());
            l_query.bindValue(":id_people", people.id());
            l_query.bindValue(":type", type);
            if (!l_query.exec())
            {
                Macaw::DEBUG("In updatePeopleInMovie():");
                Macaw::DEBUG(l_query.lastError().text());

                return false;
            }
        }
    }

    return true;
}

/**
 * @brief Updates a tag in database
 *
 * @param Tag to update
 * @return bool
 */
bool DatabaseManager::updateTag(Tag &tag)
{
    QSqlQuery l_query(m_db);
    l_query.prepare("UPDATE tags "
                    "SET name = :name "
                    "WHERE id = :id");
    l_query.bindValue(":name", tag.name());
    l_query.bindValue(":id", tag.id());

    if (!l_query.exec())
    {
        Macaw::DEBUG("In updateTag():");
        Macaw::DEBUG(l_query.lastError().text());

        return false;
    }

    return true;
}

/**
 * @brief Updates a tag linked to a movie
 *
 * @param Tag tag
 * @param Movie movie
 * @return bool
 */
bool DatabaseManager::updateTagInMovie(Tag &tag, Movie &movie)
{
    // If the id is 0, then the tag doesn't exist
    if (tag.id() == 0)
    {
        Macaw::DEBUG("Tag not known");
        addTagToMovie(tag, movie);
    }
    // This means that the tag exists, so we upgrade
    else
    {
        Macaw::DEBUG("Tag known");
        updateTag(tag);

        // Checks if the people and the movie are connected, if not connects them
        QSqlQuery l_query(m_db);
        l_query.prepare("SELECT id "
                        "FROM movies_tags "
                        "WHERE id_movie = :id_movie AND id_tag = :id_tag");
        l_query.bindValue(":id_movie", movie.id());
        l_query.bindValue(":id_tag", tag.id());
        if (!l_query.exec())
        {
            Macaw::DEBUG("In updateTagInMovie():");
            Macaw::DEBUG(l_query.lastError().text());

            return false;
        }

        if (!l_query.next())
        {
            Macaw::DEBUG("Tag not connected to the movie");
            l_query.prepare("INSERT INTO movies_tags(id_movie, id_tag) "
                            "VALUES(:id_movie, :id_tag)");
            l_query.bindValue(":id_movie", movie.id());
            l_query.bindValue(":id_tag", tag.id());
            if (!l_query.exec())
            {
                Macaw::DEBUG("In updateTagInMovie():");
                Macaw::DEBUG(l_query.lastError().text());

                return false;
            }
        }
    }

    return true;
}

/**
 * @brief Updates a playlist from database
 *
 * @param Playlist
 * @return bool
 */
bool DatabaseManager::updatePlaylist(Playlist &playlist)
{
    Macaw::DEBUG("[DatabaseManager] Enters updatePlaylist()");
    QSqlQuery l_query(m_db);
    l_query.prepare("UPDATE playlists "
                    "SET name = :name, "
                        "rate = :rate "
                    "WHERE id = :id");
    l_query.bindValue(":name", playlist.name());
    l_query.bindValue(":rate", playlist.rate());
    l_query.bindValue(":id", playlist.id());

    if (!l_query.exec())
    {
        Macaw::DEBUG("In updatePlaylist():");
        Macaw::DEBUG(l_query.lastError().text());

        return false;
    }

    // Insertions/Updates of the linked elements
    foreach (Movie l_movie, playlist.movieList())
    {
        updateMovieInPlaylist(l_movie, playlist);
    }

    // Deletion of the old linked elements
    l_query.prepare("SELECT " + m_movieFields + " "
                    "FROM movies AS m, movies_playlists AS mpl "
                    "WHERE mpl.id_movie = m.id AND mpl.id_playlist = :id_playlist");
    l_query.bindValue(":id_playlist", playlist.id());
    if (!l_query.exec())
    {
        Macaw::DEBUG("In updatePlaylist():");
        Macaw::DEBUG(l_query.lastError().text());

        return false;
    }
    while(l_query.next())
    {
        Movie l_movie = hydrateMovie(l_query);
        if(playlist.movieList().indexOf(l_movie) < 0)
        {
            removeMovieFromPlaylist(l_movie, playlist);
        }
    }

    Macaw::DEBUG("[DatabaseManager] Playlist updated");

    return true;
}

/**
 * @brief Updates a movie linked to a playlist
 *
 * @param Movie
 * @param Playlist
 * @return bool
 */
bool DatabaseManager::updateMovieInPlaylist(Movie &movie, Playlist &playlist)
{
    updateMovie(movie);

    // Checks if the people and the movie are connected, if not connects them
    QSqlQuery l_query(m_db);
    l_query.prepare("SELECT id "
                    "FROM movies_playlists "
                    "WHERE id_movie = :id_movie AND id_playlist = :id_playlist");
    l_query.bindValue(":id_movie", movie.id());
    l_query.bindValue(":id_playlist", playlist.id());
    if (!l_query.exec())
    {
        Macaw::DEBUG("In updateMovieInPlaylist():");
        Macaw::DEBUG(l_query.lastError().text());

        return false;
    }

    if (!l_query.next())
    {
        Macaw::DEBUG("Movie not connected to the playlist");
        l_query.prepare("INSERT INTO movies_playlists(id_movie, id_playlist) "
                        "VALUES(:id_movie, :id_playlist)");
        l_query.bindValue(":id_movie", movie.id());
        l_query.bindValue(":id_playlist", playlist.id());
        if (!l_query.exec())
        {
            Macaw::DEBUG("In updateMovieInPlaylist():");
            Macaw::DEBUG(l_query.lastError().text());

            return false;
        }
    }

    return true;
}
