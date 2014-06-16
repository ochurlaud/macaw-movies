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

#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    setFixedSize(800,600);
    m_mainLayout = new QVBoxLayout(this);
    m_hLayout = new QHBoxLayout;
    DatabaseManager l_databaseManager;

    // m_leftPannel : list of movies or realisators or ...
    m_leftPannel = new QListWidget;
    QSqlQuery l_titlesRequest = l_databaseManager.getAllTitles();
    int i(0);
    while(l_titlesRequest.next())
    {
        QString l_title = l_titlesRequest.value(0).toString();
        m_moviesTitles.push_back(new QListWidgetItem(l_title));
        m_leftPannel->insertItem(i, m_moviesTitles[i]);
        i++;
    }
    m_leftPannel->setMaximumWidth(this->width()*.3);

    // m_movieList : central list. Would be great to be able to how it looks like
    m_moviesList = new QTableView;
    QSqlQueryModel *l_modelMoviesList = new QSqlQueryModel;
    l_modelMoviesList = l_databaseManager.createModel();
    l_modelMoviesList->setQuery(l_databaseManager.getAllMovies());
    m_moviesList->setModel(l_modelMoviesList);
    m_moviesList->setShowGrid(false);
    m_moviesList->verticalHeader()->hide();
    m_moviesList->setAlternatingRowColors(true);

    m_settingsButton = new QPushButton("Settings");
    QObject::connect(m_settingsButton, SIGNAL(clicked()), this, SLOT(showSettingsWindow()));

    m_mainLayout->addWidget(m_settingsButton);
    m_mainLayout->addLayout(m_hLayout);
    m_hLayout->addWidget(m_leftPannel);
    m_hLayout->addWidget(m_moviesList);
}

MainWindow::~MainWindow()
{
}

void MainWindow::showSettingsWindow()
{
    SettingsWindow *l_settingsWindow = new SettingsWindow;
    l_settingsWindow->show();
    QObject::connect(l_settingsWindow,SIGNAL(closed()), this, SLOT(update()));
}
