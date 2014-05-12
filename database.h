#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QFile>

class Database : public QObject
    {
    public:
        Database(QObject *parent = 0);
        ~Database();

    public:
        bool openDB();
        bool deleteDB();
        QSqlError lastError();

    private:
        QSqlDatabase db;
    };

#endif // DATABASE_H
