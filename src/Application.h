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

#include "DatabaseManager.h"
#include "MainWindow.h"

#include "Entities/Movie.h"

namespace Macaw
{
    enum fields {
        ObjectId = Qt::UserRole,
        ObjectType = Qt::UserRole+1,
        PeopleType = Qt::UserRole+2
    };
    enum typeElement {
        None,
        isMovie,
        isPeople,
        isTag,
        isPlaylist
    };

}

class MovieDebug;
class MainWindow;

/**
 * @brief The Application class. Core of the application
 */
class Application : public QApplication
{
Q_OBJECT

public:
    Application(int &argc, char **argv);
    ~Application();
    QString tmdbkey() { return m_tmdbkey; }

private:

    /**
     * @brief API key for The Movie Database (TMDb)
     *
     * should be called with `m_app->tmdbkey()`
     */
    QString m_tmdbkey;

    /**
     * @brief MainWindow: where everything happens
     */
    MainWindow *m_mainWindow;
};

#endif // APPLICATION_H
