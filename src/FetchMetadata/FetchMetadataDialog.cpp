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

#include "FetchMetadata/FetchMetadataDialog.h"
#include "ui_FetchMetadataDialog.h"

FetchMetadataDialog::FetchMetadataDialog(Movie movie, QList<Movie> moviesPropositionList, QWidget *parent):
    QDialog(parent),
    m_ui(new Ui::FetchMetadataDialog)
{
    m_app = qobject_cast<Application *>(qApp);
    m_app->debug("[FetchMetadataDialog] Constructor called");
    m_ui->setupUi(this);

    m_movie = movie;
    m_ui->lineEdit->setText(m_movie.title());
    m_ui->moviePathLabel->setText(m_movie.filePath());

    setMovieList(moviesPropositionList);
}

FetchMetadataDialog::~FetchMetadataDialog()
{
    delete m_ui;
}


void FetchMetadataDialog::on_buttonBox_accepted()
{
    if (m_ui->listWidget->selectedItems().count() == 0)
    {
        QMessageBox::warning(this, "Warning", "You must select a movie!");
    }
    else
    {
        int tmdbID = m_ui->listWidget->selectedItems().at(0)->data(Macaw::ObjectId).toInt();
        m_movie.setId(tmdbID);
        emit(selectedMovie(m_movie));
        this->accept();
    }
}

void FetchMetadataDialog::on_listWidget_doubleClicked(const QModelIndex &index)
{
    int tmdbID = index.data(Macaw::ObjectId).toInt();
    m_movie.setId(tmdbID);
    emit(selectedMovie(m_movie));
    this->accept();
}

void FetchMetadataDialog::on_searchButton_clicked()
{
    emit(searchMovies(m_ui->lineEdit->text()));
}

void FetchMetadataDialog::on_lineEdit_returnPressed()
{
    emit(searchMovies(m_ui->lineEdit->text()));
}

void FetchMetadataDialog::setMovieList(QList<Movie> &movieList)
{
    m_ui->listWidget->clear();
    if (movieList.count() != 0) {
        foreach (Movie l_movie, movieList) {
            QListWidgetItem *l_item = new QListWidgetItem;
            QString l_textItem = l_movie.title()
                    + " [" + QString::number(l_movie.releaseDate().year())
                    + "]";
            l_item->setText(l_textItem);
            l_item->setData(Macaw::ObjectId, l_movie.id());
            m_ui->listWidget->addItem(l_item);
        }
    }
}

void FetchMetadataDialog::on_resetButton_clicked()
{
    m_ui->lineEdit->setText(m_movie.title());
}

void FetchMetadataDialog::on_buttonBox_rejected()
{
    this->accept();
}
