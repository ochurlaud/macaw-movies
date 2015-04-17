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
#include "MovieDialog.h"
#include "PeopleDialog.h"
#include "SettingsWindow.h"

#include "Entities/Movie.h"
#include "Entities/Playlist.h"
#include "Entities/Tag.h"
#include "FetchMetadata/FetchMetadata.h"

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
    void on_leftPannel_itemSelectionChanged();
    void on_mainPannel_itemSelectionChanged();
    void on_actionEdit_leftPannelMetadata_triggered();
    void on_actionEdit_mainPannelMetadata_triggered();
    void on_actionDelete_triggered();
    void selfUpdate();
    void addNewMovies();
    void on_searchEdit_returnPressed();
    void addPlaylistMenu_triggered(QAction* action);
    void on_actionAbout_triggered();
    void closeEvent(QCloseEvent *event);

signals:
    void startFetchingMetadata(const QList<Movie>&);

private:
    Ui::MainWindow *m_ui;

    /**
     * @brief QList of movies that can be listed in the main window
     * (because they match the searching field)
     */
    QList<Movie> m_authorizedMoviesList;

    /**
     * @brief QList of movies that are actually listed in the main window
     */
    QList<Movie> m_displayedMoviesList;

    /**
     * @brief QList of ids of the leftPannel
     */
    QList<int> m_leftElementsIdList;

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
    void fillLeftPannel();
    void setLeftPannelLabel();
    QList<Movie> moviesToDisplay(int id);
    void updatePannels();
    void fillMetadataPannel(Movie movie);
    void removeMovieFromPlaylist(const QList<Movie> &movieList, Playlist &playlist);
    bool permanentlyDeleteFile(QFile *movieFileToDelete);
    bool moveFileToTrash(QList<Movie> &movieList);
    QString unix_findTrashFolder(QString movieFilePath);
    bool linux_moveFileToTrash(QString movieFilePath);
    bool windows_moveFileToTrash(QString movieFilePath);
    bool macosx_moveFileToTrash(QString movieFilePath);

};

#endif // MainWindow_H
