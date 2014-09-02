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

#include "SettingsWindow.h"

/**
 * @brief Constructor.
 */
SettingsWindow::SettingsWindow(QDialog *parent) : QDialog(parent)
{
    m_app = qobject_cast<Application *>(qApp);
    m_app->debug("[SettingsWindow] Constructor called");

    resize(400,300);
    setWindowTitle("Settings");
    m_mainLayout = new QVBoxLayout(this);
    m_filesPathLayout = new QHBoxLayout;
    m_filesPathMessage = new QLabel("");

    m_filesPathLabel = new QLabel("Files path");
    m_filesPathEdit = new QLineEdit;
    m_filesPathEdit->setText(m_app->getFilesPath());
    m_filesPathSearchButton = new QPushButton("Browse");
    QObject::connect(m_filesPathSearchButton, SIGNAL(clicked()), this, SLOT(browseFilesPathDialog()));

    m_knownPathList = new QListView;
    m_knownPathList->setModel(m_app->getDatabaseManager()->getMoviesPathModel());

    m_cancelButton = new QPushButton("Cancel");
    QObject::connect(m_cancelButton, SIGNAL(clicked()), this, SLOT(close()));

    m_submitButton = new QPushButton("Submit");
    QObject::connect(m_submitButton, SIGNAL(clicked()), this, SLOT(applySettings()));

    m_closeLayout = new QHBoxLayout;
    m_mainLayout->addLayout(m_filesPathLayout);
    m_mainLayout->addWidget(m_filesPathMessage);
    m_mainLayout->addWidget(m_knownPathList);
    m_mainLayout->addLayout(m_closeLayout);
    m_closeLayout->addWidget(m_cancelButton);
    m_closeLayout->addWidget(m_submitButton);
    m_filesPathLayout->addWidget(m_filesPathLabel);
    m_filesPathLayout->addWidget(m_filesPathEdit);
    m_filesPathLayout->addWidget(m_filesPathSearchButton);
    m_app->debug("[SettingsWindow] Construction done");
}

/**
 * @brief Destructor.
 */
SettingsWindow::~SettingsWindow()
{
    m_app->debug("[SettingsWindow] Destructed");
}

/**
 * @brief Save the settings.
 */
void SettingsWindow::applySettings()
{
    m_app->debug("[SettingsWindow] Enters applySettings()");
    if(QDir(m_filesPathEdit->text()).exists())
    {
        m_app->addFilesPath(m_filesPathEdit->text());
        emit closed();
        close();
    }
    else
    {
        m_filesPathEdit->setText("");
        m_filesPathMessage->setText("Choose an existant path");
    }
    m_app->debug("[SettingsWindow] Exits applySettings()");

}

/**
 * @brief Opens a QFileDialog to choose a path.
 */
void SettingsWindow::browseFilesPathDialog()
{
    m_app->debug("[SettingsWindow] Enters browseFilesPathDialog()");

    QString l_folder = QFileDialog::getExistingDirectory();
    m_filesPathEdit->setText(l_folder);
    m_app->debug("[SettingsWindow] Exits browseFilesPathDialog()");

}

