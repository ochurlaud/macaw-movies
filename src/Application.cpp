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

#include "Application.h"

/**
 * @brief Constructor
 */
Application::Application(int &argc, char **argv) :
    QApplication(argc, argv)
{
    Macaw::DEBUG_IN("[Application] started");

    this->definePaths();
    DatabaseManager *databaseManager = DatabaseManager::instance();

    this->setApplicationName(APP_NAME);
    this->setApplicationVersion(APP_VERSION);
    this->setWindowIcon(QIcon(":/img/logov0_1.png"));
    m_tmdbkey = "6e4cbac7861ad5b847ef8f60489dc04e";
    m_mainWindow = new MainWindow;
    m_fetchMetadata = NULL;

    connect(databaseManager, SIGNAL(orphanTagDetected(Tag&)),
            this, SLOT(askForOrphanTagDeletion(Tag&)));
    connect(databaseManager, SIGNAL(orphanPeopleDetected(People&)),
            this, SLOT(askForOrphanPeopleDeletion(People&)));
    connect(m_mainWindow, SIGNAL(startFetchingMetadata(const QList<Movie>&)),
            this, SLOT(on_startFetchingMetadata(const QList<Movie>&)));
    connect(this, SIGNAL(updatedMainWindow()),
            m_mainWindow, SLOT(selfUpdate()));

    qApp->property("filesPath");
    m_mainWindow->show();

    Macaw::DEBUG_OUT("[Application] Application initialized");
}

/**
 * @brief Destructor
 */
Application::~Application()
{
    Macaw::DEBUG("[Application] Destructed");
}

/**
 * @brief Slot triggered when DatabaseManager finds an orphan tag.
 * A QMessageBox asks the user if the tag should be delete or not.
 *
 * @param orphanTag concerned by the choice
 */
void Application::askForOrphanTagDeletion(Tag &orphanTag)
{
    DatabaseManager *databaseManager = DatabaseManager::instance();

    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setText("The tag <b>"+ orphanTag.name() +"</b> is not used in any movie now. ");
    msgBox.setInformativeText("Do you want to delete this tag?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);

    if(msgBox.exec() == QMessageBox::Yes) {
        databaseManager->deleteTag(orphanTag);
    }
}

/**
 * @brief Slot triggered when DatabaseManager finds an orphan person.
 * A QMessageBox asks the user if the person should be delete or not.
 *
 * @param orphanPeople concerned by the choice
 */
void Application::askForOrphanPeopleDeletion(People &orphanPeople)
{
    DatabaseManager *databaseManager = DatabaseManager::instance();

    QString l_name = orphanPeople.name();

    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setText("The person <b>" +l_name+ "</b> is not linked to any movie now.");
    msgBox.setInformativeText("Do you want to delete it?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);

    if(msgBox.exec() == QMessageBox::Yes) {
        databaseManager->deletePeople(orphanPeople);
    }
}

/**
 * @brief Slot triggered when the user wants to fetch metadata on the internet
 */
void Application::on_startFetchingMetadata(const QList<Movie> &movieList)
{
    Macaw::DEBUG("[Application] startFetchingMetadata called");
    if(m_fetchMetadata == NULL) {
        m_fetchMetadata = new FetchMetadata(movieList);
    }

    connect(this, SIGNAL(fetchMetadata()),
            m_fetchMetadata, SLOT(startProcess()));
    connect(m_fetchMetadata, SIGNAL(jobDone()),
            this, SLOT(on_fethMetadataJobDone()));
    emit fetchMetadata();
}

/**
 * @brief Slot triggered when m_fetchMetadata has finished its job
 * Delete the pointer concerned.
 */
void Application::on_fethMetadataJobDone()
{
    delete m_fetchMetadata;
    emit updatedMainWindow();
}

/**
 * @brief Define the paths used in the app
 * Can be retrieved by `qApp->property("name").toString`
 */
void Application::definePaths()
{
    QString l_filesPath = "";

#ifdef Q_OS_LINUX
    // Files in ~/.local/share/macaw-movies and create the folder if not exists
    l_filesPath = QString(QDir::home().path().append(QDir::separator())
                                             .append(".local")
                                             .append(QDir::separator())
                                             .append("share"));
#endif

#ifdef Q_OS_WIN
    // File in $USER\AppData\Local
    l_filesPath = QString(QDir::home().path().append(QDir::separator())
                                            .append("AppData")
                                            .append(QDir::separator())
                                            .append("Local"));
#endif

#ifdef Q_OS_OSX
    // Files in ~/Library/Application Support
    l_filesPath = QString(QDir::home().path().append(QDir::separator())
                                            .append("Library")
                                            .append(QDir::separator())
                                            .append("Application Support"));
#endif

    l_filesPath.append(QDir::separator());
    l_filesPath = QDir::toNativeSeparators(l_filesPath);

    QFileInfo checkFolder(l_filesPath + APP_NAME_SMALL);
    if (!checkFolder.exists())
    {
        QDir(l_filesPath).mkdir(APP_NAME_SMALL);
    }

    l_filesPath.append(APP_NAME_SMALL).append(QDir::separator());

    l_filesPath = QDir::toNativeSeparators(l_filesPath);
    checkFolder = QString(l_filesPath + "posters");
    if (!checkFolder.exists())
    {
        QDir(l_filesPath).mkdir("posters");
    }

    QString l_postersPath = QDir::toNativeSeparators(l_filesPath
                                                     + "posters"
                                                     +  QDir::separator()
                                                     );
    this->setProperty("filesPath", l_filesPath);
    this->setProperty("postersPath", l_postersPath);
}
