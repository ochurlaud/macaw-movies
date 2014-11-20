#include "DatabaseManager.h"

/**
 * @brief Updates a movie from database
 *
 * @param Movie
 * @return bool
 */
bool DatabaseManager::updateMovie(Movie &movie)
{
    qDebug() << "[DatabaseManager] Enters updateMovie()";
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
        qDebug() << "In updateMovie():";
        qDebug() << l_query.lastError().text();

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
        qDebug() << "In updateMovie():";
        qDebug() << l_query.lastError().text();

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
            qDebug() << "In updateMovie():";
            qDebug() << l_query.lastError().text();

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

    qDebug() << "[DatabaseManager] Movie updated";

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
        qDebug() << "In updatePeople():";
        qDebug() << l_query.lastError().text();

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
bool DatabaseManager::updatePeopleInMovie(People &people, Movie &movie, const int type)
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
                        "FROM movies_people "
                        "WHERE id_movie = :id_movie AND id_people = :id_people AND type = :type");
        l_query.bindValue(":id_movie", movie.getId());
        l_query.bindValue(":id_people", people.getId());
        l_query.bindValue(":type", type);

        if (!l_query.exec())
        {
            qDebug() << "In updatePeopleInMovie():";
            qDebug() << l_query.lastError().text();

            return false;
        }

        if (!l_query.next())
        {
            qDebug() << "People not connected to the movie";
            l_query.prepare("INSERT INTO movies_people(id_movie, id_people, type) "
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
        qDebug() << "In updateTag():";
        qDebug() << l_query.lastError().text();

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
                        "FROM movies_tags "
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
            l_query.prepare("INSERT INTO movies_tags(id_movie, id_tag) "
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
