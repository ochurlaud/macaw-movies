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

#include "MainWindow.h"

/**
 * @brief Constructor.
 * Creates the design of the window.
 */
MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    m_app = qobject_cast<Application *>(qApp);
    m_app->debug("[MainWindow] Constructor called");

    setWindowTitle(m_app->getAppName());
    setFixedSize(800,600);
    m_mainLayout = new QVBoxLayout(this);
    m_SecondaryLayout = new QHBoxLayout;
    m_hLayout = new QHBoxLayout;
    m_button_layout = new QVBoxLayout;


    // m_leftPannel : list of movies or realisators or ...
    m_allMoviesButton = new QPushButton("Films");
    m_tagsButton = new QPushButton("Tags");
    m_directorsButton = new QPushButton("Director");
    m_toWatchButton = new QPushButton("To Watch");
    m_playlistButton = new QPushButton("Playlist");
    QObject::connect(m_allMoviesButton, SIGNAL(clicked()), this, SLOT(fillMoviesListAll()));
    QObject::connect(m_tagsButton, SIGNAL(clicked()), this, SLOT(fillTagsList()));
    QObject::connect(m_directorsButton, SIGNAL(clicked()), this, SLOT(fillDirectorList()));
    QObject::connect(m_toWatchButton, SIGNAL(clicked()), this, SLOT(fillToWatchList()));
    QObject::connect(m_playlistButton, SIGNAL(clicked()), this, SLOT(fillPlaylist()));

    // m_movieList : central list. Would be great to be able to choose how it looks like
    m_centralLayout = new QVBoxLayout;

    fillMoviesList(m_app->getDatabaseManager()->getAllMovies());

    m_settingsButton = new QPushButton("Settings");
    QObject::connect(m_settingsButton, SIGNAL(clicked()), this, SLOT(showSettingsWindow()));

    m_mainLayout->addWidget(m_settingsButton);
    m_mainLayout->addLayout(m_SecondaryLayout);
    m_SecondaryLayout->addLayout(m_button_layout);
    m_SecondaryLayout->addLayout(m_hLayout);
    m_button_layout->addWidget(m_allMoviesButton);
    m_button_layout->addWidget(m_tagsButton);
    m_button_layout->addWidget(m_directorsButton);
    m_button_layout->addWidget(m_toWatchButton);
    m_button_layout->addWidget(m_playlistButton);
    m_hLayout->addLayout(m_centralLayout);
    m_app->debug("[MainWindow] Construction done");
}


/**
 * @brief Destructor
 */
MainWindow::~MainWindow()
{
    m_app->debug("[MainWindow] Destructed");
}

/**
 * @brief Call and shows the settings window.
 * Calls the SettingsWindow class and waits until it closes to handle the results
 */
void MainWindow::showSettingsWindow()
{
    m_app->debug("[MainWindow] Enters showSettingsWindow()");
    SettingsWindow *l_settingsWindow = new SettingsWindow;
    l_settingsWindow->setModal(true);
    l_settingsWindow->setAttribute(Qt::WA_DeleteOnClose);
    l_settingsWindow->show();
    QObject::connect(l_settingsWindow,SIGNAL(closeAndSave()), this, SLOT(updateApp()));
    m_app->debug("[MainWindow] Exits showSettingsWindow()");
}

/**
 * @brief Add movies to the database and updates the application.
 */
void MainWindow::updateApp()
{
    m_app->debug("[MainWindow] Enters updateApp()");
    QString l_directoryName = m_app->getFilesPath();
    QDirIterator l_path(l_directoryName, QDir::NoDotAndDotDot | QDir::Files,QDirIterator::Subdirectories);

    while (l_path.hasNext()) {
        l_path.next();

        QString l_filePath = l_path.fileInfo().absoluteFilePath();
        QString l_fileSuffix = l_path.fileInfo().suffix();
        // First we check whether the file is already saved or not
        bool l_movieExists = m_app->getDatabaseManager()->existMovie(l_filePath);
        qDebug() << l_fileSuffix;
        qDebug() << (!l_movieExists && (l_fileSuffix == "mkv" || l_fileSuffix == "avi" || l_fileSuffix == "mp4") );
        if (!l_movieExists && (l_fileSuffix == "mkv" || l_fileSuffix == "avi" || l_fileSuffix == "mp4") )
        {
            Movie l_movie;
            l_movie.setTitle(l_path.fileInfo().completeBaseName());
            l_movie.setFilePath(l_path.fileInfo().absoluteFilePath());
            l_movie.setSuffix(l_fileSuffix);
            m_app->getDatabaseManager()->insertNewMovie(l_movie);
        }
    }
    fillMoviesList(m_app->getDatabaseManager()->getAllMovies());
    m_app->debug("[MainWindow] Exits updateApp()");
}

/**
 * @brief Reads the database and fills the main pannel of the window.
 */
void MainWindow::fillMoviesList(QVector<Movie> moviesList)
{
    m_app->debug("[MainWindow] Enters fillMoviesList()");
    m_moviesTable = new QTableWidget;

    m_moviesTable->setRowCount(moviesList.size());
    m_moviesTable->setColumnCount(4);
    QStringList l_headers;
    l_headers << "Title" << "Original Title" << "Year" << "Path of the file";
    m_moviesTable->setHorizontalHeaderLabels(l_headers);
    for (int i = 0 ; i < moviesList.size() ; i++)
    {
        QTableWidgetItem *l_title = new QTableWidgetItem(moviesList.at(i).getTitle());
        l_title->setData(Qt::UserRole, moviesList.at(i).getId());
        QTableWidgetItem *l_originalTitle = new QTableWidgetItem(moviesList.at(i).getOriginalTitle());
        l_originalTitle->setData(Qt::UserRole, moviesList.at(i).getId());
        QTableWidgetItem *l_year = new QTableWidgetItem(moviesList.at(i).getYear());
        l_year->setData(Qt::UserRole, moviesList.at(i).getId());
        QTableWidgetItem *l_filePath = new QTableWidgetItem(moviesList.at(i).getFilePath());
        l_filePath->setData(Qt::UserRole, moviesList.at(i).getId());

        m_moviesTable->setItem(i, 0, l_title);
        m_moviesTable->setItem(i, 1, l_originalTitle);
        m_moviesTable->setItem(i, 2, l_year);
        m_moviesTable->setItem(i, 3, l_filePath);
    }
    m_moviesTable->setShowGrid(false);
    m_moviesTable->verticalHeader()->hide();
    m_moviesTable->setAlternatingRowColors(true);

    connect(m_moviesTable,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),this,SLOT(startMovie(QTableWidgetItem*)));

    while(!m_centralLayout->isEmpty())
    {
        delete m_centralLayout->takeAt(0);
    }
    m_centralLayout->addWidget(m_moviesTable);
    m_app->debug("[MainWindow] Exits fillMoviesList()");
}

void MainWindow::fillTagsList()
{
    m_app->debug("[MainWindow] Enters fillTagsList");
    m_centralList = new QListWidget(this);
    QListWidgetItem *l_tempItem = new QListWidgetItem;
    l_tempItem->setText("All");
    m_centralList->insertItem(0, l_tempItem);
    QVector<Tag> l_tagsVector = m_app->getDatabaseManager()->getAllTags();

    for (int i = 0 ; i < l_tagsVector.size() ; i++)
    {
        Tag l_tag = l_tagsVector.at(i);
        QListWidgetItem *l_tempItem = new QListWidgetItem;
        l_tempItem->setText(l_tag.getName());
        l_tempItem->setData(Qt::UserRole, l_tag.getId());
        m_centralList->insertItem(i+1, l_tempItem);
    }

    QObject::connect(m_centralList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(showTagsMovies(QListWidgetItem*)));
    m_centralList->setViewMode(QListView::IconMode);
    while(!m_centralLayout->isEmpty())
    {
        delete m_centralLayout->takeAt(0);
    }
    m_centralLayout->addWidget(m_centralList);

    m_app->debug("[MainWindow] Exits fillTagsList");
}

void MainWindow::fillDirectorList()
{
    m_app->debug("[MainWindow] Enters fillDirectorList");
    m_centralList = new QListWidget(this);
    QListWidgetItem *l_tempItem = new QListWidgetItem;
    l_tempItem->setText("All");
    m_centralList->insertItem(0, l_tempItem);
    QVector<People> l_directors = m_app->getDatabaseManager()->getAllDirectors();

    for (int i = 0 ; i < l_directors.size() ; i++)
    {
        People director = l_directors.at(i);
        QListWidgetItem *l_tempItem = new QListWidgetItem;
        l_tempItem->setText(director.getFirstname() + " " + director.getLastname() );
        l_tempItem->setData(Qt::UserRole, director.getId());
        m_centralList->insertItem(i+1, l_tempItem);
    }

    QObject::connect(m_centralList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(showDirectorsMovies(QListWidgetItem*)));
    m_centralList->setViewMode(QListView::IconMode);
    while(!m_centralLayout->isEmpty())
    {
        delete m_centralLayout->takeAt(0);
    }
    m_centralLayout->addWidget(m_centralList);

    m_app->debug("[MainWindow] Exits fillDirectorList");
}

void MainWindow::fillToWatchList()
{

}

void MainWindow::fillPlaylist()
{

}


void MainWindow::startMovie(QTableWidgetItem *item)
{
    m_app->debug("[MainWindow] Enters startMovie");

    int l_movieId = item->data(Qt::UserRole).toInt();
    Movie l_movie = m_app->getDatabaseManager()->getOneMovieById(l_movieId);

    m_app->debug("[MainWindow] Opened movie: " + l_movie.getFilePath());

    QDesktopServices::openUrl(QUrl("file:///" + l_movie.getFilePath(), QUrl::TolerantMode));

    m_app->debug("[MainWindow] Exits startMovie");
}

void MainWindow::showDirectorsMovies(QListWidgetItem* item)
{
    m_app->debug("[MainWindow] Enters showDirectorMovies");

    if (item->data(Qt::UserRole).isNull())
    {
        fillMoviesList(m_app->getDatabaseManager()->getAllMovies());
    }
    else
    {
        People l_director = m_app->getDatabaseManager()->getOneDirectorById(item->data(Qt::UserRole).toInt());
        fillMoviesList(m_app->getDatabaseManager()->getMoviesByDirector(l_director));
    }
    m_app->debug("[MainWindow] Exits showDirectorMovies");
}

void MainWindow::showTagsMovies(QListWidgetItem* item)
{
    m_app->debug("[MainWindow] Enters showTagsMovies");

    if (item->data(Qt::UserRole).isNull())
    {
        fillMoviesList(m_app->getDatabaseManager()->getAllMovies());
    }
    else
    {
        Tag l_tag = m_app->getDatabaseManager()->getOneTagById(item->data(Qt::UserRole).toInt());
        fillMoviesList(m_app->getDatabaseManager()->getMoviesByTag(l_tag));
    }
    m_app->debug("[MainWindow] Exits showTagsMovies");
}

void MainWindow::fillMoviesListAll()
 {
     fillMoviesList(m_app->getDatabaseManager()->getAllMovies());
 }
