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

#ifndef METADATAPANNEL_H
#define METADATAPANNEL_H

#include <QWidget>

#include "Application.h"
#include "Entities/Movie.h"
#include "Entities/People.h"

namespace Ui {
class MetadataPannel;
}

class MetadataPannel : public QWidget
{
    Q_OBJECT

public:
    explicit MetadataPannel(QWidget *parent = 0);
    ~MetadataPannel();
    void fill(const Movie &movie);
    void resizeEvent(QResizeEvent *event);

private:
    Ui::MetadataPannel *m_ui;
    Movie m_movie;
    void setPoster();
};

#endif // METADATAPANNEL_H
