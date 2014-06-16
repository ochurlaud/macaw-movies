/* Copyright (C) 2014 MusicProject
 * (Olivier CHURLAUD)
 *
 * This file is part of MusicProject.
 *
 * MusicProject is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * MusicProject is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with MusicProject.  If not, see <http://www.gnu.org/licenses/>.
 */

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
