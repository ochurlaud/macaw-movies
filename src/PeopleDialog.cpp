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

#include "PeopleDialog.h"
#include "ui_PeopleDialog.h"

PeopleDialog::PeopleDialog(int id, int type, QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::PeopleDialog),
    m_type(type)
{
    m_app = qobject_cast<Application *>(qApp);
    m_app->debug("[PeopleDialog] Constructor called");

    m_ui->setupUi(this);
    this->setWindowTitle("Edit People Metadata");
    this->setAttribute(Qt::WA_DeleteOnClose);

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
    m_app->debug("[PeopleDialog] Construction done");
}

PeopleDialog::~PeopleDialog()
{
    delete m_ui;
}

void PeopleDialog::setFirstname(QString firstname)
{
    m_ui->firstnameEdit->setText(firstname);
}

QString PeopleDialog::getFirstname()
{
    return m_ui->firstnameEdit->text();
}

void PeopleDialog::setLastname(QString lastname)
{
    m_ui->lastnameEdit->setText(lastname);
}

QString PeopleDialog::getLastname()
{
    return m_ui->lastnameEdit->text();
}

void PeopleDialog::setRealname(QString realname)
{
    m_ui->realnameEdit->setText(realname);
}

QString PeopleDialog::getRealname()
{
    return m_ui->realnameEdit->text();
}

void PeopleDialog::setBirthday(QDate birthday)
{
    m_ui->birthdayEdit->setDate(birthday);
}

QDate PeopleDialog::getBirthday()
{
    return m_ui->birthdayEdit->date();
}

void PeopleDialog::setBiography(QString biography)
{
    m_ui->biographyEdit->setPlainText(biography);
}

QString PeopleDialog::getBiography()
{
    return m_ui->biographyEdit->toPlainText();
}

void PeopleDialog::on_validationButtons_accepted()
{
    m_app->debug("[PeopleDialog] validationButtons accepted");
    m_people.setFirstname(getFirstname());
    m_people.setLastname(getLastname());
    m_people.setRealname(getRealname());
    m_people.setBirthday(getBirthday());
    m_people.setBiography(getBiography());

    // If type != 0, it means we come from the metadata window
    // If type = 0, it means we directly edit a people
    if (m_type != 0)
    {
        emit(peopleCreated(m_people, m_type));
    }
    else
    {
        m_app->getDatabaseManager()->updatePeople(m_people);
    }

    m_app->debug("[PeopleDialog] validationButtons method done");
}
