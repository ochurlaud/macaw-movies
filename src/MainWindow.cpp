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

/**
 * @brief Cosntructor.
 * Creates the design of the window.
 */
MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    m_app = qobject_cast<Application *>(qApp);
    setWindowTitle(m_app->getFilesPath());
    setFixedSize(800,600);
    m_mainLayout = new QVBoxLayout(this);
    m_hLayout = new QHBoxLayout;

    // m_leftPannel : list of movies or realisators or ...
    m_leftPannel = new QListWidget;
    fillLeftPannel();
    m_leftPannel->setMaximumWidth(this->width()*.3);

    // m_movieList : central list. Would be great to be able to how it looks like
    m_moviesList = new QTableView;
    fillMoviesList();
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


/**
 * @brief Destructor
 */
MainWindow::~MainWindow()
{
}

/**
 * @brief Call and shows the settings window.
 * Calls the SettingsWindow class and waits until it closes to handle the results
 */
void MainWindow::showSettingsWindow()
{
    SettingsWindow *l_settingsWindow = new SettingsWindow;
    l_settingsWindow->setModal(true);
    l_settingsWindow->show();
    QObject::connect(l_settingsWindow,SIGNAL(closed()), this, SLOT(updateApp()));
}

/**
 * @brief Add movies to the database and updates the application.
 */
void MainWindow::updateApp()
{
    QString l_directoryName = m_app->getFilesPath();
    QDirIterator l_path(l_directoryName, QDir::NoDotAndDotDot | QDir::Files,QDirIterator::Subdirectories);

    while (l_path.hasNext()) {
        l_path.next();

        // Could be cheacked before whether a row with same *path* exists
        QStringList l_value;
        l_value << "title" << l_path.fileInfo().baseName()
                << "file_path" << l_path.fileInfo().absoluteFilePath()
                << "format" << l_path.fileInfo().suffix();
        m_app->getDataBaseManager()->insertNewTitle(l_value);

    }
    fillLeftPannel();
    fillMoviesList();
}

/**
 * @brief Reads the database and fills the left pannel of the window.
 */
void MainWindow::fillLeftPannel()
{

    QSqlQuery l_titlesRequest = m_app->getDataBaseManager()->getAllTitles();
    int i(0);
    while(l_titlesRequest.next())
    {
        QString l_title = l_titlesRequest.value(0).toString();
        m_moviesTitles.push_back(new QListWidgetItem(l_title));
        m_leftPannel->insertItem(i, m_moviesTitles[i]);
        i++;
    }
}

/**
 * @brief Reads the database and fills the main pannel of the window.
 */
void MainWindow::fillMoviesList()
{
    QSqlQueryModel * l_modelMoviesList = new QSqlQueryModel;
    l_modelMoviesList = m_app->getDataBaseManager()->createModel();
    l_modelMoviesList->setQuery(m_app->getDataBaseManager()->getAllMovies());
    m_moviesList->setModel(l_modelMoviesList);
}
