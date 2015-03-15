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

#include "MainWindow.h"
#include "ui_MainWindow.h"

/**
 * @brief Constructor
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow)
{
    m_app = qobject_cast<Application *>(qApp);
    Macaw::DEBUG("[MainWindow] Constructor called");

    m_ui->setupUi(this);
    m_ui->leftPannelWidget->setContentsMargins(0,1,1,0);
    this->setWindowTitle(m_app->applicationDisplayName());
    this->setWindowIcon(m_app->windowIcon());
    this->readSettings();
    connect(m_ui->mainPannel, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(on_customContextMenuRequested(const QPoint &)));
    connect(m_ui->leftPannel, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(on_customContextMenuRequested(const QPoint &)));
    connect(m_app->getDatabaseManager(),SIGNAL(orphanTagDetected(Tag&)),
            this, SLOT(askForOrphanTagDeletion(Tag&)));
    connect(m_app->getDatabaseManager(),SIGNAL(orphanPeopleDetected(People&)),
            this, SLOT(askForOrphanPeopleDeletion(People&)));
    connect(this, SIGNAL(toUpdate()),
            this, SLOT(selfUpdate()));
    m_moviesList = m_app->getDatabaseManager()->getAllMovies();
    m_leftPannelSelectedId = 0;
    fillMainPannel();
    fillLeftPannel(Macaw::isPeople, People::Director);
    Macaw::DEBUG("[MainWindow] Construction done");
}

/**
 * @brief Destructor
 */
MainWindow::~MainWindow()
{
    delete m_ui;
    Macaw::DEBUG("[MainWindow] Destructed");
}

/**
 * @brief Call and shows the settings window.
 * Calls the SettingsWindow class and waits until it closes to handle the results.
 */
void MainWindow::on_actionEdit_Settings_triggered()
{
    Macaw::DEBUG("[MainWindow] Enters showSettingsWindow()");
    SettingsWindow *l_settingsWindow = new SettingsWindow;
    l_settingsWindow->show();
    QObject::connect(l_settingsWindow,SIGNAL(closeAndSave()), this, SLOT(addNewMovies()));
    Macaw::DEBUG("[MainWindow] Exits showSettingsWindow()");
}

/**
 * @brief Fill the left Pannel.
 * Here is lot of black magic.... It has to be simplified.
 * @param typeElement
 * @param typePeople
 */
void MainWindow::fillLeftPannel(int typeElement, int typePeople = 0)
{
    m_ui->leftPannel->clear();
    m_typeElement = typeElement;
    m_typePeople = typePeople;

    if (typeElement != Macaw::isPlaylist)
    {
        QListWidgetItem *l_item = new QListWidgetItem("All");
        l_item->setData(Macaw::ObjectId, 0);
        m_ui->leftPannel->addItem(l_item);
        if (m_leftPannelSelectedId == 0)
        {
            l_item->setSelected(true);
        }
    }

    if (typeElement == Macaw::isPeople)
    {
        QListWidgetItem *l_item = new QListWidgetItem("Unknown");
        l_item->setData(Macaw::ObjectId, -1);
        l_item->setData(Macaw::ObjectType, Macaw::isPeople);
        m_ui->leftPannel->addItem(l_item);
        if (m_leftPannelSelectedId == -1)
        {
            l_item->setSelected(true);
        }

        QList<People> l_peopleList = m_app->getDatabaseManager()->getPeopleUsedByType(typePeople);
        foreach (People l_people, l_peopleList)
        {
            QString l_name(l_people.lastname());
            if (l_people.firstname() != "")
            {
                l_name = l_name + ", " + l_people.firstname();
            }
            l_name = l_name  + " (" + QString::number(l_people.birthday().year()) + ")";

            QListWidgetItem *l_item = new QListWidgetItem(l_name);
            l_item->setData(Macaw::ObjectId, l_people.id());
            l_item->setData(Macaw::ObjectType, Macaw::isPeople);
            l_item->setData(Macaw::PeopleType, typePeople);
            if (m_leftPannelSelectedId == l_people.id())
            {
                l_item->setSelected(true);
            }

            m_ui->leftPannel->addItem(l_item);
        }
    }
    else if(typeElement == Macaw::isTag)
    {
        QListWidgetItem *l_item = new QListWidgetItem("No Tag");
        l_item->setData(Macaw::ObjectId, -1);
        l_item->setData(Macaw::ObjectType, Macaw::isTag);
        m_ui->leftPannel->addItem(l_item);
        if (m_leftPannelSelectedId == -1)
        {
            l_item->setSelected(true);
        }

        QList<Tag> l_tagsList = m_app->getDatabaseManager()->getTagsUsed();
        foreach (Tag l_tag, l_tagsList)
        {
            QString l_name(l_tag.name());

            QListWidgetItem *l_item = new QListWidgetItem(l_name);
            l_item->setData(Macaw::ObjectId, l_tag.id());
            l_item->setData(Macaw::ObjectType, Macaw::isTag);

            m_ui->leftPannel->addItem(l_item);
            if (m_leftPannelSelectedId == l_tag.id())
            {
                l_item->setSelected(true);
            }
        }
    }
    else if(typeElement == Macaw::isPlaylist)
    {
        QListWidgetItem *l_item = new QListWidgetItem("To Watch");
        l_item->setData(Macaw::ObjectId, 1);
        l_item->setData(Macaw::ObjectType, Macaw::isPlaylist);
        m_ui->leftPannel->addItem(l_item);
        if (m_leftPannelSelectedId == 1)
        {
            l_item->setSelected(true);
        }

        QList<Playlist> l_playlistList = m_app->getDatabaseManager()->getAllPlaylists();
        foreach (Playlist l_playlist, l_playlistList)
        {
            QString l_name(l_playlist.name());

            QListWidgetItem *l_item = new QListWidgetItem(l_name);
            l_item->setData(Macaw::ObjectId, l_playlist.id());
            l_item->setData(Macaw::ObjectType, Macaw::isPlaylist);

            m_ui->leftPannel->addItem(l_item);
            if (m_leftPannelSelectedId == l_playlist.id())
            {
                l_item->setSelected(true);
            }
        }
    }
    this->setLeftPannelLabel();
}

/**
 * @brief Fill the Main Pannel with the movies of m_movies.
 *
 * 1. Clear the pannel
 * 2. Set the number of columns, rows, set the headers
 * 3. Fill each  cell of the pannel
 */
void MainWindow::fillMainPannel()
{
    Macaw::DEBUG("[MainWindow] Enters fillMainPannel()");

    m_ui->mainPannel->clear();
    int l_columnCount = 4;
    m_ui->mainPannel->setColumnCount(l_columnCount);
    QHeaderView* l_headerView = m_ui->mainPannel->horizontalHeader();
    l_headerView->setStretchLastSection(true);
    l_headerView->setSectionsMovable(true);
    QStringList l_headers;
    l_headers << "Title" << "Original Title" << "Release Date" << "Path of the file";
    m_ui->mainPannel->setHorizontalHeaderLabels(l_headers);
    m_ui->mainPannel->setRowCount(m_moviesList.size());
    int l_row = 0;

    foreach (Movie l_movie, m_moviesList)
    {
        int l_column = 0;
        QStringList l_movieData;
        l_movieData << l_movie.title()
                    << l_movie.originalTitle()
                    << l_movie.releaseDate().toString("dd MMM yyyy")
                    << l_movie.filePath();
        QVector<QTableWidgetItem*> l_itemList(4);
        foreach(QTableWidgetItem *l_item, l_itemList)
        {
            l_item = new QTableWidgetItem(l_movieData.at(l_column));
            l_item->setData(Macaw::ObjectId, l_movie.id());
            l_item->setData(Macaw::ObjectType, Macaw::isMovie);
            m_ui->mainPannel->setItem(l_row, l_column, l_item);
            l_column++;
        }
        l_row++;
    }

    Macaw::DEBUG("[MainWindow] Exits fillMainPannel()");
}

/**
 * @brief Slot triggered when an option from peopleBox is selected.
 * Refill all the pannels.
 *
 * @param type of People
 */
void MainWindow::on_peopleBox_activated(int type)
{
    Macaw::DEBUG("[MainWindow] Enters on_peopleBox_activated()");

    m_ui->tagsButton->setChecked(false);

    // People::typePeople begins by None
    // so we need to increment by 1
    int l_peopleType = type + 1;
    fillLeftPannel(Macaw::isPeople, l_peopleType);
    fillMainPannel();

    // We change of Macaw::typeElement,
    // so we reinitialise the selection in the leftPannel
    m_leftPannelSelectedId = 0;
}

/**
 * @brief Slot triggered when the toWatchButton is clicked.
 * Only the movies from the playlist 1 ("To Watch") are displayed.
 */
void MainWindow::on_toWatchButton_clicked()
{
    Macaw::DEBUG("[MainWindow] toWatchButton clicked");

    filterPannels();
}

/**
 * @brief Slot triggered when the tagsButton is clicked.
 * Has an effect only if the button was not already cheked.
 */
void MainWindow::on_tagsButton_clicked()
{
    Macaw::DEBUG("[MainWindow] tagsButton clicked");

    if(m_typeElement != Macaw::isTag) {

        m_ui->tagsButton->setChecked(true);
        fillLeftPannel(Macaw::isTag);

        // We change of Macaw::typeElement,
        // so we reinitialise the selection in the leftPannel
        m_leftPannelSelectedId = 0;
    }
}

/**
 * @brief Slot triggered when the context menu is requested.
 *
 * 1. Create the menu.
 * 2. Check which pannel has focus.
 *      -# if leftPannel, check that an editable element is selected
 *          then add actions on the menu and display it.
 *      -# if mainPannel, check that an editable element is selected
 *          then add actions on the menu and display it.
 *
 * @param point: coordinates of the cursor when requested
 */
void MainWindow::on_customContextMenuRequested(const QPoint &point)
{
    Macaw::DEBUG("[MainWindow] customContextMenuRequested()");
    QMenu *l_menu = new QMenu(this);

    // The left pannel must have focus, one item selected which id is not 0
    // (not to be "All" or "Unknown")
    if(m_ui->leftPannel->hasFocus()
            && m_ui->leftPannel->selectedItems().count() != 0
            && m_ui->leftPannel->selectedItems().at(0)->data(Macaw::ObjectId) != 0)
    {
        l_menu->addAction(m_ui->actionEdit_leftPannelMetadata);
        l_menu->exec(m_ui->leftPannel->mapToGlobal(point));
    }
    else if (m_ui->mainPannel->hasFocus()
               && m_ui->mainPannel->selectedItems().count() != 0)
    {


        if(m_ui->toWatchButton->isChecked())
        {
            Macaw::DEBUG("[MainWindow] In ToWatch detected");
            QAction * l_actionRemoveInToWatch = new QAction("Remove from to watch list", l_menu);

            QObject::connect(l_actionRemoveInToWatch, SIGNAL(triggered()), this, SLOT(actionRemoveInToWatch_triggered()));
            l_menu->addAction(l_actionRemoveInToWatch);
        }
        else
        {
            QMenu *l_addPlaylistMenu = new QMenu("Add to playlist");
            QAction *l_actionAddInToWatch = new QAction("To Watch",
                                                        l_addPlaylistMenu);
            l_actionAddInToWatch->setData(1);

            l_addPlaylistMenu->addAction(l_actionAddInToWatch);

            QObject::connect(l_addPlaylistMenu, SIGNAL(triggered(QAction*)),
                             this, SLOT(addPlaylistMenu_triggered(QAction*)));

            l_menu->addAction(l_actionAddInToWatch);
        }


        l_menu->addAction(m_ui->actionEdit_mainPannelMetadata);
        l_menu->exec(m_ui->mainPannel->mapToGlobal(point));
    }
}

/**
 * @brief Slot triggered when the edition of the metadata of an element from the mainPannel is asked.
 * It creates and shows a MovieDialog based on the id of this element.
 */
void MainWindow::on_actionEdit_mainPannelMetadata_triggered()
{
    Macaw::DEBUG("[MainWindow] actionEdit_Metadata_triggered()");
    int l_id = m_ui->mainPannel->selectedItems().at(0)->data(Macaw::ObjectId).toInt();

    MovieDialog *l_movieDialog = new MovieDialog(l_id);
    connect(l_movieDialog, SIGNAL(destroyed()), this, SLOT(selfUpdate()));
    l_movieDialog->show();
}

/**
 * @brief Slot triggered when the edition of the metadata of an element from the leftPannel is asked.
 *
 * 1. Check if editable
 * 2. Check the type of element selected
 * 3. Create and show a PeopleDialog (or a TagDialog or a ...) based on the id of this element
 */
void MainWindow::on_actionEdit_leftPannelMetadata_triggered()
{
    int l_id = m_ui->leftPannel->selectedItems().at(0)->data(Macaw::ObjectId).toInt();

    // It's editable only if id is not 0
    if(l_id != 0)
    {
        int l_typeElement = m_ui->leftPannel->selectedItems().at(0)->data(Macaw::ObjectType).toInt();
        if (l_typeElement == Macaw::isPeople) {
            PeopleDialog *l_movieDialog = new PeopleDialog(l_id);
            connect(l_movieDialog, SIGNAL(destroyed()), this, SLOT(selfUpdate()));
            l_movieDialog->show();
        } else if (l_typeElement == Macaw::isTag) {
            qDebug() << "Tag !";
        } else if (l_typeElement == Macaw::isPlaylist) {
            qDebug() << "Playlist !";
        }
    }
}

/**
 * @brief Slot triggered when the addition of a movie to a playlist is requeted.
 *
 * 1. Get the action id (= id of the playlist)
 * 2. Get the id of the movie concerned
 * 3. Add the movie in the playlist
 * 4. Request the update of the pannels
 */
void MainWindow::addPlaylistMenu_triggered(QAction* action)
{
    int l_actionId = action->data().toInt();
    int l_movieId = m_ui->mainPannel->selectedItems().at(0)->data(Macaw::ObjectId).toInt();
    Movie l_movie = m_app->getDatabaseManager()->getOneMovieById(l_movieId);
    Playlist l_playlist = m_app->getDatabaseManager()->getOnePlaylistById(l_actionId);
    l_playlist.addMovie(l_movie);
    m_app->getDatabaseManager()->updatePlaylist(l_playlist);
    emit(toUpdate());
}

void MainWindow::actionRemoveInToWatch_triggered()
{
    int l_movieId = m_ui->mainPannel->selectedItems().at(0)->data(Macaw::ObjectId).toInt();
    Movie l_movie = m_app->getDatabaseManager()->getOneMovieById(l_movieId);
    Playlist l_playlist = m_app->getDatabaseManager()->getOnePlaylistById(1);
    l_playlist.removeMovie(l_movie);
    m_app->getDatabaseManager()->updatePlaylist(l_playlist);
    emit toUpdate();
}


/**
 * @brief Slot triggered when DatabaseManager finds an orphan tag.
 * A QMessageBox asks the user if the tag should be delete or not.
 *
 * @param orphanTag concerned by the choice
 */
void MainWindow::askForOrphanTagDeletion(Tag &orphanTag)
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setText("The tag <b>"+ orphanTag.name() +"</b> is not used in any movie now. ");
    msgBox.setInformativeText("Do you want to delete this tag?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);

    if(msgBox.exec() == QMessageBox::Yes) {
        m_app->getDatabaseManager()->deleteTag(orphanTag);
    }
}

/**
 * @brief Slot triggered when DatabaseManager finds an orphan person.
 * A QMessageBox asks the user if the person should be delete or not.
 *
 * @param orphanPeople concerned by the choice
 */
void MainWindow::askForOrphanPeopleDeletion(People &orphanPeople)
{
    QString l_name;
    if (orphanPeople.firstname() != "") {
        l_name = orphanPeople.firstname() + " ";
    }
    l_name = l_name + orphanPeople.lastname();

    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setText("The person <b>" +l_name+ "</b> is not linked to any movie now.");
    msgBox.setInformativeText("Do you want to delete it?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);

    if(msgBox.exec() == QMessageBox::Yes) {
        m_app->getDatabaseManager()->deletePeople(orphanPeople);
    }
}

/**
 * @brief Slot triggered when an element of the MainWindow is double clicked.
 * Start the movie in the default player
 *
 * @param item which was double clicked
 */
void MainWindow::on_mainPannel_itemDoubleClicked(QTableWidgetItem *item)
{
    Macaw::DEBUG("[MainWindow] itemDoubleClicked on mainPannel");

    int l_movieId = item->data(Macaw::ObjectId).toInt();
    Movie l_movie = m_app->getDatabaseManager()->getOneMovieById(l_movieId);

    Macaw::DEBUG("[MainWindow.startMovie()] Opened movie: " + l_movie.filePath());

    QDesktopServices::openUrl(QUrl("file://" + l_movie.filePath(), QUrl::TolerantMode));
}

/**
 * @brief Slot triggered when an element of the leftPannel is clicked
 * Call `prepareMovieToDisplay()` with the id of the clicked element
 *
 * @param item which was clicked
 */
void MainWindow::on_leftPannel_clicked(const QModelIndex &item)
{
    Macaw::DEBUG("[MainWindow] itemClicked on leftPannel");
    m_leftPannelSelectedId = item.data(Macaw::ObjectId).toInt();
    this->prepareMoviesToDisplay(m_leftPannelSelectedId);
}

/**
 * @brief Slot triggered when a movie of the mainPannel is clicked
 * Call `fillMetadataPannel` to fill the pannel with the selected Movie data
 *
 * @param item which was clicked
 */
void MainWindow::on_mainPannel_clicked(const QModelIndex &index)
{
    Macaw::DEBUG("[MainWindow] mainPannel clicked");
    int l_idMovie = index.data(Macaw::ObjectId).toInt();
    Movie l_movie = m_app->getDatabaseManager()->getOneMovieById(l_idMovie);
    this->fillMetadataPannel(l_movie);

}

/**
 * @brief Prepare the movies to display in min window, based on an id and m_typeElement.
 * Then filter the pannels
 *
 * @param id of the leftPannel element
 */
void MainWindow::prepareMoviesToDisplay(int id)
{
    Macaw::DEBUG("[MainWindow] prepareMoviesToDisplay()");

    m_leftPannelSelectedId = id;
    if(m_leftPannelSelectedId == 0) {
        m_moviesList = m_app->getDatabaseManager()->getAllMovies();
    } else if(m_typeElement == Macaw::isPeople) {
        if (m_leftPannelSelectedId == -1) {
            m_moviesList = m_app->getDatabaseManager()->getMoviesWithoutPeople(m_typePeople);
        } else {
            m_moviesList = m_app->getDatabaseManager()->getMoviesByPeople(m_leftPannelSelectedId, m_typePeople);
        }
    } else if (m_typeElement == Macaw::isTag) {
        if (m_leftPannelSelectedId == -1) {
            m_moviesList = m_app->getDatabaseManager()->getMoviesWithoutTag();
        } else {
            m_moviesList = m_app->getDatabaseManager()->getMoviesByTag(m_leftPannelSelectedId);
        }
    } else if (m_typeElement == Macaw::isPlaylist) {
        Playlist l_playlist = m_app->getDatabaseManager()->getOnePlaylistById(m_leftPannelSelectedId);
        m_moviesList = l_playlist.movieList();
    }
    filterPannels();
}

/**
 * @brief Slot triggered when the window should update its pannels
 */
void MainWindow::selfUpdate()
{
    Macaw::DEBUG("[MainWindow] selfUpdate()");
    m_moviesList.clear();

    fillLeftPannel(m_typeElement, m_typePeople);

    for (int i = 0 ; i < m_ui->leftPannel->count() ; i++) {
        QListWidgetItem *l_item = m_ui->leftPannel->item(i);
        if (l_item->data(Macaw::ObjectId).toInt() == m_leftPannelSelectedId) {
            l_item->setSelected(true);
            this->prepareMoviesToDisplay(m_leftPannelSelectedId);
            break;
        }
    }
    if (m_ui->leftPannel->selectedItems().count() == 0) {
        m_leftPannelSelectedId = 0;
        fillLeftPannel(m_typeElement, m_typePeople);
        this->prepareMoviesToDisplay(m_leftPannelSelectedId);
    }

}

/**
 * @brief Set the leftPannelLabel based on m_typeElement and m_typePeople
 */
void MainWindow::setLeftPannelLabel()
{
    switch (m_typeElement)
    {
    case Macaw::isTag:
        m_ui->leftPannelLabel->setText("Tags");
        break;
    case Macaw::isPlaylist:
        m_ui->leftPannelLabel->setText("Playlists");
        break;
    case Macaw::isPeople:
        switch (m_typePeople)
        {
        case People::Director:
            m_ui->leftPannelLabel->setText("Directors");
            break;
        case People::Producer:
            m_ui->leftPannelLabel->setText("Producers");
            break;
        case People::Actor:
            m_ui->leftPannelLabel->setText("Actors");
            break;
        }
    }
}

/**
 * @brief Slot triggered when enter pressed in the search field
 * Call `filterPannels()`
 */
void MainWindow::on_searchEdit_returnPressed()
{
    Macaw::DEBUG("[MainWindow] editing finished on searchEdit");

    filterPannels();
}

/**
 * @brief Filter the pannels to follow the requirements of the search field, playlists...
 * Black magic. To be simplified
 */
void MainWindow::filterPannels()
{
    Macaw::DEBUG("[MainWindow] Enters filterPannels()");

    fillMainPannel();
    fillLeftPannel(m_typeElement, m_typePeople);

    QString l_text = m_ui->searchEdit->text();

    // mainPannel
    QList<Movie> l_authorizedMoviesList = m_app->getDatabaseManager()->getMoviesByAny(l_text);
    for (int i = 0 ; i < m_ui->mainPannel->rowCount(); i++) {
        bool l_isPresent(false);
        QTableWidgetItem *l_item = m_ui->mainPannel->item(i, 0);
        int l_itemMovieId = l_item->data(Macaw::ObjectId).toInt();

        foreach(Movie l_movie, l_authorizedMoviesList) {
             if (l_movie.id() == l_itemMovieId) {
                 l_isPresent = true;
             }
        }
        if (l_isPresent
                 && m_ui->toWatchButton->isChecked()) {
             l_isPresent = m_app->getDatabaseManager()->isMovieInPlaylist(l_itemMovieId, Playlist::ToWatch);
        }
        if (l_isPresent == false) {
            m_ui->mainPannel->removeRow(i);
            i--; // We remove a line, so we have to decrement i...
        }
    }

    // leftPannel
    if (m_typeElement == Macaw::isTag) {
        QList<Tag> l_authorizedTagList = m_app->getDatabaseManager()->getTagsByAny(l_text);
        for (int i = 2 ; i < m_ui->leftPannel->count(); i++) {
            bool l_isPresent(false);
            QListWidgetItem *l_item = m_ui->leftPannel->item(i);
            foreach(Tag l_tag, l_authorizedTagList) {
                 if (l_tag.id() == l_item->data(Macaw::ObjectId)) {
                     l_isPresent = true;
                 }
            }
            if (l_isPresent == false) {
                delete l_item;
                i--; // We remove an item so we have to decrement i
            }
        }
    } else if (m_typeElement == Macaw::isPeople) {
        QList<People> l_authorizedPeopleList = m_app->getDatabaseManager()->getPeopleByAny(l_text, m_typePeople);

        // We begin at 2 because the first item is "All", second is "Unknown"
        for (int i = 2 ; i < m_ui->leftPannel->count() ; i++) {
            bool l_isPresent(false);
            QListWidgetItem *l_item = m_ui->leftPannel->item(i);
            foreach(People l_people, l_authorizedPeopleList) {
                 if (l_people.id() == l_item->data(Macaw::ObjectId)) {
                     l_isPresent = true;
                 }
            }
            if (l_isPresent == false) {
                delete l_item;
                i--; // We remove an item so we have to decrement i
            }
        }
    } else if (m_typeElement == Macaw::isPlaylist) {
        // Don't do anything here, we want to see the name of the playlists
        // also when empty
    }

    for (int i = 0 ; i < m_ui->leftPannel->count() ; i ++) {
        QListWidgetItem *l_item = m_ui->leftPannel->item(i);
        if (m_leftPannelSelectedId == l_item->data(Macaw::ObjectId)) {
            l_item->setSelected(true);
        }
    }
}

/**
 * @brief Slot triggered to add the movies of the saved path.
 *
 * 1. Read all the paths
 * 2. For each file:
 *      -# Check that the suffix is correct
 *      -# Check that the file has not been already imported
 *      -# Import the movie in the database
 *      -# Request FetchMetadata to get the metadata on internet
 *      -# Update the movie
 * 3. Request the update of all pannels
 */
void MainWindow::addNewMovies()
{
    Macaw::DEBUG("[MainWindow] Enter addNewMovies");

    bool l_imported = false;
    QStringList l_moviesPathsList = m_app->getDatabaseManager()->getMoviesPaths(l_imported);
    foreach (QString l_moviesPath, l_moviesPathsList) {
        QDirIterator l_file(l_moviesPath, QDir::NoDotAndDotDot | QDir::Files,QDirIterator::Subdirectories);
        while (l_file.hasNext()) {
            l_file.next();
            QString l_filePath = l_file.fileInfo().absoluteFilePath();
            QString l_fileSuffix = l_file.fileInfo().suffix();
            QStringList l_authorizedSuffixList;
            l_authorizedSuffixList << "mkv"
                                   << "avi"
                                   << "mp4"
                                   << "mpg"
                                   << "flv"
                                   << "mov";
            if (l_authorizedSuffixList.contains(l_fileSuffix, Qt::CaseInsensitive)) {
                Macaw::DEBUG("[MainWindow.updateApp()] Suffix accepted");
                bool l_movieExists = m_app->getDatabaseManager()->existMovie(l_filePath);
                if (!l_movieExists) {
                    Macaw::DEBUG("[MainWindow.updateApp()] Movie not already known");
                    Movie l_movie;
                    l_movie.setTitle(l_file.fileInfo().completeBaseName());
                    l_movie.setFilePath(l_file.fileInfo().absoluteFilePath());
                    l_movie.setSuffix(l_fileSuffix);
                    m_app->getDatabaseManager()->insertNewMovie(l_movie);

                    // Currently a wainting loop is created in FetchMetadata,
                    // multithreading would be better
                    // Should be done in async
                    FetchMetadata l_fetchMetadata;
                    bool l_result = l_fetchMetadata.startProcess(l_movie);
                } else {
                    Macaw::DEBUG("[MainWindow.updateApp()] Movie already known. Skipped");
                }
            }
        }
        m_app->getDatabaseManager()->setMoviesPathImported(l_moviesPath,true);
    }

    emit(toUpdate());
    Macaw::DEBUG("[MainWindow] Exit addNewMovies");
}

/**
 * @brief Fill the Metadata pannel with the data of a given movie
 *
 * @param movie which metadata should be shown
 */
void MainWindow::fillMetadataPannel(Movie movie)
{
    Macaw::DEBUG("[MainWindow] Enter fillMetadataPannel");

    QString l_title = "<html>"+movie.title()+"<br />";
    QString l_originalTitle = "<i>"+movie.originalTitle()+"</i></br /><br />";
    QString l_directors = "<i>Directed by</i><br />";
    QString l_producers = "<i>Produced by</i><br />";
    QString l_actors = "<i>With</i><br />";

    foreach (People l_people, movie.peopleList()) {
        switch (l_people.type())
        {
        case People::Director:
            if (l_people.firstname() != "") {
                l_directors += l_people.firstname() + " ";
            }
            l_directors += l_people.lastname() + ", ";
            break;
        case People::Producer:
            if (l_people.firstname() != "") {
                l_producers += l_people.firstname() + " ";
            }
            l_producers += l_people.lastname() + ", ";
            break;
        case People::Actor:
            if(l_people.firstname() != "") {
                l_actors += l_people.firstname() + " ";
            }
            l_actors += l_people.lastname() + ", ";
            break;
        }
    }
    // Need to remove ", " = 2 chars
    if (l_directors.right(2) == ", ") {
        l_directors = l_directors.remove(l_directors.size(),-2);
    }
    l_directors += "<br /><br />";
    if (l_producers.right(2) == ", ") {
        l_producers = l_producers.remove(l_producers.size(),-2);
    }
    l_producers += "<br /><br />";
    if (l_actors.right(2) == ", ") {
        l_actors = l_actors.remove(l_actors.size(),-2);
    }
    l_actors += "<br /><br />";
    l_actors += "</html>";

    m_ui->metadataTop->setText(l_title+l_originalTitle + l_directors +l_producers + l_actors);
    m_ui->metadataPlot->setText(movie.synopsis());
    m_ui->metadataCover->hide();
    Macaw::DEBUG("[MainWindow] Exit fillMetadataPannel");
}

/**
 * @brief Slot triggered when the application is closed.
 * Overload the QMainWindow class: save the window state to QSettings
 *
 * @param event
 */
void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings l_settings("Macaw-Movies", "Macaw-Movies");
    l_settings.setValue("geometry", saveGeometry());
    l_settings.setValue("windowState", saveState());
    l_settings.setValue("mainSplitter/geometry", m_ui->mainSplitter->saveGeometry());
    l_settings.setValue("mainSplitter/state", m_ui->mainSplitter->saveState());
    QMainWindow::closeEvent(event);
}

/**
 * @brief Read the QSettings of the application to give restore the last state of the window
 */
void MainWindow::readSettings()
{
    QSettings l_settings("Macaw-Movies", "Macaw-Movies");
    restoreGeometry(l_settings.value("geometry").toByteArray());
    restoreState(l_settings.value("windowState").toByteArray());
    m_ui->mainSplitter->restoreGeometry(l_settings.value("mainSplitter/geometry").toByteArray());
    m_ui->mainSplitter->restoreState(l_settings.value("mainSplitter/state").toByteArray());
}

/**
 * @brief Slot triggered when the user clicks on the About menu.
 * Show the about menu.
 */
void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, "About " APP_NAME,
                       "<h1>" APP_NAME "</h1><br />"
                       "<em>Copyright (C) 2014 Macaw-Movies<br />"
                       "(Olivier CHURLAUD, Sébastien TOUZÉ)</em>"
                       "<br /><br />"
                       "Compiled with Qt " QT_VERSION_STR ", uses the API of <a href='http://www.themoviedb.org/'>TMDB</a>"
                       "<br /><br />"
                       "Macaw-Movies is distributed in the hope that it will be useful, "
                       "but WITHOUT ANY WARRANTY; without even the implied warranty of "
                       "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.<br />"
                       "See the GNU General Public License for more details.");
}
