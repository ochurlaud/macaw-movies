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

#include "ShowsPannel.h"
#include "ui_ShowsPannel.h"

#include "MacawDebug.h"
#include "ServicesManager.h"
#include "Entities/Episode.h"
#include "Entities/Playlist.h"

ShowsPannel::ShowsPannel(QWidget *parent) :
    MainPannel(parent),
    m_ui(new Ui::ShowsPannel)
{
    m_ui->setupUi(this);
}

ShowsPannel::~ShowsPannel()
{
    delete m_ui;
}

void ShowsPannel::fill(const QList<Movie> &movieList)
{
    Macaw::DEBUG_IN("[ShowsPannel] Enters fill()");

    m_ui->treeWidget->clear();

    ServicesManager *servicesManager = ServicesManager::instance();
    DatabaseManager *databaseManager = servicesManager->databaseManager();

    QList<Episode> l_episodeList = databaseManager->getEpisodesByMovies(movieList);
    QList<Movie> l_matchingMovieList = servicesManager->matchingMovieList();

    foreach (Episode l_episode, l_episodeList) {
        if(l_matchingMovieList.contains(l_episode.movie())) {
            if( (servicesManager->toWatchState()
                 && databaseManager->isMovieInPlaylist(l_episode.movie().id(),
                                                       Playlist::ToWatch)
                 ) || !servicesManager->toWatchState()
               ) {
                this->addEpisodeToPannel(l_episode);
            }
        }
    }
    Macaw::DEBUG_OUT("[ShowsPannel] Exits fill()");
}

void ShowsPannel::addEpisodeToPannel(const Episode &episode)
{
    QStringList l_textValues (QString::number(episode.number()) + "- " + episode.movie().title());
    QTreeWidgetItem *l_episodeItem = new QTreeWidgetItem(l_textValues);

    QList<QTreeWidgetItem*> l_showItemList = m_ui->treeWidget->findItems(episode.show().name(),
                                                                           Qt::MatchFixedString);
    if (!l_showItemList.isEmpty()) {
        QTreeWidgetItem* l_showItem = l_showItemList.at(0);
        bool l_seasonExists(false);
        for (int i = 0 ; i < l_showItem->childCount() ; i++) {
            if (l_showItem->child(i)->text(0) == QString::number(episode.season())) {
                l_showItem->child(i)->addChild(l_episodeItem);
                l_seasonExists = true;
                break;
            }
        }
        if (!l_seasonExists) {
            QTreeWidgetItem *l_seasonItem = new QTreeWidgetItem();
            l_seasonItem->setText(0, QString::number(episode.season()));
            l_seasonItem->addChild(l_episodeItem);
            l_showItem->addChild(l_seasonItem);
        }
    } else {
        QTreeWidgetItem *l_showItem = new QTreeWidgetItem();
        l_showItem->setText(0, episode.show().name());
        QTreeWidgetItem *l_seasonItem = new QTreeWidgetItem();
        l_seasonItem->setText(0, QString::number(episode.season()));

        m_ui->treeWidget->addTopLevelItem(l_showItem);
        l_showItem->addChild(l_seasonItem);
        l_seasonItem->addChild(l_episodeItem);

    }
}
