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
#include "MainWindowWidgets/LeftPannel.h"
#include "MainWindowWidgets/MainPannel.h"
#include "MainWindowWidgets/MoviesPannel.h"

class LeftPannel;
class MainPannel;
class MoviesPannel;

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
    void on_toWatchButton_clicked();  
    void selfUpdate();
    void updateMainPannel();
    void addNewMovies();
    void on_searchEdit_returnPressed();
    void on_actionAbout_triggered();
    void closeEvent(QCloseEvent *event);

signals:
    void startFetchingMetadata(const QList<Movie>&);

private:
    Ui::MainWindow *m_ui;
    LeftPannel *m_leftPannel;
    MainPannel *m_mainPannel;

    /**
     * @brief QList of movies that can be listed in the main window
     * (because they match the searching field)
     */
    QList<Movie> m_authorizedMoviesList;

    /**
     * @brief QList of movies that are actually listed in the main window
     */
    QList<Movie> m_displayedMoviesList;

    void readSettings();
    QList<Movie> moviesToDisplay(int id);
    void updatePannels();
    void fillMetadataPannel(Movie movie);

};

#endif // MainWindow_H
