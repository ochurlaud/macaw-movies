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

#ifndef PEOPLEWINDOW_H
#define PEOPLEWINDOW_H

#include <QDialog>
#include "Application.h"

class Application;

namespace Ui {
class PeopleWindow;
}

class PeopleWindow : public QDialog
{
    Q_OBJECT

    public:
        explicit PeopleWindow(int = 0, QWidget *parent = 0);
        ~PeopleWindow();
        void setFirstname(QString);
        QString getFirstname();
        void setLastname(QString);
        QString getLastname();
        void setRealname(QString);
        QString getRealname();
        void setBirthday(QDate);
        QDate getBirthday();
        void setBiography(QString);
        QString getBiography();

    public slots:
        void on_validationButtons_accepted();

    signals:
        void peopleCreated(People);

    private:
        Ui::PeopleWindow *m_ui;
        People m_people;
        Application *m_app;
};

#endif // PEOPLEWINDOW_H
