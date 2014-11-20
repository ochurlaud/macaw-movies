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
