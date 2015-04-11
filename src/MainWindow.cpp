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

#ifdef Q_OS_WIN
    #include "windows.h"
#endif

/**
 * @brief Constructor
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow)
{
    Macaw::DEBUG("[MainWindow] Constructor called");

    m_ui->setupUi(this);
    m_ui->leftPannelWidget->setContentsMargins(0,1,1,0);
    this->readSettings();
    connect(m_ui->mainPannel, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(on_customContextMenuRequested(const QPoint &)));
    connect(m_ui->leftPannel, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(on_customContextMenuRequested(const QPoint &)));
    m_ui->mainPannel->addAction(m_ui->actionDelete);
    m_ui->mainPannel->addAction(m_ui->actionEdit_mainPannelMetadata);
    m_ui->leftPannel->addAction(m_ui->actionEdit_leftPannelMetadata);

    m_leftPannelSelectedId = 0;
    m_typeElement =  Macaw::isPeople;
    m_typePeople = People::Director;

    this->updatePannels();

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
 */
void MainWindow::fillLeftPannel()
{
    Macaw::DEBUG("[MainWindow] Enters fillLeftPannel()");
    m_ui->leftPannel->clear();
    m_leftElementsIdList.clear();
    DatabaseManager *databaseManager = DatabaseManager::instance();

    if(m_typeElement != Macaw::isPlaylist) {
        QListWidgetItem *l_item = new QListWidgetItem("All");
        l_item->setData(Macaw::ObjectId, 0);
        m_ui->leftPannel->addItem(l_item);

        if (m_leftPannelSelectedId == 0) {
            l_item->setSelected(true);
        }
    }
//Create function for the following (code is repeated)
   // ??? Templates ???
    if (m_typeElement == Macaw::isPeople) {
        foreach(Movie l_movie, m_authorizedMoviesList) {
            if( (m_ui->toWatchButton->isChecked()
                 && databaseManager->isMovieInPlaylist(l_movie.id(), Playlist::ToWatch)
                 ) || !m_ui->toWatchButton->isChecked()
               ) {
                if(l_movie.peopleList().isEmpty()
                   && !m_leftElementsIdList.contains(-1)
                  ) {
                    m_leftElementsIdList.append(-1);
                }
                foreach(People l_people, l_movie.peopleList(m_typePeople)) {
                    if(!m_leftElementsIdList.contains(l_people.id())) {
                        m_leftElementsIdList.append(l_people.id());
                    }
                }
            }
        }
    } else if(m_typeElement == Macaw::isTag) {
        foreach(Movie l_movie, m_authorizedMoviesList) {
            if( (m_ui->toWatchButton->isChecked()
                 && databaseManager->isMovieInPlaylist(l_movie.id(), Playlist::ToWatch)
                 ) || !m_ui->toWatchButton->isChecked()
               ) {
                if(l_movie.tagList().isEmpty()
                   && !m_leftElementsIdList.contains(-1)
                  ) {
                    m_leftElementsIdList.append(-1);
                }
                foreach(Tag l_tag, l_movie.tagList()) {
                    if(!m_leftElementsIdList.contains(l_tag.id())) {
                        m_leftElementsIdList.append(l_tag.id());
                    }
                }
            }
        }
    }

    if (m_typeElement == Macaw::isPeople) {
        foreach(int l_objectId, m_leftElementsIdList) {
            if(l_objectId == -1) {
                QListWidgetItem *l_item = new QListWidgetItem("Unknown");
                l_item->setData(Macaw::ObjectId, -1);
                l_item->setData(Macaw::ObjectType, Macaw::isPeople);
                m_ui->leftPannel->addItem(l_item);
                if (m_leftPannelSelectedId == -1) {
                    l_item->setSelected(true);
                }
            } else {
                People l_people = databaseManager->getOnePeopleById(l_objectId);
                QString l_name(l_people.name());

                QListWidgetItem *l_item = new QListWidgetItem(l_name);
                l_item->setData(Macaw::ObjectId, l_people.id());
                l_item->setData(Macaw::ObjectType, Macaw::isPeople);
                l_item->setData(Macaw::PeopleType, m_typePeople);
                if (m_leftPannelSelectedId == l_people.id())
                {
                    l_item->setSelected(true);
                }

                m_ui->leftPannel->addItem(l_item);
            }
        }
    } else if(m_typeElement == Macaw::isTag) {
        foreach(int l_objectId, m_leftElementsIdList) {
            if(l_objectId == -1) {
                QListWidgetItem *l_item = new QListWidgetItem("No Tag");
                l_item->setData(Macaw::ObjectId, -1);
                l_item->setData(Macaw::ObjectType, Macaw::isTag);
                m_ui->leftPannel->addItem(l_item);
                if (m_leftPannelSelectedId == -1)
                {
                    l_item->setSelected(true);
                }
            } else {
                Tag l_tag = databaseManager->getOneTagById(l_objectId);
                QString l_name(l_tag.name());

                QListWidgetItem *l_item = new QListWidgetItem(l_name);
                l_item->setData(Macaw::ObjectId, l_tag.id());
                l_item->setData(Macaw::ObjectType, Macaw::isTag);
                if (m_leftPannelSelectedId == l_tag.id())
                {
                    l_item->setSelected(true);
                }

                m_ui->leftPannel->addItem(l_item);
            }
        }
    }
    if(m_ui->leftPannel->selectedItems().isEmpty()) {
        m_ui->leftPannel->item(0)->setSelected(true);
    }
    this->setLeftPannelLabel();
    Macaw::DEBUG("[MainWindow] Exits fillLeftPannel()");
}

/**
 * @brief Fill the Main Pannel.
 *
 * 1. Clear the pannel
 * 2. Set the number of columns, rows, set the headers
 * 3. Fill each cell of the pannel
 */
void MainWindow::fillMainPannel()
{
    Macaw::DEBUG("[MainWindow] Enters fillMainPannel()");

    m_ui->mainPannel->clearContents();
    m_ui->mainPannel->setRowCount(0);
    m_displayedMoviesList.clear();
    DatabaseManager *databaseManager = DatabaseManager::instance();

    // Create function setHeaders
    // And call it only the first time or when settings changed
    int l_columnCount = 4;
    m_ui->mainPannel->setColumnCount(l_columnCount);
    QHeaderView* l_headerView = m_ui->mainPannel->horizontalHeader();
    l_headerView->setStretchLastSection(true);
    l_headerView->setSectionsMovable(true);
    QStringList l_headers;
    l_headers << "Title" << "Original Title" << "Release Date" << "Path of the file";
    m_ui->mainPannel->setHorizontalHeaderLabels(l_headers);
    // End function setHeaders

    QList<Movie> l_moviesList = moviesToDisplay(m_leftPannelSelectedId);
    int l_row = 0;
    foreach (Movie l_movie, m_authorizedMoviesList) {
        if(l_moviesList.contains(l_movie)) {
            if( (m_ui->toWatchButton->isChecked()
                 && databaseManager->isMovieInPlaylist(l_movie.id(), Playlist::ToWatch)
                 ) || !m_ui->toWatchButton->isChecked()
               ) {
               //Create function `ADD MOVIE TO PANNEL`
                int l_column = 0;
                QStringList l_movieData;
                m_ui->mainPannel->insertRow(m_ui->mainPannel->rowCount());

                m_displayedMoviesList.append(l_movie);
                l_movieData << l_movie.title()
                            << l_movie.originalTitle()
                            << l_movie.releaseDate().toString("dd MMM yyyy")
                            << l_movie.filePath();
                QVector<QTableWidgetItem*> l_itemList(4);
                foreach(QTableWidgetItem *l_item, l_itemList) {
                    l_item = new QTableWidgetItem(l_movieData.at(l_column));
                    l_item->setData(Macaw::ObjectId, l_movie.id());
                    l_item->setData(Macaw::ObjectType, Macaw::isMovie);
                    m_ui->mainPannel->setItem(l_row, l_column, l_item);
                    l_column++;
                }
                l_row++;
            }
        }
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
    m_typeElement = Macaw::isPeople;
    m_typePeople = l_peopleType;

    // We change of Macaw::typeElement,
    // so we reinitialise the selection in the leftPannel
    m_leftPannelSelectedId = 0;

    this->updatePannels();
}

/**
 * @brief Slot triggered when the toWatchButton is clicked.
 * Only the movies from the playlist 1 ("To Watch") are displayed.
 */
void MainWindow::on_toWatchButton_clicked()
{
    Macaw::DEBUG("[MainWindow] toWatchButton clicked");

    this->updatePannels();
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
        m_typeElement =  Macaw::isTag;

        // We change of Macaw::typeElement,
        // so we reinitialise the selection in the leftPannel
        m_leftPannelSelectedId = 0;

        this->updatePannels();
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

    // The left pannel must have focus, one item selected which id is not 0 or -1
    // (not to be "All" or "Unknown")
    if(m_ui->leftPannel->hasFocus()
            && !m_ui->leftPannel->selectedItems().isEmpty()
            && m_ui->leftPannel->selectedItems().at(0)->data(Macaw::ObjectId) > 0)
    {
        l_menu->addAction(m_ui->actionEdit_leftPannelMetadata);
        l_menu->exec(m_ui->leftPannel->mapToGlobal(point));
    }
    else if (m_ui->mainPannel->hasFocus()
               && !m_ui->mainPannel->selectedItems().isEmpty())
    {
        if(m_ui->toWatchButton->isChecked()) {
            Macaw::DEBUG("[MainWindow] In ToWatch detected");
            m_ui->actionDelete->setText("Remove from ToWatch list");
        } else {
            QAction *l_actionAddInToWatch = new QAction("To Watch",
                                                        l_menu);
            l_actionAddInToWatch->setData(1);

            QObject::connect(l_menu, SIGNAL(triggered(QAction*)),
                                         this, SLOT(addPlaylistMenu_triggered(QAction*)));

            l_menu->addAction(l_actionAddInToWatch);
            m_ui->actionDelete->setText("Move to trash");
        }

        l_menu->addAction(m_ui->actionEdit_mainPannelMetadata);
        l_menu->addAction(m_ui->actionDelete);
        l_menu->exec(m_ui->mainPannel->mapToGlobal(point));
    }
}

/**
 * @brief Slot triggered when the edition of the metadata of an element from the mainPannel is asked.
 * It creates and shows a MovieDialog based on the id of this element.
 */
void MainWindow::on_actionEdit_mainPannelMetadata_triggered()
{
    Macaw::DEBUG("[MainWindow] actionEdit_mainPannelMetadata_triggered()");
    if(!m_ui->mainPannel->selectedItems().isEmpty()) {
        int l_id = m_ui->mainPannel->selectedItems().at(0)->data(Macaw::ObjectId).toInt();

        MovieDialog *l_movieDialog = new MovieDialog(l_id);
        connect(l_movieDialog, SIGNAL(destroyed()), this, SLOT(selfUpdate()));
        l_movieDialog->show();
    }
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
    Macaw::DEBUG("[MainWindow] actionEdit_leftPannelMetadata_triggered()");
    // The left pannel must have one item selected which id is not -1 or 0
    // (not to be "All" or "Unknown")
    if(!m_ui->leftPannel->selectedItems().isEmpty()) {
        int l_id = m_ui->leftPannel->selectedItems().at(0)->data(Macaw::ObjectId).toInt();
        // It's editable only if id is not 0 or -1
        if(l_id > 0) {
            Macaw::DEBUG("Element is editable");
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
}


/**
 * @brief triggered when the user delete a movie.
 * Depending on the case it will delete the file or remove the movie from the playlist
 */
void MainWindow::on_actionDelete_triggered()
{
    int l_movieId = m_ui->mainPannel->selectedItems().at(0)->data(Macaw::ObjectId).toInt();
    DatabaseManager *databaseManager = DatabaseManager::instance();

    Movie l_movie = databaseManager->getOneMovieById(l_movieId);

    if(m_ui->toWatchButton->isChecked()) {
        Playlist l_playlist = databaseManager->getOnePlaylistById(1);
        removeMovieFromPlaylist(l_movie, l_playlist);
    } else {
        moveFileToTrash(l_movie);
    }
}

/**
 * @brief Remove a movie from a playlist
 * @param movie to remove from the playlist
 * @param playlist to update
 */
void MainWindow::removeMovieFromPlaylist(Movie &movie, Playlist &playlist)
{
    DatabaseManager *databaseManager = DatabaseManager::instance();
    QMessageBox * l_confirmationDialog = new QMessageBox(QMessageBox::Question, "Remove from ToWatch list ?",
                                                         "Do you want to remove this movie from the ToWatch list ?",
                                                         QMessageBox::Yes|QMessageBox::No, this);
    l_confirmationDialog->setDefaultButton(QMessageBox::No);

    if(l_confirmationDialog->exec() == QMessageBox::Yes) {
        if(databaseManager->removeMovieFromPlaylist(movie, playlist))
        {
            Macaw::DEBUG("[MainWindow] Movie removed from playlist "+ playlist.name());
            this->updatePannels();
        }
    }
}

/**
 * @brief move the specified movie's file to trash bin
 * This function tries to move a file to trash,  as trash bin is not implemeted the same way amond OSes this could fail.
 * If the function cannot find the trash bin, it asks the user for the file permanant deletion.
 * @param movie to delete
 */
bool MainWindow::moveFileToTrash(Movie &movie)
{
    DatabaseManager *databaseManager = DatabaseManager::instance();
    QMessageBox * l_confirmationDialog = new QMessageBox(QMessageBox::Warning, "Move to trash? ",
                                                         "Move this file to trash? All data in Macaw-Movies specific to this movie will be deleted, this cannot be undone. ",
                                                         QMessageBox::Yes|QMessageBox::No, this);

    QFile * movieFileToDelete = new QFile(movie.filePath());
    l_confirmationDialog->setDefaultButton(QMessageBox::No);

    if(l_confirmationDialog->exec() == QMessageBox::Yes)
    {
        bool l_successfullyDeleted = false;

#ifdef Q_OS_LINUX

        //try to locate the trash folder
        //Note : I suppose here that the file is in the computer HD => to improve
        Macaw::DEBUG("[MainWindow] Trying to locate Linux trash folder");
        QString l_trashbinDirectory(QDir::home().path().append(QDir::separator())
                                                .append(".local/share/Trash"));

        if(!QDir(l_trashbinDirectory).exists()) {
            l_trashbinDirectory = QDir::home().path().append(QDir::separator()).append(".trash");
            if(!QDir(l_trashbinDirectory).exists()) {
                l_trashbinDirectory = QString(getenv("XDG_DATA_HOME")).append("/Trash");
                if(!QDir(l_trashbinDirectory).exists()) {
                    QMessageBox * l_errorMovingToTrash = new QMessageBox(QMessageBox::Critical, "Trash not found",
                                                                         "No trash file have been found on your system, maybe your desktop environement do not support trash bin? \n You can insted PERMANANTLY delete this file. Do you want to delete this file from tyour disk ? This action cannot be undone. ",
                                                                         QMessageBox::Yes|QMessageBox::No, this);
                    if(l_errorMovingToTrash->exec() == QMessageBox::Yes) {
                        l_successfullyDeleted = permanentlyDeleteFile(movieFileToDelete);
                    }
                } else {
                    l_successfullyDeleted = linux_moveFileToTrash(movie.filePath(), l_trashbinDirectory);
                }
            } else {
                l_successfullyDeleted = linux_moveFileToTrash(movie.filePath(), l_trashbinDirectory);
            }
        } else {
            l_successfullyDeleted = linux_moveFileToTrash(movie.filePath(), l_trashbinDirectory);
        }
#endif

#ifdef Q_OS_WIN
        windows_moveFileToTrash(movie.filePath())
#endif

#ifdef Q_OS_OSX

#endif

        if(l_successfullyDeleted) {
            if(!databaseManager->deleteMovie(movie)) {
                QMessageBox * msgBox = new QMessageBox(QMessageBox::Critical, "Error deleting",
                                                    "Error deleting the movie from the database. ",
                                                    QMessageBox::Ok, this);
                msgBox->exec();
                return false;
            }
        }

        this->updatePannels();
    }
    return true;
}

/**
 * @brief Moves a movie file to trash bin for Linux.
 *
 * Moving a file to trash on GNU/Linux is made moving the file in a <trash folder>/files directory
 * and creating and info file in <trash folder>/info
 *
 * @param moviePath the path to the movie's file
 * @param trashbinDirectory the path to trash bin on the Linux distro
 *
 * @return bool true if the file have been successfully moved to trash or permanently deleted
 *
 */
bool MainWindow::linux_moveFileToTrash(QString movieFilePath, QString trashbinDirectory) {
    QDir l_dir;
    QFile l_movieFile(movieFilePath);
    QFileInfo l_movieFileInfo(movieFilePath);
    QString l_trashFilesPath(trashbinDirectory + "/files/");    //folder to put the deleted file
    QString l_trashInfoPath(trashbinDirectory + "/info/");      //folder to put the info about deleted file

    QString l_trashName = l_movieFileInfo.fileName();
    QFileInfo l_targetMovieFileInfo(l_trashInfoPath + l_trashName + ".trashinfo");
    QFileInfo l_targetMovieFileFiles(l_trashFilesPath + l_trashName);
    int l_count = 0;
    while(l_targetMovieFileInfo.exists() || l_targetMovieFileFiles.exists()) {
        l_count++;

        //Change the filename of "movie.suffix" to "movie(i).suffix"
        l_trashName = (l_movieFileInfo.completeBaseName().isEmpty() ? "" : l_movieFileInfo.completeBaseName())
                + "(" + QString::number(l_count) + ")" +
                (l_movieFileInfo.suffix().isEmpty() ? "" : "." + l_movieFileInfo.suffix());

        l_targetMovieFileInfo = QFileInfo(l_trashInfoPath + l_trashName + ".trashinfo");
        l_targetMovieFileFiles = QFileInfo(l_trashFilesPath + l_trashName);
    }

    QFile l_trashInfoFile(l_targetMovieFileInfo.absoluteFilePath());  //file containing the info about deleted file

    Macaw::DEBUG("[MainWindow] Trash folder found here: "+trashbinDirectory);
    Macaw::DEBUG("[MainWindow] Trash name for the file: "+l_trashName);

    if(!l_movieFileInfo.exists() || !l_movieFileInfo.isFile()) {
        QMessageBox * l_msgBoxErrorFileNotExist = new QMessageBox(QMessageBox::Critical, "Error deleting",
                                            "File does not exists or is not a file, it cannot be deleted. ",
                                            QMessageBox::Ok, this);
        l_msgBoxErrorFileNotExist->exec();
        return false;
    }

    Macaw::DEBUG("[MainWindow] Writting info to: " + l_targetMovieFileInfo.absoluteFilePath());

    if(l_trashInfoFile.open(QIODevice::WriteOnly)) {
        Macaw::DEBUG("[MainWindow] Moving file FROM: "+movieFilePath+" TO: " + l_targetMovieFileFiles.absoluteFilePath());
        bool moveFile = l_dir.rename(movieFilePath, l_targetMovieFileFiles.absoluteFilePath());

        if(moveFile) {
            l_trashInfoFile.write("[Trash Info]\n");
            QString l_path = "Path=" + movieFilePath + "\n";
            QString l_deletionDate = "DeletionDate=" + QDateTime::currentDateTime().toString(Qt::ISODate) + "\n";

            l_trashInfoFile.write((char *)(l_path.toStdString().c_str()));
            l_trashInfoFile.write((char *)(l_deletionDate.toStdString().c_str()));
            l_trashInfoFile.close();

            return true;
        }
        else {
            Macaw::DEBUG("[MainWindow] Failled to move file to trash");
            l_trashInfoFile.close();
            l_trashInfoFile.remove();
        }
    }
    else {
        Macaw::DEBUG("[MainWindow] Failled to create and open the file's' trash info");
    }

    QMessageBox * l_msgBoxErrorMovingToTrash = new QMessageBox(QMessageBox::Warning, "Error moving file to trash",
                                        "Something went wrong when moving the file to the trash. Do you want to permanently delete it instead? ",
                                        QMessageBox::Yes|QMessageBox::No, this);

    if(l_msgBoxErrorMovingToTrash->exec() == QMessageBox::Yes)
    {
        bool l_permanentlyDelete = permanentlyDeleteFile(&l_movieFile);
        return l_permanentlyDelete;
    }

    return false;
}

/**
 * @brief Moves a movie file to trash bin for MS Windows.
 *
 * @param moviePath the path to the movie's file
 * @return bool true if the file have been successfully moved to trash
 *
 */
#ifdef Q_OS_WIN
bool MainWindow::windows_moveFileToTrash(QString movieFilePath) {
    QFileInfo fileinfo( movieFilePath );

    if( !fileinfo.exists() ) {
        QMessageBox * l_msgBoxErrorFileNotExist = new QMessageBox(QMessageBox::Critical, "Error deleting",
                                            "File does not exists or is not a file, it cannot be deleted. ",
                                            QMessageBox::Ok, this);
        l_msgBoxErrorFileNotExist->exec();
        return false;
    }

    WCHAR from[ MAX_PATH ];
    memset( from, 0, sizeof( from ));
    int l = fileinfo.absoluteFilePath().toWCharArray( from );
    Q_ASSERT( 0 <= l && l < MAX_PATH );
    from[ l ] = '\0';
    SHFILEOPSTRUCT fileop;
    memset( &fileop, 0, sizeof( fileop ) );
    fileop.wFunc = FO_DELETE;
    fileop.pFrom = from;
    fileop.fFlags = FOF_ALLOWUNDO | FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SILENT;
    int rv = SHFileOperation( &fileop );

    if( rv != 0 ){
        Macaw::DEBUG("Moving file to trash failed with: " + QString::number(rv));
        QMessageBox * l_msgBoxErrorMovingToTrash = new QMessageBox(QMessageBox::Warning, "Error moving file to trash",
                                            "Something went wrong when moving the file to the trash. Do you want to permanently delete it instead? ",
                                            QMessageBox::Yes|QMessageBox::No, this);

        if(l_msgBoxErrorMovingToTrash->exec() == QMessageBox::Yes)
        {
            bool l_permanentlyDelete = permanentlyDeleteFile(&l_movieFile);
            return l_permanentlyDelete;
        }
    }
}
#else
bool MainWindow::windows_moveFileToTrash(QString movieFilePath) {return false;}
#endif


/**
 * @brief Moves a movie file to trash bin for Mac OSX.
 *
 * Moving a file to trash on Mac OSX is made moving the file in <User's home>/.Trash directory
 * This function do not allow to restore file as it do not uses the Apple API
 *
 * @param moviePath the path to the movie's file
 * @return bool true if the file have been successfully moved to trash or permanently deleted
 *
 */
bool MainWindow::macosx_moveFileToTrash(QString movieFilePath) {
    QDir l_dir;
    QFile l_movieFile(movieFilePath);
    QFileInfo l_movieFileInfo(movieFilePath);
    QString l_trashFilesPath(QDir::home().path().append(QDir::separator()).append(".Trash/"));    //folder to put the deleted file

    QString l_trashName = l_movieFileInfo.fileName();
    QFileInfo l_targetMovieFileFiles(l_trashFilesPath + l_trashName);
    int l_count = 0;
    while(l_targetMovieFileFiles.exists()) {
        l_count++;

        //Change the filename of "movie.suffix" to "movie(i).suffix"
        l_trashName = (l_movieFileInfo.completeBaseName().isEmpty() ? "" : l_movieFileInfo.completeBaseName())
                + "(" + QString::number(l_count) + ")" +
                (l_movieFileInfo.suffix().isEmpty() ? "" : "." + l_movieFileInfo.suffix());

        l_targetMovieFileFiles = QFileInfo(l_trashFilesPath + l_trashName);
    }

    Macaw::DEBUG("[MainWindow] Trash name for the file: "+l_trashName);

    if(!l_movieFileInfo.exists() || !l_movieFileInfo.isFile()) {
        QMessageBox * l_msgBoxErrorFileNotExist = new QMessageBox(QMessageBox::Critical, "Error deleting",
                                            "File does not exists or is not a file, it cannot be deleted. ",
                                            QMessageBox::Ok, this);
        l_msgBoxErrorFileNotExist->exec();
        return false;
    }

    Macaw::DEBUG("[MainWindow] Moving file FROM: "+movieFilePath+" TO: " + l_targetMovieFileFiles.absoluteFilePath());
    bool moveFile = l_dir.rename(movieFilePath, l_targetMovieFileFiles.absoluteFilePath());

    if(moveFile) {
        return true;
    }
    else {
        Macaw::DEBUG("[MainWindow] Failled to move file to trash");
    }

    QMessageBox * l_msgBoxErrorMovingToTrash = new QMessageBox(QMessageBox::Warning, "Error moving file to trash",
                                        "Something went wrong when moving the file to the trash. Do you want to permanently delete it instead? ",
                                        QMessageBox::Yes|QMessageBox::No, this);

    if(l_msgBoxErrorMovingToTrash->exec() == QMessageBox::Yes)
    {
        bool l_permanentlyDelete = permanentlyDeleteFile(&l_movieFile);
        return l_permanentlyDelete;
    }

    return false;
}

/**
 * @brief Permanetly delete a file.
 *
 * @param movieFileToDelete pointer to a QFile representing the file to delete
 * @return bool true if the file have been successfully deleted
 *
 */
bool MainWindow::permanentlyDeleteFile(QFile * movieFileToDelete) {
        Macaw::DEBUG("[MainWindow] Permanently deleting file");
        if(!movieFileToDelete->remove())
        {
            QMessageBox * l_msgBoxErrorDeleting = new QMessageBox(QMessageBox::Critical, "Error deleting",
                                                "Error deleting the file. ",
                                                QMessageBox::Ok, this);
            l_msgBoxErrorDeleting->exec();
            return false;
        }
        return true;
}


/**
 * @brief Slot triggered when the addition of a movie to a playlist is requeted.
 *
 * 1. Get the action id (= id of the playlist)
 * 2. Get the id of the movie concerned
 * 3. Add the movie in the playlist
 */
void MainWindow::addPlaylistMenu_triggered(QAction* action)
{
    Macaw::DEBUG("[MainWindow] addPlaylistMenu_triggered");
    DatabaseManager *databaseManager = DatabaseManager::instance();
    int l_actionId = action->data().toInt();
    if (!m_ui->mainPannel->selectedItems().isEmpty() && l_actionId != 0) {
        int l_movieId = m_ui->mainPannel->selectedItems().at(0)->data(Macaw::ObjectId).toInt();
        Movie l_movie = databaseManager->getOneMovieById(l_movieId);
        Playlist l_playlist = databaseManager->getOnePlaylistById(l_actionId);
        l_playlist.addMovie(l_movie);
        databaseManager->updatePlaylist(l_playlist);
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
    DatabaseManager *databaseManager = DatabaseManager::instance();
    Macaw::DEBUG("[MainWindow] itemDoubleClicked on mainPannel");

    int l_movieId = item->data(Macaw::ObjectId).toInt();
    Movie l_movie = databaseManager->getOneMovieById(l_movieId);

    Macaw::DEBUG("[MainWindow.startMovie()] Opened movie: " + l_movie.filePath());

    QDesktopServices::openUrl(QUrl("file://" + l_movie.filePath(), QUrl::TolerantMode));
}

/**
 * @brief Slot triggered when an element of the leftPannel is selected
 * Fill the Main Pannel according to the selected element
 */
void MainWindow::on_leftPannel_itemSelectionChanged()
{
    Macaw::DEBUG("[MainWindow] item selected on leftPannel");
    if (m_ui->leftPannel->selectedItems().count() > 0) {
        QListWidgetItem *l_item = m_ui->leftPannel->selectedItems().first();

        m_leftPannelSelectedId = l_item->data(Macaw::ObjectId).toInt();
        this->fillMainPannel();
    }
}

/**
 * @brief Slot triggered when a movie of the mainPannel is selected
 * Call `fillMetadataPannel` to fill the pannel with the selected Movie data
 */
void MainWindow::on_mainPannel_itemSelectionChanged()
{
    Macaw::DEBUG("[MainWindow] mainPannel selected");
    DatabaseManager *databaseManager = DatabaseManager::instance();

    Movie l_movie;
    if (!m_ui->mainPannel->selectedItems().isEmpty()) {
        QTableWidgetItem *l_item = m_ui->mainPannel->selectedItems().first();

        int l_idMovie = l_item->data(Macaw::ObjectId).toInt();
        l_movie = databaseManager->getOneMovieById(l_idMovie);
    }

    this->fillMetadataPannel(l_movie);
}

/**
 * @brief Returns the movies to display in mainWindow, based on an id and m_typeElement.
 *
 * @param id of the leftPannel element
 * @return QList of movies to display
 */
QList<Movie> MainWindow::moviesToDisplay(int id)
{
    Macaw::DEBUG("[MainWindow] prepareMoviesToDisplay()");

    DatabaseManager *databaseManager = DatabaseManager::instance();

    m_leftPannelSelectedId = id;
    if(m_leftPannelSelectedId == 0) {

        return databaseManager->getAllMovies();
    } else if(m_typeElement == Macaw::isPeople) {
        if (m_leftPannelSelectedId == -1) {

            return databaseManager->getMoviesWithoutPeople(m_typePeople);
        } else {

            return databaseManager->getMoviesByPeople(m_leftPannelSelectedId, m_typePeople);
        }
    } else if (m_typeElement == Macaw::isTag) {
        if (m_leftPannelSelectedId == -1) {

            return databaseManager->getMoviesWithoutTag();
        } else {

            return databaseManager->getMoviesByTag(m_leftPannelSelectedId);
        }
    }
    QList<Movie> l_emptyList;

    return  l_emptyList;
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
 * Call `updatePannels()`
 */
void MainWindow::on_searchEdit_returnPressed()
{
    Macaw::DEBUG("[MainWindow] editing finished on searchEdit");

    this->updatePannels();
}

/**
 * @brief Slot triggered when Dialogs are closed in order to update the pannels
 * Call `updatePannels()`
 */
void MainWindow::selfUpdate()
{
    Macaw::DEBUG("[MainWindow] selfUpdate() triggered");

    this->updatePannels();
}

/**
 * @brief Filter the pannels to follow the requirements of the search field, playlists...
 * Black magic. To be simplified
 */
void MainWindow::updatePannels()
{
    Macaw::DEBUG("[MainWindow] Enters updatePannels()");

    DatabaseManager *databaseManager = DatabaseManager::instance();
    QString l_text = m_ui->searchEdit->text();
    m_authorizedMoviesList = databaseManager->getMoviesByAny(l_text);
    fillLeftPannel();
    fillMainPannel();
    Macaw::DEBUG("[MainWindow] Exits updatePannels()");
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

    DatabaseManager *databaseManager = DatabaseManager::instance();
    bool l_imported = false;
    int l_addedCount(0);
    QStringList l_moviesPathsList = databaseManager->getMoviesPaths(l_imported);

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
                bool l_movieExists = databaseManager->existMovie(l_filePath);
                if (!l_movieExists) {
                    Macaw::DEBUG("[MainWindow.updateApp()] Movie not already known");
                    Movie l_movie;
                    l_movie.setTitle(l_file.fileInfo().completeBaseName());
                    l_movie.setFilePath(l_file.fileInfo().absoluteFilePath());
                    l_movie.setSuffix(l_fileSuffix);
                    databaseManager->insertNewMovie(l_movie);
                    l_addedCount++;
                    if(l_addedCount == 5) {
                        emit startFetchingMetadata();
                    }
                } else {
                    Macaw::DEBUG("[MainWindow.updateApp()] Movie already known. Skipped");
                }
            }
        }
        databaseManager->setMoviesPathImported(l_moviesPath,true);
    }
    emit startFetchingMetadata();
    this->updatePannels();
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
            l_directors += l_people.name() + ", ";
            break;
        case People::Producer:
            l_producers += l_people.name() + ", ";
            break;
        case People::Actor:
            l_actors += l_people.name() + ", ";
            break;
        }
    }
    // Need to remove ", " = 2 chars
    if (l_directors.right(2) == ", ") {
        l_directors = l_directors.remove(l_directors.size()-2, 2);

    }
    l_directors += "<br /><br />";
    if (l_producers.right(2) == ", ") {
        l_producers = l_producers.remove(l_producers.size()-2,2);
    }
    l_producers += "<br /><br />";
    if (l_actors.right(2) == ", ") {
        l_actors = l_actors.remove(l_actors.size()-2,2);
    }
    l_actors += "<br /><br />";
    l_actors += "</html>";

    m_ui->metadataTop->setText(l_title+l_originalTitle + l_directors +l_producers + l_actors);
    m_ui->metadataPlot->setText(movie.synopsis());
    QString l_posterPath = qApp->property("postersPath").toString()
            + movie.posterPath();
    QPixmap l_poster(l_posterPath);
    m_ui->metadataCover->setPixmap(l_poster);

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
