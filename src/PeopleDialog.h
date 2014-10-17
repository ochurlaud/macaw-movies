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

#ifndef PEOPLEDIALOG_H
#define PEOPLEDIALOG_H

#include <QDialog>
#include "Application.h"

class Application;

namespace Ui {
class PeopleDialog;
}

class PeopleDialog : public QDialog
{
    Q_OBJECT

    public:
        explicit PeopleDialog(int = 0, int = 0, QWidget *parent = 0);
        ~PeopleDialog();
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
        void peopleCreated(People, int);

    private:
        Ui::PeopleDialog *m_ui;
        People m_people;
        Application *m_app;
        int m_type;
};

#endif // PEOPLEDIALOG_H
