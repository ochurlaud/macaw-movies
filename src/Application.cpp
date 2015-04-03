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

Q_GLOBAL_STATIC(DatabaseManager, databaseManager)

/**
 * @brief Constructor
 */
Application::Application(int &argc, char **argv) :
    QApplication(argc, argv)
{
    Macaw::DEBUG("[Application] started");

    this->setApplicationName(APP_NAME);
    this->setApplicationVersion(APP_VERSION);
    this->setWindowIcon(QIcon(":/img/logov0_1.png"));
    m_tmdbkey = "6e4cbac7861ad5b847ef8f60489dc04e";

    connect(databaseManager,SIGNAL(orphanTagDetected(Tag&)),
            this, SLOT(askForOrphanTagDeletion(Tag&)));
    connect(databaseManager,SIGNAL(orphanPeopleDetected(People&)),
            this, SLOT(askForOrphanPeopleDeletion(People&)));

    m_mainWindow = new MainWindow;
    m_mainWindow->show();
    Macaw::DEBUG("[Application] Database initialized");
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
