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
 * @brief Updates a movie from database
 *
 * @param Movie
 * @return bool
 */
bool DatabaseManager::updateMovie(Movie &movie)
{
    debug("[DatabaseManager] Enters updateMovie()");
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
                    "WHERE id = :id");
    l_query.bindValue(":title", movie.getTitle());
    l_query.bindValue(":original_title", movie.getOriginalTitle());
    l_query.bindValue(":release_date", movie.getReleaseDate().toString(DATE_FORMAT));
    l_query.bindValue(":country", movie.getCountry());
    l_query.bindValue(":synopsis", movie.getSynopsis());
    l_query.bindValue(":colored", movie.isColored());
    l_query.bindValue(":format", movie.getFormat());
    l_query.bindValue(":rank", movie.getRank());
    l_query.bindValue(":id", movie.getId());


    if (!l_query.exec())
    {
        debug("In updateMovie():");
        debug(l_query.lastError().text());

        return false;
    }

    // Insertions/Updates of the linked elements
    foreach (People l_people, movie.getPeopleList())
    {
        updatePeopleInMovie(l_people, movie, l_people.getType());
    }

    foreach (Tag l_tag, movie.getTagList())
    {
        updateTagInMovie(l_tag, movie);
    }

    // Deletion of the old linked elements
    l_query.prepare("SELECT t.id, t.name "
                    "FROM tags AS t, movies_tags AS mt "
                    "WHERE mt.id_movie = :id_movie AND mt.id_tag = t.id");
    l_query.bindValue(":id_movie", movie.getId());
    if (!l_query.exec())
    {
        debug("In updateMovie():");
        debug(l_query.lastError().text());

        return false;
    }
    while(l_query.next())
    {
        Tag l_tag;
        l_tag.setId(l_query.value(0).toInt());
        l_tag.setName(l_query.value(1).toString());
        if(movie.getTagList().indexOf(l_tag) < 0)
        {
            removeTagFromMovie(l_tag, movie);
        }
    }

    QList<int> types;
    types << Director << Producer << Actor;
    foreach (int type, types)
    {
        l_query.prepare("SELECT " + m_peopleFields + ", mp.type "
                        "FROM people AS p, movies_people AS mp "
                        "WHERE mp.id_movie = :id_movie "
                          "AND mp.type = :type "
                          "AND mp.id_people = p.id");
        l_query.bindValue(":id_movie", movie.getId());
        l_query.bindValue(":type", type);
        if (!l_query.exec())
        {
            debug("In updateMovie():");
            debug(l_query.lastError().text());

            return false;
        }
        while(l_query.next())
        {
            People l_people = hydratePeople(l_query);
            if(movie.getPeopleList().indexOf(l_people) < 0)
            {
                removePeopleFromMovie(l_people, movie, l_people.getType());
            }
        }
    }

    debug("[DatabaseManager] Movie updated");

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
    debug("[DatabaseManager] Enters updatePeople()");

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
    l_query.bindValue(":birthday",  people.getBirthday().toString(DATE_FORMAT));
    l_query.bindValue(":biography", people.getBiography());
    l_query.bindValue(":id", people.getId());

    if (!l_query.exec())
    {
        debug("In updatePeople():");
        debug(l_query.lastError().text());

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
    if (people.getId() == 0)
    {
        debug("People not known");
        addPeopleToMovie(people, movie, type);
    }
    // This means that the director exists, so we upgrade
    else
    {
        debug("People known");
        updatePeople(people);

        // Checks if the people and the movie are connected, if not connects them
        QSqlQuery l_query(m_db);
        l_query.prepare("SELECT id "
                        "FROM movies_people "
                        "WHERE id_movie = :id_movie AND id_people = :id_people AND type = :type");
        l_query.bindValue(":id_movie", movie.getId());
        l_query.bindValue(":id_people", people.getId());
        l_query.bindValue(":type", type);

        if (!l_query.exec())
        {
            debug("In updatePeopleInMovie():");
            debug(l_query.lastError().text());

            return false;
        }

        if (!l_query.next())
        {
            debug("People not connected to the movie");
            l_query.prepare("INSERT INTO movies_people(id_movie, id_people, type) "
                            "VALUES(:id_movie, :id_people, :type)");
            l_query.bindValue(":id_movie", movie.getId());
            l_query.bindValue(":id_people", people.getId());
            l_query.bindValue(":type", type);
            if (!l_query.exec())
            {
                debug("In updatePeopleInMovie():");
                debug(l_query.lastError().text());

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
    l_query.bindValue(":name", tag.getName());
    l_query.bindValue(":id", tag.getId());

    if (!l_query.exec())
    {
        debug("In updateTag():");
        debug(l_query.lastError().text());

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
    if (tag.getId() == 0)
    {
        debug("Tag not known");
        addTagToMovie(tag, movie);
    }
    // This means that the tag exists, so we upgrade
    else
    {
        debug("Tag known");
        updateTag(tag);

        // Checks if the people and the movie are connected, if not connects them
        QSqlQuery l_query(m_db);
        l_query.prepare("SELECT id "
                        "FROM movies_tags "
                        "WHERE id_movie = :id_movie AND id_tag = :id_tag");
        l_query.bindValue(":id_movie", movie.getId());
        l_query.bindValue(":id_tag", tag.getId());
        if (!l_query.exec())
        {
            debug("In updateTagInMovie():");
            debug(l_query.lastError().text());

            return false;
        }

        if (!l_query.next())
        {
            debug("Tag not connected to the movie");
            l_query.prepare("INSERT INTO movies_tags(id_movie, id_tag) "
                            "VALUES(:id_movie, :id_tag)");
            l_query.bindValue(":id_movie", movie.getId());
            l_query.bindValue(":id_tag", tag.getId());
            if (!l_query.exec())
            {
                debug("In updateTagInMovie():");
                debug(l_query.lastError().text());

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
    debug("[DatabaseManager] Enters updatePlaylist()");
    QSqlQuery l_query(m_db);
    l_query.prepare("UPDATE playlists "
                    "SET name = :name, "
                        "rate = :rate "
                    "WHERE id = :id");
    l_query.bindValue(":name", playlist.getName());
    l_query.bindValue(":rate", playlist.getRate());
    l_query.bindValue(":id", playlist.getId());

    if (!l_query.exec())
    {
        debug("In updatePlaylist():");
        debug(l_query.lastError().text());

        return false;
    }

    // Insertions/Updates of the linked elements
    foreach (Movie l_movie, playlist.getMovieList())
    {
        updateMovieInPlaylist(l_movie, playlist);
    }

    // Deletion of the old linked elements
    l_query.prepare("SELECT " + m_movieFields + " "
                    "FROM movies AS m, movies_playlists AS mpl "
                    "WHERE mpl.id_movie = m.id AND mpl.id_playlist = :id_playlist");
    l_query.bindValue(":id_playlist", playlist.getId());
    if (!l_query.exec())
    {
        debug("In updatePlaylist():");
        debug(l_query.lastError().text());

        return false;
    }
    while(l_query.next())
    {
        Movie l_movie = hydrateMovie(l_query);
        if(playlist.getMovieList().indexOf(l_movie) < 0)
        {
            removeMovieFromPlaylist(l_movie, playlist);
        }
    }

    debug("[DatabaseManager] Playlist updated");

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
    l_query.bindValue(":id_movie", movie.getId());
    l_query.bindValue(":id_playlist", playlist.getId());
    if (!l_query.exec())
    {
        debug("In updateMovieInPlaylist():");
        debug(l_query.lastError().text());

        return false;
    }

    if (!l_query.next())
    {
        debug("Movie not connected to the playlist");
        l_query.prepare("INSERT INTO movies_playlists(id_movie, id_playlist) "
                        "VALUES(:id_movie, :id_playlist)");
        l_query.bindValue(":id_movie", movie.getId());
        l_query.bindValue(":id_playlist", playlist.getId());
        if (!l_query.exec())
        {
            debug("In updateMovieInPlaylist():");
            debug(l_query.lastError().text());

            return false;
        }
    }

    return true;
}
