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

#include "Series.h"

Series::Series(const QString name) :
    Entity(name)
{

}

bool Series::isFinished() const
{
    return m_finished;
}

void Series::setFinished(const bool finished)
{
    m_finished = finished;
}

QList<SeriesConnection> Series::seriesConnectionList() const
{
    return m_seriesConnectionList;
}
void Series::setSeriesConnection(const QList<SeriesConnection> &seriesConnectionList)
{
    m_seriesConnectionList = seriesConnectionList;
}

void Series::addSeriesConnection(const SeriesConnection &seriesConnection)
{
    m_seriesConnectionList.append(seriesConnection);
}

void Series::removeSeriesConnection(const SeriesConnection &seriesConnection)
{
    m_seriesConnectionList.removeAll(seriesConnection);
}

void Series::updateSeriesConnection(const SeriesConnection &seriesConnection)
{
    for (int i = 0 ; i < m_seriesConnectionList.size() ; i++)
    {
        if(m_seriesConnectionList.at(i).id() == seriesConnection.id())
        {
            m_seriesConnectionList.replace(i, seriesConnection);
        }
   }
}

