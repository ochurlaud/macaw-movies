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
    if(m_db.tables().isEmpty())
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
    m_db = QSqlDatabase::addDatabase("QSQLITE");

  #ifdef Q_OS_LINUX
    // NOTE: We have to store database file into user home folder in Linux
    QString path(QDir::home().path());
    path.append(QDir::separator()).append("my.db.sqlite");
    path = QDir::toNativeSeparators(path);
    m_db.setDatabaseName(path);
  #else
    // NOTE: File exists in the application private folder, in Symbian Qt implementation
    m_db.setDatabaseName("my.db.sqlite");
  #endif

    // Open databasee
    return m_db.open();
}

QSqlError DatabaseManager::lastError()
{
    // If opening database has failed user can ask
    // error description by QSqlError::text()
    return m_db.lastError();
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
    m_db.close();

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
    bool l_ret = false;
    if (m_db.isOpen())
    {
        QSqlQuery l_query;
        l_ret = l_query.exec("CREATE TABLE movies("
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
    }

    return l_ret;
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
    QSqlQuery l_query;
    l_query.prepare("SELECT * FROM movies WHERE " + parameter_name + "=:parameter_value");
    l_query.bindValue(":parameter_value", parameter_value);
    l_query.exec();

    return l_query;
}

/**
 * @brief DatabaseManager::getAllMovies
 * Gets all the movies
 *
 * @return QSqlQuery query
 */
QSqlQuery DatabaseManager::getAllMovies()
{
    QSqlQuery l_query;
    l_query.prepare("SELECT title, director, year FROM movies");
    l_query.exec();

    return l_query;
}

QSqlQueryModel *DatabaseManager::createModel()
{
   QSqlQueryModel *l_model = new QSqlQueryModel;
   l_model->setHeaderData(0, Qt::Horizontal, tr("rtle"));
   l_model->setHeaderData(1, Qt::Horizontal, tr("Director"));
   l_model->setHeaderData(2, Qt::Horizontal, tr("Year"));

   return l_model;
}

QSqlQuery DatabaseManager::getAllTitles()
{
    QSqlQuery l_query;
    l_query.prepare("SELECT title || ifnull(' (' || year || ')','') as value FROM movies");
    l_query.exec();

    return l_query;
}

