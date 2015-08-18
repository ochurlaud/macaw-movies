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

#include "Show.h"

Show::Show(const QString name) :
    Entity(name)
{

}

bool Show::isFinished() const
{
    return m_finished;
}

void Show::setFinished(const bool finished)
{
    m_finished = finished;
}

/*
QList<Episode> Show::EpisodeList() const
{
    return m_episodeList;
}
void Show::setEpisode(const QList<Episode> &episodeList)
{
    m_episodeList = episodeList;
}

void Show::addEpisode(const Episode &episode)
{
    m_episodeList.append(episode);
}

void Show::removeEpisode(const Episode &episode)
{
    m_episodeList.removeAll(episode);
}

void Show::updateEpisode(const Episode &episode)
{
    for (int i = 0 ; i < m_episodeList.size() ; i++)
    {
        if(m_episodeList.at(i).id() == episode.id())
        {
            m_episodeList.replace(i, episode);
        }
   }
}
*/
