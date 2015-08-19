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

#ifndef SHOWSPANNEL_H
#define SHOWSPANNEL_H

#include "MainPannel.h"

class Episode;

namespace Ui {
class ShowsPannel;
}
/**
 * @brief The ShowsPannel class
 *
 * @author Olivier CHURLAUD <olivier@churlaud.com>
 */
class ShowsPannel : public MainPannel
{
    Q_OBJECT

public:
    explicit ShowsPannel(QWidget *parent = 0);
    ~ShowsPannel();
    void fill(const QList<Movie> &movieList);


private:
    Ui::ShowsPannel *m_ui;
    void addEpisodeToPannel(const Episode &episode);
};

#endif // SHOWSPANNEL_H
