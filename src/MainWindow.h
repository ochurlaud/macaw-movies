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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "DatabaseManager.h"
#include "SettingsWindow.h"
#include "Application.h"

#include <QtSql>
#include <QtGui>
#include <QtWidgets>

#include <QtDebug>



class MainWindow : public QWidget
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    public slots:
        void setDodo();
        void showSettingsWindow();

    private:
        void fillLeftPannel();
        void fillMoviesList();
        QPushButton *m_settingsButton;
        QHBoxLayout *m_hLayout;
        QVBoxLayout *m_mainLayout;
        QTextEdit *m_searchEdit;
        QListWidget *m_leftPannel;
        QTableView *m_moviesList;
        QVector<QListWidgetItem*> m_moviesTitles;
};

#endif // MAINWINDOW_H
