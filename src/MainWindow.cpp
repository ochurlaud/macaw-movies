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
 * @brief Add movies of the paths to the database and updates the application.
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

        if (l_fileSuffix == "mkv" || l_fileSuffix == "avi" || l_fileSuffix == "mp4" || l_fileSuffix == "mpg" || l_fileSuffix == "flv" || l_fileSuffix == "mov")
        {
            m_app->debug("[MainWindow.updateApp()] Suffix accepted");
            bool l_movieExists = m_app->getDatabaseManager()->existMovie(l_filePath);

            if(!l_movieExists)
            {
                m_app->debug("[MainWindow.updateApp()] Movie not already known");
                Movie l_movie;
                l_movie.setTitle(l_path.fileInfo().completeBaseName());
                l_movie.setFilePath(l_path.fileInfo().absoluteFilePath());
                l_movie.setSuffix(l_fileSuffix);
                m_app->getDatabaseManager()->insertNewMovie(l_movie);
            }
            else
            {
                m_app->debug("[MainWindow.updateApp()] Movie already known. Skipped");
            }
        }
    }
    fillMoviesList(m_app->getDatabaseManager()->getAllMovies());
    m_app->debug("[MainWindow] Exits updateApp()");
}

/**
 * @brief Fills the main pannel of the window.
 */
void MainWindow::fillMoviesList(QVector<Movie> moviesList)
{
    m_app->debug("[MainWindow] Enters fillMoviesList()");

    m_centralTreeWidget = new QTreeWidget();
    m_centralTreeWidget->setColumnCount(4);
    m_centralTreeWidget->setAlternatingRowColors(true);
    m_centralTreeWidget->setWordWrap(true);

    QStringList l_headers;
    l_headers << "Title" << "Original Title" << "Release Date" << "Path of the file";
    m_centralTreeWidget->setHeaderLabels(l_headers);
    m_centralTreeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(m_centralTreeWidget, SIGNAL(customContextMenuRequested(QPoint)), SLOT(customMenuRequested(QPoint)));

    foreach (Movie l_movie, moviesList)
    {
        QTreeWidgetItem *l_movieItem = new QTreeWidgetItem();
        l_movieItem->setData(0, Qt::UserRole, l_movie.getId());
        l_movieItem->setData(0, Qt::UserRole+1, 1);

        QLabel *l_movieTitle = new QLabel(l_movie.getTitle());
        l_movieTitle->setWordWrap(true);
        QLabel *l_movieOriginalTitle = new QLabel(l_movie.getOriginalTitle());
        l_movieOriginalTitle->setWordWrap(true);
        QLabel *l_movieReleaseDate = new QLabel(l_movie.getReleaseDate().toString("dd MMM yyyy"));
        l_movieReleaseDate->setWordWrap(true);
        QLabel *l_movieFilePath = new QLabel(l_movie.getFilePath());
        l_movieFilePath->setWordWrap(false);

        m_centralTreeWidget->insertTopLevelItem(m_centralTreeWidget->topLevelItemCount(), l_movieItem);
        m_centralTreeWidget->setItemWidget(l_movieItem, 0, l_movieTitle);
        m_centralTreeWidget->setItemWidget(l_movieItem, 1, l_movieOriginalTitle);
        m_centralTreeWidget->setItemWidget(l_movieItem, 2, l_movieReleaseDate);
        m_centralTreeWidget->setItemWidget(l_movieItem, 3, l_movieFilePath);
    }

    QObject::connect(m_centralTreeWidget,SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)),this,SLOT(startMovie(QTreeWidgetItem*, int)));

    while(!m_centralLayout->isEmpty())
    {
        delete m_centralLayout->takeAt(0);
    }
    m_centralLayout->addWidget(m_centralTreeWidget);
    m_app->debug("[MainWindow] Exits fillMoviesList()");
}

/**
 * @brief Fills the main pannel of the window with tags.
 */
void MainWindow::fillTagsList()
{
    m_app->debug("[MainWindow] Enters fillTagsList()");

    m_centralTreeWidget = new QTreeWidget();
    m_centralTreeWidget->setColumnCount(3);
    QList<QTreeWidgetItem *> l_tagsList;

    QTreeWidgetItem *l_tagItem = new QTreeWidgetItem((QTreeWidget*)0, QStringList("Without Tag"));
    QVector<Movie> l_moviesVector = m_app->getDatabaseManager()->getMoviesWithoutTag();

    l_tagItem->setData(0, Qt::UserRole, 0);
    foreach (Movie l_movie, l_moviesVector)
    {
        QTreeWidgetItem *l_movieItem = new QTreeWidgetItem();
        l_movieItem->setData(0, Qt::UserRole, l_movie.getId());
        l_movieItem->setData(0, Qt::UserRole+1, 1);
        l_movieItem->setData(0, Qt::DisplayRole, l_movie.getTitle());
        l_movieItem->setData(1, Qt::DisplayRole, l_movie.getReleaseDate());
        l_movieItem->setData(2, Qt::DisplayRole, l_movie.getFilePath());
        l_tagItem->addChild(l_movieItem);
    }
    l_tagsList.append(l_tagItem);

    QVector<Tag> l_tagsVector = m_app->getDatabaseManager()->getAllTags();
    foreach (Tag l_tag, l_tagsVector)
    {
        QTreeWidgetItem *l_tagItem = new QTreeWidgetItem((QTreeWidget*)0, QStringList(l_tag.getName()));
        l_tagItem->setData(0, Qt::UserRole, l_tag.getId());

        QVector<Movie> l_moviesVector = m_app->getDatabaseManager()->getMoviesByTag(l_tag);
        foreach (Movie l_movie, l_moviesVector)
        {
            QTreeWidgetItem *l_movieItem = new QTreeWidgetItem();
            l_movieItem->setData(0, Qt::UserRole, l_movie.getId());
            l_movieItem->setData(0, Qt::UserRole+1, 1);
            l_movieItem->setData(0, Qt::DisplayRole, l_movie.getTitle());
            l_movieItem->setData(1, Qt::DisplayRole, l_movie.getReleaseDate());
            l_movieItem->setData(2, Qt::DisplayRole, l_movie.getFilePath());
            l_tagItem->addChild(l_movieItem);
        }

        l_tagsList.append(l_tagItem);
    }
    m_centralTreeWidget->insertTopLevelItems(0, l_tagsList);
    QObject::connect(m_centralTreeWidget,SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)),this,SLOT(startMovie(QTreeWidgetItem*, int)));

    while(!m_centralLayout->isEmpty())
    {
        delete m_centralLayout->takeAt(0);
    }
    m_centralLayout->addWidget(m_centralTreeWidget);

    m_app->debug("[MainWindow] Exits fillTagsList()");
}

/**
 * @brief Fills the main pannel of the window with directors.
 */
void MainWindow::fillDirectorList()
{
    m_app->debug("[MainWindow] Enters fillDirectorList()");

    m_centralTreeWidget = new QTreeWidget();
    m_centralTreeWidget->setColumnCount(3);
    m_centralTreeWidget->setAlternatingRowColors(true);
    m_centralTreeWidget->setWordWrap(true);
    QList<QTreeWidgetItem *> l_directorsList;

    QStringList l_headers;
    l_headers << "Title" << "Release Date" << "Path of the file";
    m_centralTreeWidget->setHeaderLabels(l_headers);
    m_centralTreeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(m_centralTreeWidget, SIGNAL(customContextMenuRequested(QPoint)), SLOT(customMenuRequested(QPoint)));

    QTreeWidgetItem *l_directorItem = new QTreeWidgetItem((QTreeWidget*)0, QStringList("Without Director"));
    QVector<Movie> l_moviesVector = m_app->getDatabaseManager()->getMoviesWithoutDirector();

    l_directorItem->setData(0, Qt::UserRole, 0);
    foreach (Movie l_movie, l_moviesVector)
    {
        QTreeWidgetItem *l_movieItem = new QTreeWidgetItem();
        l_movieItem->setData(0, Qt::UserRole, l_movie.getId());
        l_movieItem->setData(0, Qt::UserRole+1, 1);
        l_directorItem->addChild(l_movieItem);

        QLabel *l_movieTitle = new QLabel(l_movie.getTitle());
        l_movieTitle->setWordWrap(true);
        QLabel *l_movieReleaseDate = new QLabel(l_movie.getReleaseDate().toString("dd MMM yyyy"));
        l_movieReleaseDate->setWordWrap(true);
        QLabel *l_movieFilePath = new QLabel(l_movie.getFilePath());
        l_movieFilePath->setWordWrap(false);

        m_centralTreeWidget->setItemWidget(l_movieItem, 0, l_movieTitle);
        m_centralTreeWidget->setItemWidget(l_movieItem, 1, l_movieReleaseDate);
        m_centralTreeWidget->setItemWidget(l_movieItem, 2, l_movieFilePath);
    }
    l_directorsList.append(l_directorItem);

    QVector<People> l_directorsVector = m_app->getDatabaseManager()->getAllDirectors();
    foreach (People l_director, l_directorsVector)
    {
        QString l_directorFullname;
        if(l_director.getFirstname() != "")
        {
            l_directorFullname = l_director.getFirstname() + " ";
        }
        l_directorFullname = l_directorFullname + l_director.getLastname();
        QTreeWidgetItem *l_directorItem = new QTreeWidgetItem((QTreeWidget*)0, QStringList(l_directorFullname));
        l_directorItem->setData(0, Qt::UserRole, l_director.getId());

        QVector<Movie> l_moviesVector = m_app->getDatabaseManager()->getMoviesByDirector(l_director);
        foreach (Movie l_movie, l_moviesVector)
        {
            QTreeWidgetItem *l_movieItem = new QTreeWidgetItem();
            l_movieItem->setData(0, Qt::UserRole, l_movie.getId());
            l_movieItem->setData(0, Qt::UserRole+1, 1);
            l_directorItem->addChild(l_movieItem);

            QLabel *l_movieTitle = new QLabel(l_movie.getTitle());
            l_movieTitle->setWordWrap(true);
            QLabel *l_movieReleaseDate = new QLabel(l_movie.getReleaseDate().toString("dd MMM yyyy"));
            l_movieReleaseDate->setWordWrap(true);
            QLabel *l_movieFilePath = new QLabel(l_movie.getFilePath());
            l_movieFilePath->setWordWrap(false);

            m_centralTreeWidget->setItemWidget(l_movieItem, 0, l_movieTitle);
            m_centralTreeWidget->setItemWidget(l_movieItem, 1, l_movieReleaseDate);
            m_centralTreeWidget->setItemWidget(l_movieItem, 2, l_movieFilePath);
        }

        l_directorsList.append(l_directorItem);
    }
    m_centralTreeWidget->insertTopLevelItems(0, l_directorsList);

    QObject::connect(m_centralTreeWidget,SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)),this,SLOT(startMovie(QTreeWidgetItem*, int)));

    while(!m_centralLayout->isEmpty())
    {
        delete m_centralLayout->takeAt(0);
    }
    m_centralLayout->addWidget(m_centralTreeWidget);

    m_app->debug("[MainWindow] Exits fillDirectorList()");
}

/**
 * @brief Fills the main pannel of the movies labeled "To Watch".
 */
void MainWindow::fillToWatchList()
{

}

/**
 * @brief Fills the main pannel of the movies of the playlist
 */
void MainWindow::fillPlaylist()
{

}

/**
 * @brief Launch a movie with the user favorite program.
 *
 * @param QTableWidgetItem* item doubleclicked by the user
 */
void MainWindow::startMovie(QTreeWidgetItem *item, int column)
{
    m_app->debug("[MainWindow] Enters startMovie()");

    // We do something only if it's a movie
    if (item->data(0, Qt::UserRole+1).toInt() == 1)
    {
        int l_movieId = item->data(0, Qt::UserRole).toInt();
        Movie l_movie = m_app->getDatabaseManager()->getOneMovieById(l_movieId);

        m_app->debug("[MainWindow.startMovie()] Opened movie: " + l_movie.getFilePath());

        QDesktopServices::openUrl(QUrl("file://" + l_movie.getFilePath(), QUrl::TolerantMode));
    }
    m_app->debug("[MainWindow] Exits startMovie()");
}

void MainWindow::fillMoviesListAll()
 {
     fillMoviesList(m_app->getDatabaseManager()->getAllMovies());
 }

/**
 * @brief Shows the context-menu where the user rightclicks
 *
 * @param QPoint position of the cursor
 */
void MainWindow::customMenuRequested(QPoint pos)
{
    m_app->debug("[MainWindow] Enters customMenuRequested()");
    QMenu *l_menu = new QMenu(this);
    QAction *l_setMetadataAction = new QAction("Set Metada", this);
    QObject::connect(l_setMetadataAction, SIGNAL(triggered()), this, SLOT(showMetadataWindow()));
    l_menu->addAction(new QAction("Nothing to do there", this));
    l_menu->addAction(l_setMetadataAction);
    l_menu->popup(m_moviesTable->viewport()->mapToGlobal(pos));
    m_app->debug("[MainWindow] Exits customMenuRequested()");
}

/**
 * @brief Shows the window to view/edit the metadata of a movie
 */
void MainWindow::showMetadataWindow()
{
    m_app->debug("[MainWindow] Enters showMetadataWindow()");
    int l_movieId = m_moviesTable->selectedItems().at(0)->data(Qt::UserRole).toInt();
    MetadataWindow *l_metadataWindow = new MetadataWindow(l_movieId);
    l_metadataWindow->show();
    m_app->debug("[MainWindow] Exits showMetadataWindow()");
}
