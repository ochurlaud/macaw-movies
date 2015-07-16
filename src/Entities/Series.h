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

#ifndef SERIES_H
#define SERIES_H

#include "Entities/Entity.h"
#include "Entities/Movie.h"
#include "Entities/SeriesConnection.h"

class SeriesConnection;

class Series : public Entity
{
public:
    Series(QString const name = "");
    bool isFinished() const;
    void setFinished(const bool finished);
    QList<SeriesConnection> seriesConnectionList() const;
    void setSeriesConnection(const QList<SeriesConnection> &seriesConnectionList);
    void addSeriesConnection(const SeriesConnection &seriesConnection);
    void removeSeriesConnection(const SeriesConnection &seriesConnection);
    void updateSeriesConnection(const SeriesConnection &seriesConnection);

private:
    bool m_finished; // series finished of not
    QList<SeriesConnection> m_seriesConnectionList;
};

#endif // SERIES_H
