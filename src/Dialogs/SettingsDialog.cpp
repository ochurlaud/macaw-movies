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

    QList<PathForMovies> l_moviesPathList = databaseManager->getMoviesPaths(false);
    l_moviesPathList << databaseManager->getMoviesPaths(true);


    m_ui->knownPathCollection->setRowCount(l_moviesPathList.count());
    m_ui->knownPathCollection->setColumnCount(3);

    for (int i = 0 ; i < l_moviesPathList.count() ; i++) {
        PathForMovies l_moviesPath = l_moviesPathList.at(i);

        QCheckBox *l_moviesCheckbox = new QCheckBox();
        l_moviesCheckbox->setChecked(l_moviesPath.hasMovies());
        QCheckBox *l_showsCheckbox = new QCheckBox();
        l_showsCheckbox->setChecked(l_moviesPath.hasShows());

        m_ui->knownPathCollection->setItem(i, 0, new QTableWidgetItem(l_moviesPath.path()));
        m_ui->knownPathCollection->setCellWidget(i, 1, l_moviesCheckbox);
        m_ui->knownPathCollection->setCellWidget(i, 2, l_showsCheckbox);
    }

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

void SettingsDialog::on_browseButton_clicked()
{
    Macaw::DEBUG_IN("[SettingsDialog] Enters browseFilesPathDialog()");
    QString l_folder = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                          "/home/",
                                                          QFileDialog::ShowDirsOnly);
    m_ui->folderPathEdit->setText(l_folder);
    Macaw::DEBUG_OUT("[SettingsDialog] Exits browseFilesPathDialog()");
}

void SettingsDialog::on_buttonBox_accepted()
{
    Macaw::DEBUG_IN("[SettingsDialog] Enters on_buttonBox_accepted()");

    DatabaseManager *databaseManager = ServicesManager::instance()->databaseManager();

    QString l_newPath = m_ui->folderPathEdit->text();
    addToknownPathCollection(l_newPath);

    QList<PathForMovies> l_moviesPathList = databaseManager->getMoviesPaths(true);
    l_moviesPathList << databaseManager->getMoviesPaths(false);
    foreach (PathForMovies l_moviesPath, l_moviesPathList) {
        if (m_ui->knownPathCollection->findItems(l_moviesPath.path(), Qt::MatchExactly).isEmpty()) {
            Macaw::DEBUG("[MainWindow] Remove path "+l_moviesPath.path());
            // We remove the path and all the movies behind
            databaseManager->deleteMoviesPath(l_moviesPath);
        }
    }

    for (int i=0 ; i < m_ui->knownPathCollection->rowCount() ; i++) {
        QString l_path = m_ui->knownPathCollection->item(i,0)->text();
        databaseManager->addMoviesPath(l_path);
        if (m_ui->recheckBox->isChecked()) {
            databaseManager->setMoviesPathImported(l_path, false);
        }
    }

    //if (m_ui->playerPathEdit->text() != "") {
        if (m_ui->playerPathEdit->text().compare(databaseManager->getMediaPlayerPath())) {
            databaseManager->addMediaPlayerPath(m_ui->playerPathEdit->text());
        }
    //}

    emit closeAndSave();
    close();
    Macaw::DEBUG_OUT("[SettingsDialog] Exits on_buttonBox_accepted()");
}

void SettingsDialog::on_folderPathEdit_textChanged(const QString string)
{
    if (!string.isEmpty()) {
        m_ui->addButton->setEnabled(true);
    } else {
        m_ui->addButton->setDisabled(true);
    }
}

void SettingsDialog::on_addButton_clicked()
{
    QString l_newPath = m_ui->folderPathEdit->text();
    addToknownPathCollection(l_newPath);
}

void SettingsDialog::on_removeButton_clicked()
{
    QList<QTableWidgetItem*> l_selectedItemList = m_ui->knownPathCollection->selectedItems();
    foreach (QTableWidgetItem *l_selectedItem, l_selectedItemList) {
        m_ui->knownPathCollection->removeRow(l_selectedItem->row());
    }
}

void SettingsDialog::addToknownPathCollection(QString path)
{
    if (!QDir(path).exists() || path.isEmpty()) {
        m_ui->folderPathMessage->setText(tr("Choose an existant path"));
    } else {
        bool l_pathExits = false;
        for (int i = 0 ; i < m_ui->knownPathCollection->rowCount() ; i++) {
            if (m_ui->knownPathCollection->itemAt(1,0)->text() == path) {
                m_ui->folderPathMessage->setText(tr("This path is already known"));
                l_pathExits = true;
                break;
            }
        }
        if (!l_pathExits) {
                int l_rowCount = m_ui->knownPathCollection->rowCount();
                m_ui->knownPathCollection->setRowCount(l_rowCount+1);
                m_ui->knownPathCollection->setItem(l_rowCount, 0, new QTableWidgetItem(path));
                m_ui->knownPathCollection->setCellWidget(l_rowCount, 1, new QCheckBox);
                m_ui->knownPathCollection->setCellWidget(l_rowCount, 2, new QCheckBox);
        }
    }
    m_ui->folderPathEdit->clear();
}

void SettingsDialog::on_playerBrowseButton_clicked()
{
    Macaw::DEBUG_IN("[SettingsDialog] Enters browseFilesPathDialog()");
    QString l_mediaPlayer = QFileDialog::getOpenFileName(this,
                                                         tr("Select File"),
                                                         "/home/",
                                                         tr("Program Files (*.app *.exe *.bat)"));
    m_ui->playerPathEdit->setText(l_mediaPlayer);
    Macaw::DEBUG_OUT("[SettingsDialog] Exits browseFilesPathDialog()");
}

void SettingsDialog::on_playerClearButton_clicked()
{
    Macaw::DEBUG_IN("[SettingsDialog] on_playerClearButton_clicked()");
    m_ui->playerPathEdit->clear();
}
