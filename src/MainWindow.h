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

#ifndef MainWindow_H
#define MainWindow_H

#include <QtGui>
#include <QtWidgets>
#include "Application.h"
#include "Entities/Movie.h"
#include "Entities/Tag.h"
#include "SettingsWindow.h"
#include "MovieDialog.h"
#include "PeopleDialog.h"
#include "FetchMetadata/FetchMetadata.h"


class Application;

namespace Ui {
class MainWindow;
}

/**
 * @brief Implementation of the main window
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionEdit_Settings_triggered();
    void on_peopleBox_activated(int type);
    void on_toWatchButton_clicked();
    void on_tagsButton_clicked();
    void on_customContextMenuRequested(const QPoint &);
    void on_mainPannel_itemDoubleClicked(QTableWidgetItem *item);
    void on_leftPannel_clicked(const QModelIndex &index);
    void on_mainPannel_clicked(const QModelIndex &index);
    void on_actionEdit_leftPannelMetadata_triggered();
    void on_actionEdit_mainPannelMetadata_triggered();
    void actionRemoveInToWatch_triggered();
    void selfUpdate();
    void addNewMovies();
    void on_searchEdit_returnPressed();
    void addPlaylistMenu_triggered(QAction* action);
    void askForOrphanTagDeletion(Tag &orphanTag);
    void askForOrphanPeopleDeletion(People &orphanPeople);
    void on_actionAbout_triggered();
    void closeEvent(QCloseEvent *event);

signals:
    void toUpdate();

private:
    Ui::MainWindow *m_ui;
    Application *m_app;

    /**
     * @brief QList of movies that can be listed in the main window
     * (before verification if they match the search field)
     * This simplifies fillMetadataPannel() (no need to query the database)
     */
    QList<Movie> m_moviesList;

    /**
     * @brief type of elements lastly shown in the leftPannel (Tags/People)
     */
    int m_typeElement;

    /**
     * @brief type of People (if People) lastly shown in the leftPannel
     */
    int m_typePeople;

    /**
     * @brief id of the selected element in leftPannel
     */
    int m_leftPannelSelectedId;
    void readSettings();
    void fillMainPannel();
    void fillLeftPannel(int typeElement, int typePeople);
    void setLeftPannelLabel();
    void prepareMoviesToDisplay(int id);
    void filterPannels();
    void fillMetadataPannel(Movie movie);

};

#endif // MainWindow_H
