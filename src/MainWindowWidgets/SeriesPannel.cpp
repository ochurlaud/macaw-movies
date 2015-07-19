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

#include "SeriesPannel.h"
#include "ui_SeriesPannel.h"

SeriesPannel::SeriesPannel(QWidget *parent) :
    MainPannel(parent),
    m_ui(new Ui::SeriesPannel)
{
    m_ui->setupUi(this);
}

SeriesPannel::~SeriesPannel()
{
    delete m_ui;
}

void SeriesPannel::fill(const QList<Movie> &movieList)
{
    Macaw::DEBUG_IN("[SeriesPannel] Enters fill()");

    m_ui->treeWidget->clear();

    ServicesManager *servicesManager = ServicesManager::instance();
    DatabaseManager *databaseManager = servicesManager->databaseManager();

    QList<Episode> l_episodeList = databaseManager->getEpisodesByMovies(movieList);

    foreach (Episode l_episode, l_episodeList) {
        QStringList l_textValues (QString::number(l_episode.number()) + "- " + l_episode.movie().title());
        QTreeWidgetItem *l_episodeItem = new QTreeWidgetItem(l_textValues);

        QList<QTreeWidgetItem*> l_seriesItemList = m_ui->treeWidget->findItems(l_episode.series().name(), Qt::MatchFixedString);
        if (!l_seriesItemList.isEmpty()) {
            QTreeWidgetItem* l_seriesItem = l_seriesItemList.at(0);
            bool l_seasonExists(false);
            for (int i = 0 ; i < l_seriesItem->childCount() ; i++) {
                if (l_seriesItem->child(i)->text(0) == QString::number(l_episode.season())) {
                    l_seriesItem->child(i)->addChild(l_episodeItem);
                    l_seasonExists = true;
                    break;
                }
            }
            if (!l_seasonExists) {
                QTreeWidgetItem *l_seasonItem = new QTreeWidgetItem();
                l_seasonItem->setText(0, QString::number(l_episode.season()));
                l_seasonItem->addChild(l_episodeItem);
                l_seriesItem->addChild(l_seasonItem);
            }
        } else {
            QTreeWidgetItem *l_seriesItem = new QTreeWidgetItem();
            l_seriesItem->setText(0, l_episode.series().name());
            QTreeWidgetItem *l_seasonItem = new QTreeWidgetItem();
            l_seasonItem->setText(0, QString::number(l_episode.season()));

            m_ui->treeWidget->addTopLevelItem(l_seriesItem);
            l_seriesItem->addChild(l_seasonItem);
            l_seasonItem->addChild(l_episodeItem);

       }
    }
    Macaw::DEBUG_OUT("[SeriesPannel] Exits fill()");
}
