/* Copyright (C) 2014 Macaw-Movies
 * (Olivier CHURLAUD, Sébastien TOUZÉ)
 *
 * This file is part of Macaw-Movies.
 *
 * Macaw-Movies is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Macaw-Movies is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Macaw-Movies.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include "include_var.h"
#include "MainWindow.h"
#include "DatabaseManager.h"
#include "MoviesDebug.h"
#include "Entities/Movie.h"

class MovieDebug;

class Application : public QApplication
{
Q_OBJECT

public:
    Application(int &argc, char **argv, MoviesDebug*);
    ~Application();
    bool addFilesPath(QString path);
    QString getFilesPath();
    DatabaseManager *getDatabaseManager() { return m_dbManager; }
    void debug(QString text) { m_debug->print(text); }
    QString tmdbkey() { return m_tmdbkey; }

private:
    QString m_filesPath;
    QString m_tmdbkey;
    DatabaseManager *m_dbManager;
    MoviesDebug *m_debug;
};

#endif // APPLICATION_H
