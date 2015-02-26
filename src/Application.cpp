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

#include "Application.h"

/**
 * @brief Constructor
 *
 */
Application::Application(int &argc, char **argv, MoviesDebug *moviesDebug) :
    QApplication(argc, argv)
{
    this->setApplicationName(APP_NAME);
    this->setApplicationVersion(APP_VERSION);
    this->setWindowIcon(QIcon(":/img/logov0_1.png"));
    m_tmdbkey = "6e4cbac7861ad5b847ef8f60489dc04e";
    m_debug = moviesDebug;

    debug("[Application] started");

    m_dbManager = new DatabaseManager(moviesDebug);
    debug("[Application] Database initialized");
}

/**
 * @brief Destructor
 */
Application::~Application()
{
    debug("[Application] Destructed");
}
