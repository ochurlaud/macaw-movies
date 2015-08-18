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

#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"

/**
 * @brief Constructor
 * @param parent
 */
SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::SettingsDialog)
{
    Macaw::DEBUG_IN("[SettingsDialog] Constructor called");
    DatabaseManager *databaseManager = ServicesManager::instance()->databaseManager();

    m_ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);

    m_moviePathsTab = new MoviePathsSettings();
    m_ui->gridLayout->addWidget(m_moviePathsTab);

    QString l_mediaPlayerPath(databaseManager->getMediaPlayerPath());
    m_ui->playerPathEdit->setText(l_mediaPlayerPath);

    Macaw::DEBUG_OUT("[SettingsDialog] Construction done");
}

/**
 * @brief Destructor
 */
SettingsDialog::~SettingsDialog()
{
    delete m_ui;
}

/**
 * @brief Slot triggered when the validation button is clicked.
 * Add/Remove the changed paths, update player path in the database. A signal is emitted before closing.
 */
void SettingsDialog::on_buttonBox_accepted()
{
    Macaw::DEBUG_IN("[SettingsDialog] Enters on_buttonBox_accepted()");

    DatabaseManager *databaseManager = ServicesManager::instance()->databaseManager();

    m_moviePathsTab->saveSettings();

    if (m_ui->playerPathEdit->text().compare(databaseManager->getMediaPlayerPath())) {
        databaseManager->addMediaPlayerPath(m_ui->playerPathEdit->text());
    }

    emit closeAndSave();
    close();
    Macaw::DEBUG_OUT("[SettingsDialog] Exits on_buttonBox_accepted()");
}

/**
 * @brief SettingsDialog::on_playerBrowseButton_clicked
 */
void SettingsDialog::on_playerBrowseButton_clicked()
{
    Macaw::DEBUG_IN("[SettingsDialog] Enters browseFilesPathDialog()");
    QString l_mediaPlayer = QFileDialog::getOpenFileName(this,
                                                         tr("Select File"),
                                                         QDir::homePath(),
                                                         tr("Program Files (*.app *.exe *.bat *)"));
    m_ui->playerPathEdit->setText(l_mediaPlayer);
    Macaw::DEBUG_OUT("[SettingsDialog] Exits browseFilesPathDialog()");
}

/**
 * @brief SettingsDialog::on_playerClearButton_clicked
 */
void SettingsDialog::on_playerClearButton_clicked()
{
    Macaw::DEBUG_IN("[SettingsDialog] on_playerClearButton_clicked()");
    m_ui->playerPathEdit->clear();
}
