#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QFile>
#include <QSqlQuery>
#include <QVariant>
#include <QSqlQueryModel>

class DatabaseManager : public QObject
{
    public:
        DatabaseManager();
        bool openDB();
        bool createTables();
        bool deleteDB();
        QSqlQuery getMovies(QString, QVariant);
        QSqlQuery getAllMovies();
        QSqlQuery getAllTitles();
        QSqlError lastError();
        QSqlQueryModel *createModel();
        QSqlQueryModel *createTitleModel();

    private:
        QSqlDatabase m_db;
};

#endif // DATABASEMANAGER_H
