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

#include <QMessageBox>

#include "enumerations.h"
#include "MacawDebug.h"

FetchMetadataDialog::FetchMetadataDialog(const Movie &movie, QList<Movie> amoviesPropositionList, QWidget *parent):
    QDialog(parent),
    m_ui(new Ui::FetchMetadataDialog)
{
    Macaw::DEBUG("[FetchMetadataDialog] Constructor called");
    m_ui->setupUi(this);

    m_movie = movie;
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle("Search movie...");
    m_ui->lineEdit->setText(m_movie.title());
    m_ui->moviePathLabel->setText(m_movie.fileAbsolutePath());

    this->setMovieList(amoviesPropositionList);
}

FetchMetadataDialog::~FetchMetadataDialog()
{
    delete m_ui;
}

void FetchMetadataDialog::on_buttonBox_accepted()
{
    if (m_ui->listWidget->selectedItems().isEmpty())
    {
        QMessageBox::warning(this, "Warning", "You must select a movie!");
    }
    else
    {
        int tmdbID = m_ui->listWidget->selectedItems().at(0)->data(Macaw::ObjectId).toInt();
        m_movie.setTmdbId(tmdbID);

        emit(selectedMovie(m_movie));
        this->accept();
    }
}

void FetchMetadataDialog::on_listWidget_doubleClicked(const QModelIndex &index)
{
    int tmdbID = index.data(Macaw::ObjectId).toInt();
    m_movie.setTmdbId(tmdbID);
    emit(selectedMovie(m_movie));
    this->accept();
}

void FetchMetadataDialog::on_searchButton_clicked()
{
    this->m_ui->searchButton->setEnabled(false);
    emit(searchMovies(m_ui->lineEdit->text()));
}

void FetchMetadataDialog::on_lineEdit_returnPressed()
{
    if (!this->m_ui->searchButton->isEnabled()) {
        emit(searchMovies(m_ui->lineEdit->text()));
        this->m_ui->searchButton->setEnabled(false);
    }
}

void FetchMetadataDialog::setMovieList(const QList<Movie> &movieList)
{
    m_ui->listWidget->clear();
    this->m_ui->searchButton->setEnabled(true);
    if (!movieList.isEmpty()) {
        foreach (Movie l_movie, movieList) {
            QListWidgetItem *l_item = new QListWidgetItem;
            QString l_textItem = l_movie.title()
                    + " [" + QString::number(l_movie.releaseDate().year())
                    + ']';
            l_item->setText(l_textItem);
            l_item->setData(Macaw::ObjectId, l_movie.tmdbId());
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
    emit searchCanceled();
    this->deleteLater();
    this->accept();
}

void FetchMetadataDialog::on_dontAskButton_clicked()
{
    emit dontAskUser();
    m_ui->buttonBox->rejected();
}

void FetchMetadataDialog::on_neverAskButton_clicked()
{
    emit neverAskUser(m_movie);
    m_ui->buttonBox->rejected();
}
