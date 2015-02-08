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

#ifndef METADATAFETCHERDIALOG_H
#define METADATAFETCHERDIALOG_H

#include <QtGui>
#include <QtWidgets>

#include "Application.h"
#include "Entities/Movie.h"

class Application;

namespace Ui {
class MetadataFetcherDialog;
}

class MetadataFetcherDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MetadataFetcherDialog(QString title, QList<Movie> moviesPropositionList, QWidget *parent = 0);
    ~MetadataFetcherDialog();
    QListWidget *getListWidget();

signals:
    void selectedMovie(int tmdbID);

private slots:
    void on_buttonBox_accepted();

    void on_listWidget_doubleClicked(const QModelIndex &index);

private:
    Ui::MetadataFetcherDialog *m_ui;
    Application *m_app;
};

#endif // METADATAFETCHERDIALOG_H
