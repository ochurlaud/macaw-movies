#include "DatabaseManager.h"

#include <QtDebug>

/**
 * @brief DatabaseManager::DatabaseManager
 * Constructor.
 * Opens the Database. If empty, create the schema.
 */
DatabaseManager::DatabaseManager()
{
    openDB();
    if(db.tables().isEmpty())
    {
        createTables();
    }
}

/**
 * @brief DatabaseManager::openDB
 * Opens the database
 *
 * @return bool db.open()
 */
bool DatabaseManager::openDB()
{
    db = QSqlDatabase::addDatabase("QSQLITE");

  #ifdef Q_OS_LINUX
    // NOTE: We have to store database file into user home folder in Linux
    QString path(QDir::home().path());
    path.append(QDir::separator()).append("my.db.sqlite");
    path = QDir::toNativeSeparators(path);
    db.setDatabaseName(path);
  #else
    // NOTE: File exists in the application private folder, in Symbian Qt implementation
    db.setDatabaseName("my.db.sqlite");
  #endif

    // Open databasee
    return db.open();
}

QSqlError DatabaseManager::lastError()
{
    // If opening database has failed user can ask
    // error description by QSqlError::text()
    return db.lastError();
}

/**
 * @brief DatabaseManager::deleteDB
 * Deletes the database.
 *
 * @return bool
 */
bool DatabaseManager::deleteDB()
{
    // Close database
    db.close();

    #ifdef Q_OS_LINUX
    // NOTE: We have to store database file into user home folder in Linux
    QString path(QDir::home().path());
    path.append(QDir::separator()).append("my.db.sqlite");
    path = QDir::toNativeSeparators(path);
    return QFile::remove(path);
    #else
    // Remove created database binary file
    return QFile::remove("my.db.sqlite");
    #endif
}

/**
 * @brief DatabaseManager::createTables
 * Creates all the tables
 *
 * @return bool
 */
bool DatabaseManager::createTables()
{
    // Create table "movies"
    bool ret = false;
    if (db.isOpen())
    {
        QSqlQuery query;
        ret = query.exec("CREATE TABLE movies("
                  "id INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE, "
                  "title VARCHAR(50), "
                  "original_title VARCHAR(50), "
                  "director VARCHAR(30), "
                  "producer VARCHAR(30), "
                  "year INTEGER, "
                  "country VARCHAR(30), "
                  "duration INTEGER, "
                  "synopsis TEXT, "
                  "file_path VARCHAR(255), "
                  "colored BOOLEAN, "
                  "format VARCHAR(10) "
                  ")");

/*       Et bien l’idéal serait d’être en capacité de récupérer les infos directement dans un truc genre IMDB et l’injecter dans ton outil (comme c’est fait avec gracenote pour Itunes).

        http://fr.wikipedia.org/wiki/Liste_de_bases_de_données_cinématographiques_de_l'Internet (plein de choix….)

        —>  voir ici pour une base gratuite avec des apis http://www.bdfci.info/apis/

        Ainsi, on aurait tout plein d’infos super intéressante :
        Titre original (I)
        Titre Francais (I)
        Date de sortie (I)
        Typr fr film (Drame, Romance, Science Fiction, etc (choix multiples) (I)
        Director (I)
        Pays
        Description, résumé
        Chemin vers les fichers film et sous titre (plusieurs fichiers dans le cas d’un truc avec partie 1 et partie 2)
        Durée (a récupérer sur le fichier)
        Langues disponibles sur le fichier
        Couleur/NB
        Format (16/9, 4/3, etc) (a récupérer sur le fichier)
        Taille de l’image (620x420, 1920x1080 ou autre) (a récupérer sur le fichier)
        Affiche du film.
        Writer
        Star 1, star 2, star 3 , star 4,  etc (I)
        Producteur (I)

        Musique par :
        Site officiel
        Box office :
            budget
            Gains lors de la sortie
            Nb de spectateurs lors de la mise ne exploitation
        Prix : Oscar, lion, etc (plusieurs possibilités) et pour qui (acteur, actrice, producteur, réalisateur, writer, etc) (I)
        lieux de tournage : Paris, New york, plusieurs saisies possibles.

        Complément pour les videos TV :
        Saison,
        Episode,
        Identifiant d’épisode


        La où j’ai mis (I) c’est que ce la doit être un index de tri ou de recherche.
*/
    }
    return ret;
}

/**
 * @brief DatabaseManager::getMovies
 * Gets the movies with a defined parameter.
 *
 * @param parameter_name
 * @param parameter_value
 * @return QSqlQuery query
 */
QSqlQuery DatabaseManager::getMovies(QString parameter_name, QVariant parameter_value)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM movies WHERE " + parameter_name + "=:parameter_value");
    query.bindValue(":parameter_value", parameter_value);
    query.exec();

    return query;
}

/**
 * @brief DatabaseManager::getAllMovies
 * Gets all the movies
 *
 * @return QSqlQuery query
 */
QSqlQuery DatabaseManager::getAllMovies()
{
    QSqlQuery query;
    query.prepare("SELECT title, director, year FROM movies");
    query.exec();

    return query;
}

QSqlQueryModel *DatabaseManager::createModel()
{
   QSqlQueryModel *model = new QSqlQueryModel;
   model->setHeaderData(0, Qt::Horizontal, "Title");
   model->setHeaderData(1, Qt::Horizontal, "Director");
   model->setHeaderData(2, Qt::Horizontal, "Year");

   return model;
}
