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

#ifndef SETTINGWINDOW_H
#define SETTINGWINDOW_H

#include <QDialog>

#include "Application.h"

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QDialog
{
Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = 0);
    ~SettingsWindow();

public slots:
    void on_browseButton_clicked();
    void on_buttonBox_accepted();

signals:
    void closeAndSave();

private slots:
    void on_folderPathEdit_textChanged(const QString string);
    void on_addButton_clicked();
    void on_removeButton_clicked();

    void on_playerBrowseButton_clicked();

    void on_playerClearButton_clicked();

private:
    Ui::SettingsWindow *m_ui;
    void addToKnownPathList(QString path);

};

#endif // SETTINGSWINDOW_H
