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

#include "PeopleDialog.h"
#include "ui_PeopleDialog.h"

PeopleDialog::PeopleDialog(int id, QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::PeopleDialog)
{
    Macaw::DEBUG("[PeopleDialog] Constructor called");

    DatabaseManager *databaseManager = ServicesManager::instance()->databaseManager();

    m_ui->setupUi(this);
    this->setWindowTitle(tr("Edit People Metadata"));
    this->setAttribute(Qt::WA_DeleteOnClose);

    // If id is 0 it means we create a new People
    if (id != 0)
    {
        m_people = databaseManager->getOnePeopleById(id);
        setName(m_people.name());
        setBirthday(m_people.birthday());
        setBiography(m_people.biography());
    }
    Macaw::DEBUG("[PeopleDialog] Construction done");
}

PeopleDialog::PeopleDialog(People people, QWidget *parent):
    QDialog(parent),
    m_ui(new Ui::PeopleDialog)
{
    m_people = people;
    Macaw::DEBUG("[PeopleDialog] Constructor called (People arg)");

    m_ui->setupUi(this);
    this->setWindowTitle("Edit People Metadata");
    this->setAttribute(Qt::WA_DeleteOnClose);

    setName(m_people.name());
    setBirthday(m_people.birthday());
    setBiography(m_people.biography());

    Macaw::DEBUG("[PeopleDialog] Construction done (People arg)");
}

PeopleDialog::~PeopleDialog()
{
    delete m_ui;
    Macaw::DEBUG("[PeopleDialog] Destructed");
}

void PeopleDialog::setName(const QString name)
{
    m_ui->nameEdit->setText(name);
}

QString PeopleDialog::getName() const
{
    return m_ui->nameEdit->text();
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

/**
 * @brief Slot triggered when `Ok` is clicked
 * Updates the person or tell the MovieDialog that it's done
 */
void PeopleDialog::on_validationButtons_accepted()
{
    Macaw::DEBUG("[PeopleDialog] validationButtons accepted");
    m_people.setName(getName());
    m_people.setBirthday(getBirthday());
    m_people.setBiography(getBiography());

    // If type != 0, it means we come from the movie dialog
    // If type = 0, it means we directly edit a people
    if (m_people.type() != 0)
    {
        Macaw::DEBUG("[PeopleDialog] validationButtons method: type="
                     +QString::number(m_people.type()));
        emit(peopleCreated(m_people));
    }
    else
    {
        Macaw::DEBUG("[PeopleDialog] validationButtons method: type=0");
        DatabaseManager *databaseManager = ServicesManager::instance()->databaseManager();

        databaseManager->updatePeople(m_people);
    }

    Macaw::DEBUG("[PeopleDialog] validationButtons method done");
}

/**
 * @brief Slot triggered when the ResetBirthday Button is clicked
 * Set the date to a date < date_min to print the specialValueText
 */
void PeopleDialog::on_resetBirthdayBtn_clicked()
{
    m_ui->birthdayEdit->setDate(QDate::fromString("01/01/0001", "dd/MM/yyyy"));
}
