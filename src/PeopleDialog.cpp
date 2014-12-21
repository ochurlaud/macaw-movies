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

PeopleDialog::PeopleDialog(int id, QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::PeopleDialog)
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

PeopleDialog::PeopleDialog(People people, QWidget *parent):
    QDialog(parent),
    m_ui(new Ui::PeopleDialog)
{
    m_people = people;
    m_app = qobject_cast<Application *>(qApp);
    m_app->debug("[PeopleDialog] Constructor called (People arg)");

    m_ui->setupUi(this);
    this->setWindowTitle("Edit People Metadata");
    this->setAttribute(Qt::WA_DeleteOnClose);

    setFirstname(m_people.getFirstname());
    setLastname(m_people.getLastname());
    setRealname(m_people.getRealname());
    setBirthday(m_people.getBirthday());
    setBiography(m_people.getBiography());

    m_app->debug("[PeopleDialog] Construction done (People arg)");
}

PeopleDialog::~PeopleDialog()
{
    delete m_ui;
    m_app->debug("[PeopleDialog] Destructed");
}

void PeopleDialog::setFirstname(const QString firstname)
{
    m_ui->firstnameEdit->setText(firstname);
}

QString PeopleDialog::getFirstname() const
{
    return m_ui->firstnameEdit->text();
}

void PeopleDialog::setLastname(const QString lastname)
{
    m_ui->lastnameEdit->setText(lastname);
}

QString PeopleDialog::getLastname() const
{
    return m_ui->lastnameEdit->text();
}

void PeopleDialog::setRealname(const QString realname)
{
    m_ui->realnameEdit->setText(realname);
}

QString PeopleDialog::getRealname() const
{
    return m_ui->realnameEdit->text();
}

void PeopleDialog::setBirthday(const QDate birthday)
{
    m_ui->birthdayEdit->setSpecialValueText("--/--/----");
    if (birthday.isNull() || birthday == m_ui->birthdayEdit->minimumDate())
    {
        // The following line is a hack for setting the specialValueText instead of defaultDate
        m_ui->birthdayEdit->setDate(QDate::fromString("01/01/0001", "dd/MM/yyyy"));
    }
    else
    {
        m_ui->birthdayEdit->setDate(birthday);
        m_people.setBirthday(birthday);
    }
}

QDate PeopleDialog::getBirthday() const
{
    // if the date is the minimum date, it means it's not the good one
    // so we return a NULL date
    if (m_ui->birthdayEdit->date() == m_ui->birthdayEdit->minimumDate())
    {

        return QDate();
    }
    else
    {

        return m_ui->birthdayEdit->date();
    }
}

void PeopleDialog::setBiography(const QString biography)
{
    m_ui->biographyEdit->setPlainText(biography);
}

QString PeopleDialog::getBiography() const
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

    // If type != 0, it means we come from the movie dialog
    // If type = 0, it means we directly edit a people
    if (m_people.getType() != 0)
    {
        m_app->debug("[PeopleDialog] validationButtons method: type="
                     +QString::number(m_people.getType()));
        emit(peopleCreated(m_people));
    }
    else
    {
        m_app->debug("[PeopleDialog] validationButtons method: type=0");
        m_app->getDatabaseManager()->updatePeople(m_people);
    }

    m_app->debug("[PeopleDialog] validationButtons method done");
}

void PeopleDialog::on_resetBirthdayBtn_clicked()
{
    // We set the date to a QDate < date_min so that it prints the specialValueText
    m_ui->birthdayEdit->setDate(QDate::fromString("01/01/0001", "dd/MM/yyyy"));
}
