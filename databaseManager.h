#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QFile>
#include <QSqlQuery>
#include <QVariant>

class DatabaseManager : public QObject
{
    public:
        DatabaseManager();
        bool openDB();
        bool createTables();
        bool deleteDB();
        QSqlQuery getMovie(QString, QVariant);
        QSqlError lastError();

    private:
        QSqlDatabase db;
};

#endif // DATABASEMANAGER_H
