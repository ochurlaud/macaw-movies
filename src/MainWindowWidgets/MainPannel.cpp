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

#include "MainPannel.h"

#include <QDir>
#include <QMessageBox>
#include <QProcess>

#include "MacawDebug.h"
#include "ServicesManager.h"

MainPannel::MainPannel(QWidget *parent) :
    QWidget(parent)
{

}

/**
 * @brief move the specified movie's file to trash bin.
 *
 * This function tries to move a file to trash,  as trash bin is not implemeted the same way amond OSes this could fail.
 * If the function cannot find the trash bin, it asks the user for the file permanant deletion.
 * @param movie to delete
 */
bool MainPannel::moveFileToTrash(QList<Movie> &movieList)
{
    DatabaseManager *databaseManager = ServicesManager::instance()->databaseManager();

    QMessageBox *l_confirmationDialog = new QMessageBox(QMessageBox::Warning, tr("Move to trash?"),
                                                         tr("Move to trash? All data in Macaw-Movies specific to the concerned movie(s) will be deleted, this cannot be undone."),
                                                         QMessageBox::Yes|QMessageBox::No, this);

    l_confirmationDialog->setDefaultButton(QMessageBox::No);

    if(l_confirmationDialog->exec() == QMessageBox::Yes) {
        foreach (Movie l_movie, movieList) {
            bool l_successfullyDeleted = false;
            QFile *movieFileToDelete = new QFile(l_movie.fileAbsolutePath());

            if(movieFileToDelete->exists()) {
#ifdef Q_OS_LINUX
                l_successfullyDeleted = linux_moveFileToTrash(l_movie.fileAbsolutePath());
#endif

#ifdef Q_OS_WIN
                l_successfullyDeleted = windows_moveFileToTrash(l_movie.fileAbsolutePath());
#endif

#ifdef Q_OS_OSX
                l_successfullyDeleted = macosx_moveFileToTrash(l_movie.fileAbsolutePath());
#endif
            } else {
                Macaw::DEBUG("[MainPannel] File doesn't exist");
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
 * and the OS specificities. If an error occurred an empty string is returned (and aDebug message is sent if debug is on).
 *
 * @param movieFilePath the path to the movie's file to be moved to trash
 *
 * @return QString the right trash folder or an empty string on errors
 *
 */
QString MainPannel::unix_findTrashFolder(QString movieFilePath) {

    if(movieFilePath.left(1) != "/" || movieFilePath.size() < 6)
    {
        Macaw::DEBUG("[MainPannel - unix_findTrashFolder] The path to the movie is either too short or not an absolute one");
        return "";
    }

    if(movieFilePath.left(4) == "/mnt" || movieFilePath.left(6) == "/media" || movieFilePath.left(8) == "/Volumes") {
#ifdef Q_OS_LINUX
        QProcess l_process;
        QString l_command("df --output=target \"" + QDir::toNativeSeparators(movieFilePath) + "\" | tail -n +2");
        l_process.start("bash", QStringList() << "-c" << l_command);

        if(!l_process.waitForStarted(500)) // waits a max of 500ms for the process to start
        {
            Macaw::DEBUG("[MainPannel - unix_findTrashFolder] The Linux process for folder detection did not start in less than 500ms. Aborting...");
            return "";
        }

        bool l_processFinishStatus = false;
        QByteArray l_buffer;

        while ((l_processFinishStatus = l_process.waitForFinished(500))); // waits for the process to end or a max of 500ms

        l_buffer.append(l_process.readAll());

        if (!l_processFinishStatus) {
            Macaw::DEBUG("[MainPannel - unix_findTrashFolder] The Linux process for folder detection did not end in less than 500ms. Aborting...");
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
                    Macaw::DEBUG("[MainPannel - unix_findTrashFolder] Did not managed to find the Linux trsh folder of this computer");
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
bool MainPannel::linux_moveFileToTrash(QString movieFilePath) {
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
                + '(' + QString::number(l_count) + ')' +
                (l_movieFileInfo.suffix().isEmpty() ? "" : '.' + l_movieFileInfo.suffix());

        l_targetMovieFileInfo = QFileInfo(l_trashInfoPath + l_trashName + ".trashinfo");
        l_targetMovieFileFiles = QFileInfo(l_trashFilesPath + l_trashName);
    }

    QFile l_trashInfoFile(l_targetMovieFileInfo.absoluteFilePath());  //file containing the info about deleted file

    Macaw::DEBUG("[MainPannel] Trash folder found here: " + l_trashbinDirectory);
    Macaw::DEBUG("[MainPannel] Trash name for the file: "+l_trashName);

    if(!l_movieFileInfo.exists() || !l_movieFileInfo.isFile()) {
        QMessageBox * l_msgBoxErrorFileNotExist = new QMessageBox(QMessageBox::Critical, "Error deleting",
                                            "File does not exists or is not a file, it cannot be deleted. ",
                                            QMessageBox::Ok, this);
        l_msgBoxErrorFileNotExist->exec();
        return false;
    }

    Macaw::DEBUG("[MainPannel] Writing info to: " + l_targetMovieFileInfo.absoluteFilePath());

    if(l_trashInfoFile.open(QIODevice::WriteOnly)) {
        Macaw::DEBUG("[MainPannel] Moving file FROM: "+movieFilePath+" TO: " + l_targetMovieFileFiles.absoluteFilePath());
        bool moveFile = l_dir.rename(movieFilePath, l_targetMovieFileFiles.absoluteFilePath());

        if(moveFile) {
            l_trashInfoFile.write("[Trash Info]\n");
            QString l_path = "Path=" + movieFilePath + '\n';
            QString l_deletionDate = "DeletionDate=" + QDateTime::currentDateTime().toString(Qt::ISODate) + '\n';

            l_trashInfoFile.write((char *)(l_path.toStdString().c_str()));
            l_trashInfoFile.write((char *)(l_deletionDate.toStdString().c_str()));
            l_trashInfoFile.close();

            return true;
        }
        else {
            Macaw::DEBUG("[MainPannel] Failled to move file to trash");
            l_trashInfoFile.close();
            l_trashInfoFile.remove();
        }
    }
    else {
        Macaw::DEBUG("[MainPannel] Failled to create and open the file's' trash info");
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
bool MainPannel::windows_moveFileToTrash(QString movieFilePath) {
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
        Macaw::DEBUG("[MainPannel] Moving file to trash failed with: " + QString::number(rv));
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
bool MainPannel::windows_moveFileToTrash(QString movieFilePath) {(void)movieFilePath;return false;}
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
bool MainPannel::macosx_moveFileToTrash(QString movieFilePath) {
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
                + '(' + QString::number(l_count) + ')' +
                (l_movieFileInfo.suffix().isEmpty() ? "" : '.' + l_movieFileInfo.suffix());

        l_targetMovieFileFiles = QFileInfo(l_trashFilesPath + l_trashName);
    }

    Macaw::DEBUG("[MainPannel] Trash name for the file: "+l_trashName);

    if(!l_movieFileInfo.exists() || !l_movieFileInfo.isFile()) {
        QMessageBox * l_msgBoxErrorFileNotExist = new QMessageBox(QMessageBox::Critical, tr("Error deleting"),
                                            tr("File does not exists or is not a file, it cannot be deleted."),
                                            QMessageBox::Ok, this);
        l_msgBoxErrorFileNotExist->exec();
        return false;
    }

    Macaw::DEBUG("[MainPannel] Moving file FROM: "+movieFilePath+" TO: " + l_targetMovieFileFiles.absoluteFilePath());
    bool moveFile = l_dir.rename(movieFilePath, l_targetMovieFileFiles.absoluteFilePath());

    if(moveFile) {
        return true;
    }
    else {
        Macaw::DEBUG("[MainPannel] Failled to move file to trash");
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
bool MainPannel::permanentlyDeleteFile(QFile * movieFileToDelete) {
        Macaw::DEBUG("[MainPannel] Permanently deleting file");
        if(!movieFileToDelete->remove())
        {
            QMessageBox *l_msgBoxErrorDeleting = new QMessageBox(QMessageBox::Critical, tr("Error deleting"),
                                                tr("Error deleting the file."),
                                                QMessageBox::Ok, this);
            l_msgBoxErrorDeleting->exec();
            return false;
        }
        return true;
}
