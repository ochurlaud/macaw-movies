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

#include "SettingsWindow.h"
#include "ui_SettingsWindow.h"

SettingsWindow::SettingsWindow(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::SettingsWindow)
{
    m_app = qobject_cast<Application *>(qApp);
    Macaw::DEBUG("[SettingsWindow] Constructor called");

    m_ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);

    QStringList l_moviesPathsList = m_app->getDatabaseManager()->getMoviesPaths(0);
    l_moviesPathsList << m_app->getDatabaseManager()->getMoviesPaths(1);
    m_ui->knownPathsList->addItems(l_moviesPathsList);
}

SettingsWindow::~SettingsWindow()
{
    delete m_ui;
}

void SettingsWindow::on_browseButton_clicked()
{
    Macaw::DEBUG("[SettingsWindow] Enters browseFilesPathDialog()");
    QString l_folder = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                          "/home/",
                                                          QFileDialog::ShowDirsOnly);
    m_ui->folderPathEdit->setText(l_folder);
    Macaw::DEBUG("[SettingsWindow] Exits browseFilesPathDialog()");
}


void SettingsWindow::on_buttonBox_accepted()
{
    QString l_newPath = m_ui->folderPathEdit->text();
    addToKnownPathsList(l_newPath);

    QStringList l_moviesPathsList = m_app->getDatabaseManager()->getMoviesPaths(true);
    l_moviesPathsList << m_app->getDatabaseManager()->getMoviesPaths(false);
    qDebug() << l_moviesPathsList.count();
    foreach (QString l_moviesPath, l_moviesPathsList) {
        if (m_ui->knownPathsList->findItems(l_moviesPath, Qt::MatchExactly).count() == 0) {
            Macaw::DEBUG("[MainWindow] Remove path "+l_moviesPath);
            // We remove the path and all the movies behind
            m_app->getDatabaseManager()->deleteMoviesPath(l_moviesPath);
        }
    }

    for (int i=0 ; i < m_ui->knownPathsList->count() ; i++) {
        QString l_path = m_ui->knownPathsList->item(i)->text();
        m_app->getDatabaseManager()->addMoviesPath(l_path);
        if (m_ui->recheckBox->isChecked()) {
            m_app->getDatabaseManager()->setMoviesPathImported(l_path, false);
        }
    }
    emit closeAndSave();
    close();
    Macaw::DEBUG("[SettingsWindow] Exits on_buttonBox_accepted()");
}

void SettingsWindow::on_folderPathEdit_textChanged(const QString string)
{
    if (!string.isEmpty()) {
        m_ui->addButton->setEnabled(true);
    } else {
        m_ui->addButton->setDisabled(true);
    }
}

void SettingsWindow::on_addButton_clicked()
{
    QString l_newPath = m_ui->folderPathEdit->text();
    addToKnownPathsList(l_newPath);
}

void SettingsWindow::on_removeButton_clicked()
{
    QList<QListWidgetItem*> l_selectedItemsList = m_ui->knownPathsList->selectedItems();
    foreach (QListWidgetItem *l_selectedItem, l_selectedItemsList) {
        delete l_selectedItem;
    }
}

void SettingsWindow::addToKnownPathsList(QString path)
{
    if (!QDir(path).exists() || path.isEmpty()) {
        m_ui->folderPathMessage->setText("Choose an existant path");
    } else if (m_ui->knownPathsList->findItems(path,Qt::MatchExactly).count()) {
        m_ui->folderPathMessage->setText("This path is already known");
    } else {
        m_ui->knownPathsList->addItem(path);
    }
    m_ui->folderPathEdit->clear();
}
