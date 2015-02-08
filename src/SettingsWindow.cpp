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
#include "ui_SettingsWindow.h"

SettingsWindow::SettingsWindow(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::SettingsWindow)
{
    m_app = qobject_cast<Application *>(qApp);
    m_app->debug("[SettingsWindow] Constructor called");

    m_ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    m_ui->knownPathList->setModel(m_app->getDatabaseManager()->getMoviesPathModel());
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
    m_app->debug("[SettingsWindow] Enters on_buttonBox_accepted()");
    if(QDir(m_ui->folderPathEdit->text()).exists())
    {
        m_app->addFilesPath(m_ui->folderPathEdit->text());
        emit closeAndSave();
        close();
    }
    else
    {
        m_ui->folderPathEdit->setText("");
        m_ui->folderPathMessage->setText("Choose an existant path");
    }
    m_app->debug("[SettingsWindow] Exits on_buttonBox_accepted()");
}
