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

#ifndef PEOPLE_H
#define PEOPLE_H

#include <QtWidgets>

class People
{
public:
    People();
    int getId() const;
    void setId(int);
    QString getLastname() const;
    void setLastname(QString);
    QString getFirstname() const;
    void setFirstname(QString);
    QString getRealname() const;
    void setRealname(QString);
    QDate getBirthday() const;
    void setBirthday(QDate);
    QString getBiography() const;
    void setBiography(QString);
    bool operator== (const People&);
    bool operator!= (const People&);

private:
    int m_id;
    QString m_lastname;
    QString m_firstname;
    QString m_realname;
    QDate m_birthday;
    QString m_biography;
};

#endif // PEOPLE_H
