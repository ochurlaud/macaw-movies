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

Application::Application(int argc, char **argv) : QApplication(argc, argv)
{
    m_dbManager = new DatabaseManager();
qDebug() << "1";
    if (!m_dbManager->getMoviesPath().isEmpty())
    {
        m_filesPath = m_dbManager->getMoviesPath().at(0);
    }
    else
    {
        m_filesPath = "";
    }
qDebug() << "2";
 //   mainWindow.setFixedSize(800,600);
}

Application::~Application()
{
}

/*void Application::showMainWindow()
{
    mainWindow.show();
}*/

bool Application::setFilesPath(QString path)
{
    m_filesPath = path;
    m_dbManager->saveMoviesPath(this->m_filesPath);
    return true;
}
QString Application::getFilesPath()
{
    return m_filesPath;
}

