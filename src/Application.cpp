/* Copyright (C) 2014 Movie-Project
 * (Olivier CHURLAUD)
 *
 * This file is part of Movie-Project.
 *
 * Movie-Project is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Movie-Project is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Movie-Project.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Application.h"

/**
 * @brief Constructor
 *
 */
Application::Application(int argc, char **argv) : QApplication(argc, argv)
{
    m_dbManager = new DatabaseManager();
    if (!m_dbManager->getMoviesPath().isEmpty())
    {
        m_filesPath = m_dbManager->getMoviesPath().at(0);
    }
    else
    {
        m_filesPath = "";
    }
 //   mainWindow.setFixedSize(800,600);
}

/**
 * @brief Destructor
 */
Application::~Application()
{
}

/**
 * @brief Add a path.
 *
 * @param QString path to add
 * @return bool
 */
bool Application::addFilesPath(QString path)
{
    m_filesPath = path;
    m_dbManager->saveMoviesPath(this->m_filesPath);
    return true;
}

/**
 * @brief Gets the the files_Path (TO BE REMOVED)
 *
 */
QString Application::getFilesPath()
{
    return m_filesPath;
}

