#include "MoviesPannel.h"
#include "ui_MoviesPannel.h"

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
    connect(m_ui->tableWidget, SIGNAL(customContextMenuRequested(const QPoint &)),
                this, SLOT(on_customContextMenuRequested(const QPoint &)));

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
                << movie.filePath();
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
void MoviesPannel::fill(const QList<Movie> &moviesList)
{
    Macaw::DEBUG_IN("[MoviesPannel] Enters fill()");

    m_ui->tableWidget->clearContents();
    m_ui->tableWidget->setRowCount(0);

    ServicesManager *servicesManager = ServicesManager::instance();
    DatabaseManager *databaseManager = servicesManager->databaseManager();

    QList<Movie> l_matchingMoviesList = servicesManager->matchingMoviesList();
    foreach (Movie l_movie, l_matchingMoviesList) {
        if(moviesList.contains(l_movie)) {
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

    Macaw::DEBUG("[MoviesPannel.startMovie()] Opened movie: " + l_movie.filePath());


    if (!databaseManager->getMediaPlayerPath().compare("")) {
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
                                       .arg(l_movie.filePath()));
    } else {
        QDesktopServices::openUrl(QUrl("file://" + l_movie.filePath(), QUrl::TolerantMode));
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

//------- ALL THIS MUST GO SOMEWHERE ELSES ----/

/**
 * @brief move the specified movie's file to trash bin.
 *
 * This function tries to move a file to trash,  as trash bin is not implemeted the same way amond OSes this could fail.
 * If the function cannot find the trash bin, it asks the user for the file permanant deletion.
 * @param movie to delete
 */
bool MoviesPannel::moveFileToTrash(QList<Movie> &movieList)
{
    DatabaseManager *databaseManager = ServicesManager::instance()->databaseManager();

    QMessageBox *l_confirmationDialog = new QMessageBox(QMessageBox::Warning, tr("Move to trash?"),
                                                         tr("Move to trash? All data in Macaw-Movies specific to the concerned movie(s) will be deleted, this cannot be undone."),
                                                         QMessageBox::Yes|QMessageBox::No, this);

    l_confirmationDialog->setDefaultButton(QMessageBox::No);

    if(l_confirmationDialog->exec() == QMessageBox::Yes) {
        foreach (Movie l_movie, movieList) {
            bool l_successfullyDeleted = false;
            QFile *movieFileToDelete = new QFile(l_movie.filePath());

            if(movieFileToDelete->exists()) {
#ifdef Q_OS_LINUX
                l_successfullyDeleted = linux_moveFileToTrash(l_movie.filePath());
#endif

#ifdef Q_OS_WIN
                l_successfullyDeleted = windows_moveFileToTrash(l_movie.filePath());
#endif

#ifdef Q_OS_OSX
                l_successfullyDeleted = macosx_moveFileToTrash(l_movie.filePath());
#endif
            } else {
                Macaw::DEBUG("[MoviesPannel] File doesn't exist");
                l_successfullyDeleted = true;
            }

            if(l_successfullyDeleted) {
                if(!databaseManager->deleteMovie(l_movie)) {
                    QMessageBox *msgBox = new QMessageBox(QMessageBox::Critical, tr("Error deleting"),
                                                    tr("Error deleting the movie from the database."),
                                                    QMessageBox::Ok, this);
                    msgBox->exec();
                    return false;
                }
            }
        }
        emit updatePannels();
    }
    return true;
}

/**
 * @brief searches and returns the right trash folder for GNU/Linux and OSX platforms
 *
 * This function chooses the right folder according to file location (local hard drive or external storage)
 * and the OS specificities. If an error occured an empty string is returned (and aDebug message is sent if debug is on).
 *
 * @param movieFilePath the path to the movie's file to be moved to trash
 *
 * @return QString the right trash folder or an empty string on errors
 *
 */
QString MoviesPannel::unix_findTrashFolder(QString movieFilePath) {

    if(movieFilePath.left(1) != "/" || movieFilePath.size() < 6)
    {
        Macaw::DEBUG("[MoviesPannel - unix_findTrashFolder] The path to the movie is either too short or not an absolute one");
        return "";
    }

    if(movieFilePath.left(4) == "/mnt" || movieFilePath.left(6) == "/media" || movieFilePath.left(8) == "/Volumes") {
#ifdef Q_OS_LINUX
        QProcess l_process;
        QString l_command("df --output=target \"" + QDir::toNativeSeparators(movieFilePath) + "\" | tail -n +2");
        l_process.start("bash", QStringList() << "-c" << l_command);

        if(!l_process.waitForStarted(500)) // waits a max of 500ms for the process to start
        {
            Macaw::DEBUG("[MoviesPannel - unix_findTrashFolder] The Linux process for folder detection did not start in less than 500ms. Aborting...");
            return "";
        }

        bool l_processFinishStatus = false;
        QByteArray l_buffer;

        while ((l_processFinishStatus = l_process.waitForFinished(500))); // waits for the process to end or a max of 500ms

        l_buffer.append(l_process.readAll());

        if (!l_processFinishStatus) {
            Macaw::DEBUG("[MoviesPannel - unix_findTrashFolder] The Linux process for folder detection did not end in less than 500ms. Aborting...");
            return "";
        }

        return QString(l_buffer);

#endif
#ifdef Q_OS_OSX
        QString l_trashbinDirectory = "/Volumes/";
        int l_volumeNameSize = movieFilePath.indexOf("/", 9);
        l_trashbinDirectory = movieFilePath.left(l_volumeNameSize+1).append(".Trashes/");
        return l_trashbinDirectory;
#endif
    }
    else {
#ifdef Q_OS_LINUX
        QString l_trashbinDirectory(QDir::home().path().append(QDir::separator())
                                                .append(".local/share/Trash"));

        if(!QDir(l_trashbinDirectory).exists()) {
            l_trashbinDirectory = QDir::home().path().append(QDir::separator()).append(".trash");
            if(!QDir(l_trashbinDirectory).exists()) {
                l_trashbinDirectory = QString(getenv("XDG_DATA_HOME")).append("/Trash");
                if(!QDir(l_trashbinDirectory).exists()) {
                    Macaw::DEBUG("[MoviesPannel - unix_findTrashFolder] Did not managed to find the Linux trsh folder of this computer");
                    return "";
                } else {
                    return l_trashbinDirectory;
                }
            } else {
                return l_trashbinDirectory;
            }
        } else {
            return l_trashbinDirectory;
        }
#endif
#ifdef Q_OS_OSX
        return QDir::home().path().append(QDir::separator()).append(".Trash/");
#endif
    }
}



/**
 * @brief Moves a movie file to trash bin for Linux.
 *
 * Moving a file to trash on GNU/Linux is made moving the file in a <trash folder>/files directory
 * and creating and info file in <trash folder>/info
 * The <trash folder> is found and returned by unix_findTrashFolder(movieFilePath)
 *
 * @param moviePath the path to the movie's file
 *
 * @return bool true if the file have been successfully moved to trash or permanently deleted
 *
 */
bool MoviesPannel::linux_moveFileToTrash(QString movieFilePath) {
    QDir l_dir;
    QFile l_movieFile(movieFilePath);
    QFileInfo l_movieFileInfo(movieFilePath);
    QString l_trashbinDirectory = unix_findTrashFolder(movieFilePath);    //root folder for trash
    QString l_trashFilesPath(l_trashbinDirectory + "/files/");    //folder to put the deleted file
    QString l_trashInfoPath(l_trashbinDirectory + "/info/");      //folder to put the info about deleted file

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

    Macaw::DEBUG("[MoviesPannel] Trash folder found here: " + l_trashbinDirectory);
    Macaw::DEBUG("[MoviesPannel] Trash name for the file: "+l_trashName);

    if(!l_movieFileInfo.exists() || !l_movieFileInfo.isFile()) {
        QMessageBox * l_msgBoxErrorFileNotExist = new QMessageBox(QMessageBox::Critical, "Error deleting",
                                            "File does not exists or is not a file, it cannot be deleted. ",
                                            QMessageBox::Ok, this);
        l_msgBoxErrorFileNotExist->exec();
        return false;
    }

    Macaw::DEBUG("[MoviesPannel] Writting info to: " + l_targetMovieFileInfo.absoluteFilePath());

    if(l_trashInfoFile.open(QIODevice::WriteOnly)) {
        Macaw::DEBUG("[MoviesPannel] Moving file FROM: "+movieFilePath+" TO: " + l_targetMovieFileFiles.absoluteFilePath());
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
            Macaw::DEBUG("[MoviesPannel] Failled to move file to trash");
            l_trashInfoFile.close();
            l_trashInfoFile.remove();
        }
    }
    else {
        Macaw::DEBUG("[MoviesPannel] Failled to create and open the file's' trash info");
    }

    QMessageBox *l_msgBoxErrorMovingToTrash = new QMessageBox(QMessageBox::Warning, tr("Error moving file to trash"),
                                        tr("Something went wrong when moving the file to the trash. Do you want to permanently delete it instead?"),
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
 * This function is highly inspired from one of the answers on Stackoverflow:
 * http://stackoverflow.com/questions/17964439/move-files-to-trash-recycle-bin-in-qt
 *
 * @param moviePath the path to the movie's file
 * @return bool true if the file have been successfully moved to trash
 *
 */
#ifdef Q_OS_WIN
bool MoviesPannel::windows_moveFileToTrash(QString movieFilePath) {
    QFileInfo fileinfo( movieFilePath );

    if( !fileinfo.exists() ) {
        QMessageBox * l_msgBoxErrorFileNotExist = new QMessageBox(QMessageBox::Critical, tr("Error deleting"),
                                            tr("File does not exists or is not a file, it cannot be deleted."),
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
        Macaw::DEBUG("[MoviesPannel] Moving file to trash failed with: " + QString::number(rv));
        QMessageBox * l_msgBoxErrorMovingToTrash = new QMessageBox(QMessageBox::Warning, tr("Error moving file to trash"),
                                            tr("Something went wrong when moving the file to the trash. Do you want to permanently delete it instead?"),
                                            QMessageBox::Yes|QMessageBox::No, this);

        if(l_msgBoxErrorMovingToTrash->exec() == QMessageBox::Yes)
        {
            QFile l_movieFile(movieFilePath);
            bool l_permanentlyDelete = permanentlyDeleteFile(&l_movieFile);
            return l_permanentlyDelete;
        }
    }
    else
    {
        return true;
    }

    return false; //this code must never be executed
}
#else
bool MoviesPannel::windows_moveFileToTrash(QString movieFilePath) {(void)movieFilePath;return false;}
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
bool MoviesPannel::macosx_moveFileToTrash(QString movieFilePath) {
    QDir l_dir;
    QFile l_movieFile(movieFilePath);
    QFileInfo l_movieFileInfo(movieFilePath);
    QString l_trashFilesPath = unix_findTrashFolder(movieFilePath);    //folder to put the deleted file

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

    Macaw::DEBUG("[MoviesPannel] Trash name for the file: "+l_trashName);

    if(!l_movieFileInfo.exists() || !l_movieFileInfo.isFile()) {
        QMessageBox * l_msgBoxErrorFileNotExist = new QMessageBox(QMessageBox::Critical, tr("Error deleting"),
                                            tr("File does not exists or is not a file, it cannot be deleted."),
                                            QMessageBox::Ok, this);
        l_msgBoxErrorFileNotExist->exec();
        return false;
    }

    Macaw::DEBUG("[MoviesPannel] Moving file FROM: "+movieFilePath+" TO: " + l_targetMovieFileFiles.absoluteFilePath());
    bool moveFile = l_dir.rename(movieFilePath, l_targetMovieFileFiles.absoluteFilePath());

    if(moveFile) {
        return true;
    }
    else {
        Macaw::DEBUG("[MoviesPannel] Failled to move file to trash");
    }

    QMessageBox * l_msgBoxErrorMovingToTrash = new QMessageBox(QMessageBox::Warning, tr("Error moving file to trash"),
                                        tr("Something went wrong when moving the file to the trash. Do you want to permanently delete it instead?"),
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
bool MoviesPannel::permanentlyDeleteFile(QFile * movieFileToDelete) {
        Macaw::DEBUG("[MoviesPannel] Permanently deleting file");
        if(!movieFileToDelete->remove())
        {
            QMessageBox * l_msgBoxErrorDeleting = new QMessageBox(QMessageBox::Critical, tr("Error deleting"),
                                                tr("Error deleting the file."),
                                                QMessageBox::Ok, this);
            l_msgBoxErrorDeleting->exec();
            return false;
        }
        return true;
}
