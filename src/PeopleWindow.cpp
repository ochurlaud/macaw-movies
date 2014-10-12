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

#include "PeopleWindow.h"
#include "ui_PeopleWindow.h"

PeopleWindow::PeopleWindow(int type, int id, QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::PeopleWindow),
    m_type(type)
{
    m_app = qobject_cast<Application *>(qApp);
    m_app->debug("[PeopleWindow] Constructor called");

    m_ui->setupUi(this);

    // If id is 0 it means we create a new People
    if (id != 0)
    {
        m_people = m_app->getDatabaseManager()->getOnePeopleById(id);
        setFirstname(m_people.getFirstname());
        setLastname(m_people.getLastname());
        setRealname(m_people.getRealname());
        setBirthday(m_people.getBirthday());
        setBiography(m_people.getBiography());
    }
    m_app->debug("[PeopleWindow] Construction done");
}

PeopleWindow::~PeopleWindow()
{
    delete m_ui;
}

void PeopleWindow::setFirstname(QString firstname)
{
    m_ui->firstnameEdit->setText(firstname);
}

QString PeopleWindow::getFirstname()
{
    return m_ui->firstnameEdit->text();
}

void PeopleWindow::setLastname(QString lastname)
{
    m_ui->lastnameEdit->setText(lastname);
}

QString PeopleWindow::getLastname()
{
    return m_ui->lastnameEdit->text();
}

void PeopleWindow::setRealname(QString realname)
{
    m_ui->realnameEdit->setText(realname);
}

QString PeopleWindow::getRealname()
{
    return m_ui->realnameEdit->text();
}

void PeopleWindow::setBirthday(QDate birthday)
{
    m_ui->birthdayEdit->setDate(birthday);
}

QDate PeopleWindow::getBirthday()
{
    return m_ui->birthdayEdit->date();
}

void PeopleWindow::setBiography(QString biography)
{
    m_ui->biographyEdit->setPlainText(biography);
}

QString PeopleWindow::getBiography()
{
    return m_ui->biographyEdit->toPlainText();
}

void PeopleWindow::on_validationButtons_accepted()
{
    m_app->debug("[PeopleWindow] validationButtons accepted");
    m_people.setFirstname(getFirstname());
    m_people.setLastname(getLastname());
    m_people.setRealname(getRealname());
    m_people.setBirthday(getBirthday());
    m_people.setBiography(getBiography());
    emit(peopleCreated(m_people, m_type));

    m_app->debug("[PeopleWindow] validationButtons method done");
}
