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
    m_app->debug("[SettingsWindow] Constructor called");

    m_ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);

    QStringList l_moviePathsList = m_app->getDatabaseManager()->getMoviePaths();
    m_ui->knownPathsList->addItems(l_moviePathsList);
}

SettingsWindow::~SettingsWindow()
{
    delete m_ui;
}

void SettingsWindow::on_browseButton_clicked()
{
    m_app->debug("[SettingsWindow] Enters browseFilesPathDialog()");
    QString l_folder = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                          "/home/",
                                                          QFileDialog::ShowDirsOnly);
    m_ui->folderPathEdit->setText(l_folder);
    m_app->debug("[SettingsWindow] Exits browseFilesPathDialog()");
}


void SettingsWindow::on_buttonBox_accepted()
{
    QString l_newPath = m_ui->folderPathEdit->text();
    addToKnownPathsList(l_newPath);

    for (int i=0 ; i < m_ui->knownPathsList->count() ; i++) {
        QString l_path = m_ui->knownPathsList->item(i)->text();
        m_app->getDatabaseManager()->addMoviePath(l_path);
        if (m_ui->recheckBox->isChecked()) {
            m_app->getDatabaseManager()->setMoviePathImported(l_path, false);
        }
    }
    emit closeAndSave();
    close();
    m_app->debug("[SettingsWindow] Exits on_buttonBox_accepted()");
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
