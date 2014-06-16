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

#include <QApplication>
#include <QDir>
#include "MainWindow.h"
#include "DatabaseManager.h"
#include "Application.h"

#include <QtDebug>

int main(int argc, char *argv[])
{
    Application l_app(argc, argv);
    MainWindow l_mainWindow;

    QDir l_directory("c:/Users/Olivier/Downloads/Movies");
    QFileInfoList l_list = l_directory.entryInfoList();
    for (int i = 0; i < l_list.size(); ++i) {
        QFileInfo l_fileInfo = l_list.at(i);
        qDebug() << QString(l_fileInfo.fileName());
    }

    l_mainWindow.show();

    return l_app.exec();
}
