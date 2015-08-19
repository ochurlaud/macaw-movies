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

#ifndef MOVIESPANNEL_H
#define MOVIESPANNEL_H

#include "MainWindowWidgets/MainPannel.h"

class QFile;
class QTableWidgetItem;

class Movie;
class Playlist;

namespace Ui {
class MoviesPannel;
}

class MoviesPannel : public MainPannel
{
    Q_OBJECT

public:
    explicit MoviesPannel(QWidget *parent = 0);
    ~MoviesPannel();
    void fill(const QList<Movie> &movieList);

private slots:
    void on_customContextMenuRequested(const QPoint &point);
    void on_actionEdit_mainPannelMetadata_triggered();
    void on_actionDelete_triggered();
    void on_tableWidget_itemDoubleClicked(QTableWidgetItem *item);
    void on_tableWidget_itemSelectionChanged();
    void addPlaylistMenu_triggered(QAction* action);

private:
    Ui::MoviesPannel *m_ui;
    void setHeaders();
    void addMovieToPannel(const Movie &movie);
    void removeMovieFromPlaylist(const QList<Movie> &movieList, Playlist &playlist);
};

#endif // MOVIESPANNEL_H
