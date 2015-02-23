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
    void setLastname(const QString lastname);
    QString getFirstname() const;
    void setFirstname(const QString firstname);
    QString getRealname() const;
    void setRealname(const QString realname);
    QDate getBirthday() const;
    void setBirthday(const QDate birthday);
    QString getBiography() const;
    void setBiography(const QString biography);
    int getType() const;
    void setType(const int type);

    bool operator== (const People&);
    bool operator!= (const People&);

private:
    int m_id;
    QString m_lastname;
    QString m_firstname;
    QString m_realname;
    QDate m_birthday;
    QString m_biography;
    int m_type;
};

#endif // PEOPLE_H
