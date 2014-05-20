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
        QSqlError lastError();
        QSqlQueryModel *createModel();

    private:
        QSqlDatabase db;
};

#endif // DATABASEMANAGER_H
