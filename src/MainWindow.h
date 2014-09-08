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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "SettingsWindow.h"
#include "Application.h"

#include <QtSql>
#include <QtGui>
#include <QtWidgets>

class Application;

class MainWindow : public QWidget
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    public slots:
        void updateApp();
        void showSettingsWindow();
        void startMovie(QModelIndex);
        void showDirectorsMovies(QListWidgetItem*);

    private:
        QPushButton *m_settingsButton;
        QHBoxLayout *m_hLayout;
        QVBoxLayout *m_button_layout;
        QVBoxLayout *m_mainLayout;
        QVBoxLayout *m_centralLayout;
        QHBoxLayout *m_SecondaryLayout;
        QTextEdit *m_searchEdit;
        QPushButton *m_allMoviesButton;
        QPushButton *m_tagsButton;
        QPushButton *m_directorsButton;
        QPushButton *m_toWatchButton;
        QPushButton *m_playlistButton;
        QTableView *m_moviesList;
        QListWidget *m_directorList;
        QVector<QListWidgetItem*> m_moviesTitles;
        Application * m_app;

private slots:
//        void fillLeftPannel();
    void fillMoviesList();
    void fillTagsList();
    void fillDirectorList();
    void fillToWatchList();
    void fillPlaylist();
};

#endif // MAINWINDOW_H
