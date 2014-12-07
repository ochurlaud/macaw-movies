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

#ifndef TAG_H
#define TAG_H

#include <QtWidgets>

class Tag
{
public:
    Tag();
    Tag(QString name);
    int getId() const;
    void setId(int id);
    QString getName() const;
    void setName(QString name);
    bool operator== (const Tag &other);
    bool operator!= (const Tag &other);

private:
    int m_id;
    QString m_name;
};

#endif // TAG_H
