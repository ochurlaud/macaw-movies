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

#ifndef FETCHMETADATADIALOG_H
#define FETCHMETADATADIALOG_H

#include <QDialog>

#include "Entities/Movie.h"

template<class T> class QList;
class QModelIndex;
class QString;

class Movie;

namespace Ui {
class FetchMetadataDialog;
}

/**
 * @brief The FetchMetadataDialog class
 *
 * @author Olivier CHURLAUD <olivier@churlaud.com>
 */
class FetchMetadataDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FetchMetadataDialog(Movie movie, QList<Movie> moviesPropositionList, QWidget *parent = 0);
    ~FetchMetadataDialog();
    void setMovieList(const QList<Movie> &movieList);

signals:
    void selectedMovie(const Movie &movie);
    void searchMovies(const QString newTitle);
    void searchCanceled();
    void dontAskUser();
    void neverAskUser(Movie &movie);

private slots:
    void on_buttonBox_accepted();
    void on_listWidget_doubleClicked(const QModelIndex &index);
    void on_searchButton_clicked();
    void on_resetButton_clicked();
    void on_lineEdit_returnPressed();
    void on_buttonBox_rejected();
    void on_dontAskButton_clicked();
    void on_neverAskButton_clicked();

private:
    Ui::FetchMetadataDialog *m_ui;
    Movie m_movie;
};

#endif // FETCHMETADATADIALOG_H
