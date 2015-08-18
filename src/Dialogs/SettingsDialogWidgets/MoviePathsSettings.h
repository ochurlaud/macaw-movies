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

#ifndef MOVIEPATHSSETTINGS_H
#define MOVIEPATHSSETTINGS_H

#include <QWidget>

#include "ServicesManager.h"
#include "Dialogs/SettingsDialogWidgets/CenteredCheckbox.h"
#include "Entities/PathForMovies.h"

namespace Ui {
class MoviePathsSettings;
}

class MoviePathsSettings : public QWidget
{
    Q_OBJECT

public:
    explicit MoviePathsSettings(QWidget *parent = 0);
    ~MoviePathsSettings();
    void saveSettings();

private slots:
    void on_browseButton_clicked();
    void on_folderPathEdit_textChanged(const QString string);
    void on_addButton_clicked();
    void on_removeButton_clicked();
    void updatedCheckboxes(int x, int y);

private:
    Ui::MoviePathsSettings *m_ui;
    void addToKnownPathCollection(PathForMovies moviePath);
    void addPathFromEdit();
    bool existsInPathCollection(QString path);
};

#endif // MOVIEPATHSSETTINGS_H
