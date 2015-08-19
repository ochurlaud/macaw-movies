/* Copyright (C) 2014 Macaw-Movies
 * (Olivier CHURLAUD, Sébastien TOUZÉ)
 *
 * This file is part of Macaw-Movies.
 *
 * Macaw-Movies is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Macaw-Movies is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Macaw-Movies.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "MoviesPannel.h"
#include "ui_MoviesPannel.h"

#include <QAction>
#include <QDesktopServices>
#include <QDir>
#include <QMenu>
#include <QMessageBox>
#include <QProcess>
#include <QUrl>

#include "enumerations.h"

#include "MacawDebug.h"
#include "ServicesManager.h"
#include "Dialogs/MovieDialog.h"
#include "Entities/Playlist.h"
/**
 * @brief constructor
 * @author Olivier CHURLAUD <olivier@churlaud.com>
 *
 * @param parent
 */
MoviesPannel::MoviesPannel(QWidget *parent) :
    MainPannel(parent),
    m_ui(new Ui::MoviesPannel)
{
    m_ui->setupUi(this);
    m_ui->tableWidget->setContentsMargins(0,0,0,0);
    connect(m_ui->tableWidget, SIGNAL(customContextMenuRequested(QPoint)),
                this, SLOT(on_customContextMenuRequested(QPoint)));

    this->setHeaders();
    m_ui->tableWidget->addAction(m_ui->actionDelete);
    m_ui->tableWidget->addAction(m_ui->actionEdit_mainPannelMetadata);
}

/**
 * @brief Destructor
 */
MoviesPannel::~MoviesPannel()
{
    delete m_ui;
}

/**
 * @brief Set the headers of the tableWidget
 * @author Olivier CHURLAUD <olivier@churlaud.com>
 */
void MoviesPannel::setHeaders()
{
    int l_columnCount = 4;
    m_ui->tableWidget->setColumnCount(l_columnCount);
    QHeaderView* l_headerView = m_ui->tableWidget->horizontalHeader();
    l_headerView->setStretchLastSection(true);
    l_headerView->setSectionsMovable(true);
    QStringList l_headers;
    l_headers << tr("Title") << tr("Original Title") << tr("Release Date") << tr("Path of the file");
    m_ui->tableWidget->setHorizontalHeaderLabels(l_headers);
}

/**
 * @brief Add a new row with a given Movie to the tableWidget
 * @author Olivier CHURLAUD <olivier@churlaud.com>
 *
 * @param movie
 */
void MoviesPannel::addMovieToPannel(const Movie &movie)
{
    int l_column = 0;
    int l_row = m_ui->tableWidget->rowCount();
    QStringList l_movieData;
    m_ui->tableWidget->insertRow(l_row);

    l_movieData << movie.title()
                << movie.originalTitle()
                << movie.releaseDate().toString("dd MMM yyyy")
                << movie.fileAbsolutePath();
    QVector<QTableWidgetItem*> l_itemList(4);

    foreach(QTableWidgetItem *l_item, l_itemList) {
        l_item = new QTableWidgetItem(l_movieData.at(l_column));
        l_item->setData(Macaw::ObjectId, movie.id());
        l_item->setData(Macaw::ObjectType, Macaw::isMovie);
        m_ui->tableWidget->setItem(l_row, l_column, l_item);
        l_column++;
    }
}

/**
 * @brief Fill the Main Pannel.
 *
 * 1. Clear the pannel
 * 2. Fill each cell of the pannel
 *
 * @author Olivier CHURLAUD <olivier@churlaud.com>
 * @param list of movies to show
 */
void MoviesPannel::fill(const QList<Movie> &movieList)
{
    Macaw::DEBUG_IN("[MoviesPannel] Enters fill()");

    m_ui->tableWidget->clearContents();
    m_ui->tableWidget->setRowCount(0);

    ServicesManager *servicesManager = ServicesManager::instance();
    DatabaseManager *databaseManager = servicesManager->databaseManager();

    QList<Movie> l_matchingMovieList = servicesManager->matchingMovieList();
    foreach (Movie l_movie, movieList) {
        if(l_matchingMovieList.contains(l_movie)) {
            if( (servicesManager->toWatchState()
                 && databaseManager->isMovieInPlaylist(l_movie.id(), Playlist::ToWatch)
                 ) || !servicesManager->toWatchState()
               ) {
                this->addMovieToPannel(l_movie);
            }
        }
    }

    Macaw::DEBUG_OUT("[MoviesPannel] Exits fill()");
}

/**
 * @brief Slot triggered when the context menu is requested.
 *
 * 1. Create the menu.
 * 2. Check that an editable element is selected
 * 3. Add actions on the menu
 * 4. Display it
 *
 * @author Olivier CHURLAUD <olivier@churlaud.com>
 * @param point: coordinates of the cursor when requested
 */
void MoviesPannel::on_customContextMenuRequested(const QPoint &point)
{
    Macaw::DEBUG("[MoviesPannel] customContextMenuRequested()");

    ServicesManager *servicesManager = ServicesManager::instance();
    QMenu *l_menu = new QMenu(this);
    if (!m_ui->tableWidget->selectedItems().isEmpty())
    {
        if(servicesManager->toWatchState()) {
            Macaw::DEBUG("[MainWindow] In ToWatch detected");
            m_ui->actionDelete->setText(tr("Remove from ToWatch list"));
        } else {
            QAction *l_actionAddInToWatch = new QAction(tr("To Watch"),
                                                        l_menu);
            l_actionAddInToWatch->setData(1);

            QObject::connect(l_menu, SIGNAL(triggered(QAction*)),
                                         this, SLOT(addPlaylistMenu_triggered(QAction*)));

            l_menu->addAction(l_actionAddInToWatch);
            m_ui->actionDelete->setText(tr("Move to trash"));
        }

        l_menu->addAction(m_ui->actionEdit_mainPannelMetadata);
        l_menu->addAction(m_ui->actionDelete);
        l_menu->exec(m_ui->tableWidget->mapToGlobal(point));
    }
}

/**
 * @brief Slot triggered when the edition of the metadata of an element from the mainPannel is asked.
 * It creates and shows a MovieDialog based on the id of this element.
 *
 * @author Olivier CHURLAUD <olivier@churlaud.com>
 */
void MoviesPannel::on_actionEdit_mainPannelMetadata_triggered()
{
    Macaw::DEBUG("[MoviesPannel] actionEdit_mainPannelMetadata_triggered()");
    if(!m_ui->tableWidget->selectedItems().isEmpty()) {
        int l_id = m_ui->tableWidget->selectedItems().at(0)->data(Macaw::ObjectId).toInt();

        MovieDialog *l_movieDialog = new MovieDialog(l_id);

        ServicesManager *servicesManager = ServicesManager::instance();
        connect(l_movieDialog, SIGNAL(destroyed()), servicesManager, SLOT(pannelsUpdate()));
        l_movieDialog->show();
    }
}

/**
 * @brief triggered when the user delete a movie.
 * Depending on the case it will delete the file or remove the movie from the playlist
 *
 * @author Olivier CHURLAUD <olivier@churlaud.com>
 */
void MoviesPannel::on_actionDelete_triggered()
{
    ServicesManager *servicesManager = ServicesManager::instance();
    DatabaseManager *databaseManager = servicesManager->databaseManager();

    QList<Movie> l_movieList;
    foreach (QTableWidgetItem *l_item, m_ui->tableWidget->selectedItems())
    {
        int l_movieId = l_item->data(Macaw::ObjectId).toInt();
        l_movieList.append(databaseManager->getOneMovieById(l_movieId));
    }

    if(servicesManager->toWatchState()) {
        Playlist l_playlist = databaseManager->getOnePlaylistById(1);
        removeMovieFromPlaylist(l_movieList, l_playlist);
    } else {
        moveFileToTrash(l_movieList);
    }
}

/**
 * @brief Remove a movie from a playlist
 * @author Olivier CHURLAUD <olivier@churlaud.com>
 *
 * @param movie to remove from the playlist
 * @param playlist to update
 */
void MoviesPannel::removeMovieFromPlaylist(const QList<Movie> &movieList, Playlist &playlist)
{
    DatabaseManager *databaseManager = ServicesManager::instance()->databaseManager();

    QMessageBox * l_confirmationDialog = new QMessageBox(QMessageBox::Question, tr("Remove from ToWatch list ?"),
                                                         tr("Do you want to remove this movie from the ToWatch list ?"),
                                                         QMessageBox::Yes|QMessageBox::No, this);
    l_confirmationDialog->setDefaultButton(QMessageBox::No);

    if(l_confirmationDialog->exec() == QMessageBox::Yes) {
        foreach(Movie l_movie, movieList) {
            if(databaseManager->removeMovieFromPlaylist(l_movie, playlist)) {
                Macaw::DEBUG("[MoviesPannel] Movie " + l_movie.title()
                             + " removed from playlist " + playlist.name());
            }
        }
        emit updatePannels();
    }
}

/**
 * @brief Slot triggered when an element of the MainWindow is double clicked.
 * Start the movie in the default player
 *
 * @author Olivier CHURLAUD <olivier@churlaud.com>
 *
 * @param item which was double clicked
 */
void MoviesPannel::on_tableWidget_itemDoubleClicked(QTableWidgetItem *item)
{
    Macaw::DEBUG("[MoviesPannel] itemDoubleClicked on mainPannel");

    DatabaseManager *databaseManager = ServicesManager::instance()->databaseManager();

    int l_movieId = item->data(Macaw::ObjectId).toInt();
    Movie l_movie = databaseManager->getOneMovieById(l_movieId);

    Macaw::DEBUG("[MoviesPannel.startMovie()] Opened movie: " + l_movie.fileAbsolutePath());


    if (!databaseManager->getMediaPlayerPath().isEmpty()) {
        QString l_executeMediaPlayerPath;
        #ifdef Q_OS_LINUX
            l_executeMediaPlayerPath = QString("source \"%1\"")
                                               .arg(databaseManager->getMediaPlayerPath());
        #endif
        #ifdef Q_OS_WIN
            l_executeMediaPlayerPath = QString("\"%1\"")
                                               .arg(databaseManager->getMediaPlayerPath());
        #endif
        #ifdef Q_OS_OSX
            l_executeMediaPlayerPath = QString("open \"%1\"")
                                               .arg(databaseManager->getMediaPlayerPath());
        #endif

        QProcess::startDetached(QString("%1 \"%2\"")
                                       .arg(l_executeMediaPlayerPath)
                                       .arg(l_movie.fileAbsolutePath()));
    } else {
        QDesktopServices::openUrl(QUrl("file://" + l_movie.fileAbsolutePath(), QUrl::TolerantMode));
    }
}

/**
 * @brief Slot triggered when a movie of the tableWidget is selected
 * Call `fillMetadataPannel` to fill the pannel with the selected Movie data
 *
 * @author Olivier CHURLAUD <olivier@churlaud.com>
 */
void MoviesPannel::on_tableWidget_itemSelectionChanged()
{
    Macaw::DEBUG("[MoviesPannel] mainPannel selected");
    DatabaseManager *databaseManager = ServicesManager::instance()->databaseManager();

    Movie l_movie;
    if (!m_ui->tableWidget->selectedItems().isEmpty()) {
        QTableWidgetItem *l_item = m_ui->tableWidget->selectedItems().first();

        int l_idMovie = l_item->data(Macaw::ObjectId).toInt();
        l_movie = databaseManager->getOneMovieById(l_idMovie);
    }

    emit fillMetadataPannel(l_movie);
}

/**
 * @brief Slot triggered when the addition of a movie to a playlist is requeted.
 *
 * 1. Get the action id (= id of the playlist)
 * 2. Get the id of the movie concerned
 * 3. Add the movie in the playlist
 */
void MoviesPannel::addPlaylistMenu_triggered(QAction* action)
{
    Macaw::DEBUG("[MoviesPannel] addPlaylistMenu_triggered");
    DatabaseManager *databaseManager = ServicesManager::instance()->databaseManager();

    int l_actionId = action->data().toInt();
    if (!m_ui->tableWidget->selectedItems().isEmpty() && l_actionId != 0) {
        int l_movieId = m_ui->tableWidget->selectedItems().at(0)->data(Macaw::ObjectId).toInt();
        Movie l_movie = databaseManager->getOneMovieById(l_movieId);
        Playlist l_playlist = databaseManager->getOnePlaylistById(l_actionId);
        l_playlist.addMovie(l_movie);
        databaseManager->updatePlaylist(l_playlist);
    }
}
