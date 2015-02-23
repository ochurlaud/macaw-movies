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

#include <QtGui>
#include <QtWidgets>

#include "Application.h"
#include "Entities/Movie.h"

class Application;

namespace Ui {
class FetchMetadataDialog;
}

class FetchMetadataDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FetchMetadataDialog(Movie movie, QList<Movie> moviesPropositionList, QWidget *parent = 0);
    ~FetchMetadataDialog();
    QListWidget *getListWidget();

signals:
    void selectedMovie(int tmdbID);

private slots:
    void on_buttonBox_accepted();

    void on_listWidget_doubleClicked(const QModelIndex &index);

private:
    Ui::FetchMetadataDialog *m_ui;
    Application *m_app;
};

#endif // FETCHMETADATADIALOG_H
