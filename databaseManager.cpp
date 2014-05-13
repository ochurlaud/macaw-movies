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
    QStringList tables = db.tables();
    if(!db.tables().isEmpty())
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

/**
 * @brief DatabaseManager::getMovie
 * Gets the movies with a defined parameter.
 *
 * @param parameter_name
 * @param parameter_value
 * @return QSqlQuery query
 */
QSqlQuery DatabaseManager::getMovie(QString parameter_name, QVariant parameter_value)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM movie WHERE " + parameter_name + "=:parameter_value");
    query.bindValue(":parameter_value", parameter_value);
    query.exec();

    return query;
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
    // Create table "person"
    bool ret = false;
    if (db.isOpen())
    {
        QSqlQuery query;
        ret = query.exec("CREATE TABLE IF NOT EXISTS movie "
                  "(id        INTEGER AUTO_INCREMENT PRIMARY KEY, "
                  " title     VARCHAR(50), "
                  " director  VARCHAR(30), "
                  " year      INTEGER)");


    }
    return ret;
}
