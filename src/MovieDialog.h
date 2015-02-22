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

#ifndef MOVIEDIALOG_H
#define MOVIEDIALOG_H

#include <QDialog>
#include "Application.h"
#include "PeopleDialog.h"
#include "Entities/Movie.h"

class Application;

namespace Ui {
class MovieDialog;
}

/**
 * @brief Dialog to edit the metadata of a Movie
 */
class MovieDialog : public QDialog
{
Q_OBJECT

    //Methods
public:
    explicit MovieDialog(int, QWidget *parent = 0);
    ~MovieDialog();

private:
    void setTitle(const QString title);
    QString getTitle() const;
    void setOriginalTitle(const QString originalTitle);
    QString getOriginalTitle() const;
    void setReleaseDate(const QDate releaseDate);
    QDate getReleaseDate() const;
    void setCountry(const QString country);
    QString getCountry() const;
    void setSynopsis(const QString synopsis);
    QString getSynopsis() const;
    void setPeopleList(const QList<People> &peopleList);
    QList<People> getPeopleList(int type);
    void addPeople(const People &people);
    void delPeople(const People &people);
    void updatePeople(const People &people);
    void setMovieSelectedTagList(const QList<Tag> &tagList);
    void on_peopleEdit_textEdited(int);
    void addPeopleButton_clicked(int);
    void delPeopleButton_clicked(int);
    QListWidget* getFocusedListWidget();
    QList<People> getFocusedListPeople();


private slots:
    void on_validationButtons_accepted();
    void on_addDirectorButton_clicked();
    void on_addProducerButton_clicked();
    void on_addActorButton_clicked();
    void on_delDirectorButton_clicked();
    void on_delProducerButton_clicked();
    void on_delActorButton_clicked();
    void on_directorEdit_textEdited();
    void on_producerEdit_textEdited();
    void on_actorEdit_textEdited();
    void on_addNewTagButton_clicked();
    void peopleDialog_peopleCreated(People);
    void customMenuRequested(QPoint);
    void showPeopleDialog();
    void on_resetReleaseDateBtn_clicked();

    // Atributs
private:
    Ui::MovieDialog *m_ui;
    Movie m_movie;
    Application *m_app;
};

#endif // MOVIEDIALOG_H
