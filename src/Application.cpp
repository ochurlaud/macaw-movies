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
    DatabaseManager l_databasemanager;

    m_filesPath = l_databasemanager.getMoviesPath().at(0);

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
    DatabaseManager l_databasemanager;
    l_databasemanager.saveMoviesPath(this->m_filesPath);
    return true;
}
QString Application::getFilesPath()
{
    return m_filesPath;
}

