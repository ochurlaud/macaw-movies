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

#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>

class MoviePathsSettings;

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();

signals:
    void closeAndSave();

private slots:
    void on_buttonBox_accepted();
    void on_playerBrowseButton_clicked();
    void on_playerClearButton_clicked();

private:
    Ui::SettingsDialog *m_ui;
    MoviePathsSettings *m_moviePathsTab;
};

#endif // SETTINGSDIALOG_H
