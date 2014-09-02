/* Copyright (C) 2014 Movie-Project
 * (Olivier CHURLAUD, Sébastien TOUZÉ)
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

#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include "MainWindow.h"
#include "DatabaseManager.h"
#include "MoviesDebug.h"

class Application : public QApplication
{
    Q_OBJECT

    public:
        Application(int, char**);
        ~Application();
        void showMainWindow();
        bool addFilesPath(QString path);
        QString getFilesPath();
        QString getAppName() {return this->m_appName;}
        DatabaseManager *getDatabaseManager() {return this->m_dbManager;}
        void debug(QString text) { m_debug.print(text); }

    private:
    //    MainWindow mainWindow;
        QString m_filesPath;
        DatabaseManager *m_dbManager;
        QString m_appName;
        MoviesDebug m_debug;
};

#endif // APPLICATION_H
