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

#ifndef METADATAWINDOW_H
#define METADATAWINDOW_H

#include <QDialog>
#include "Application.h"
#include "PeopleWindow.h"

class Application;

namespace Ui {
class MetadataWindow;
}

class MetadataWindow : public QDialog
{
    Q_OBJECT

    public:
        explicit MetadataWindow(int, QWidget *parent = 0);
        ~MetadataWindow();
        void setTitle(QString);
        QString getTitle();
        void setOriginalTitle(QString);
        QString getOriginalTitle();
        void setReleaseDate(QDate);
        QDate getReleaseDate();
        void setCountry(QString);
        QString getCountry();
        void setSynopsis(QString);
        QString getSynopsis();
        void setPeople(QVector<People>, int);
        QVector<People> getPeople(int);
        void addPeople(People&, int);
        void delPeople(People&, int);
        void setDirectors(QVector<People>);
        QVector<People> getDirectors();
        void addDirector(People&);
        void delDirector(People&);
        void setProducers(QVector<People>);
        QVector<People> getProducers();
        void addProducer(People&);
        void delProducer(People&);
        void setActors(QVector<People>);
        QVector<People> getActors();
        void addActor(People&);
        void delActor(People&);
        void on_peopleEdit_textEdited(int);
        void addPeopleButton_clicked(int);
        void delPeopleButton_clicked(int);


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
        void peopleWindow_peopleCreated(People, int);
        void customMenuRequested(QPoint);
        void showPeopleWindow();

    private:
        Ui::MetadataWindow *m_ui;
        Movie m_movie;
        Application *m_app;
};

#endif // METADATAWINDOW_H
