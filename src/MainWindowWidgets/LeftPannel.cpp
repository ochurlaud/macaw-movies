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

#include "LeftPannel.h"
#include "ui_LeftPannel.h"

/**
 * @brief Constructor
 * @author Olivier CHURLAUD <olivier@churlaud.com>
 *
 * @param parent
 */
LeftPannel::LeftPannel(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::LeftPannel)
{
    m_ui->setupUi(this);
    m_ui->listWidget->setContentsMargins(0,1,1,0);
    connect(m_ui->listWidget, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(on_customContextMenuRequested(const QPoint &)));
    m_ui->listWidget->addAction(m_ui->actionEdit_leftPannelMetadata);

    m_selectedId = 0;
    m_typeElement =  Macaw::isPeople;
    m_typePeople = People::Director;
}

/**
 * @brief Destructor
 */
LeftPannel::~LeftPannel()
{
    delete m_ui;
}

/**
 * @brief Fill the leftPannel
 * @author Olivier CHURLAUD <olivier@churlaud.com>
 */
void LeftPannel::fill()
{
    Macaw::DEBUG_IN("[LefPannel] Enters fill()");

    this->setElementIdsList();
    this->fillListWidget();

    Macaw::DEBUG_OUT("[LefPannel] Exits fill()");
}

/**
 * @brief Set the ElementIdsList, used to fill the listWidget
 */
void LeftPannel::setElementIdsList()
{
    Macaw::DEBUG_IN("[LeftPannel] Enters setElementIdsList()");

    m_elementIdsList.clear();

    ServicesManager *servicesManager = ServicesManager::instance();
    DatabaseManager *databaseManager = servicesManager->databaseManager();

    QList<Movie> l_matchingMovieList = servicesManager->matchingMovieList();

    foreach(Movie l_movie, l_matchingMovieList) {
        if( (servicesManager->toWatchState()
                 && databaseManager->isMovieInPlaylist(l_movie.id(), Playlist::ToWatch)
                 ) || !servicesManager->toWatchState()
               ) {
            switch (m_typeElement)
            {
                case Macaw::isPeople:
                {
                    QList<People> l_peopleList = l_movie.peopleList(m_typePeople);
                    this->updateElementIdsList(l_peopleList);
                    break;
                }
                case Macaw::isTag:
                {
                    QList<Tag> l_tagList = l_movie.tagList();
                    this->updateElementIdsList(l_tagList);
                    break;
                }
            }
        }
    }
    Macaw::DEBUG_OUT("[LefPannel] Exits setElementIdsList()");
}

/**
 * @brief Add/Update the elementIdsList
 * @author Olivier CHURLAUD <olivier@churlaud.com>
 *
 * @param list of objects of the movie
 */
template<typename T> void LeftPannel::updateElementIdsList(const QList<T> &entityList)
{
    Macaw::DEBUG_IN("[LeftPannel] Enters updateElementIdsList");
    if(entityList.isEmpty()
       && !m_elementIdsList.contains(-1)
      ) {
        m_elementIdsList.append(-1);
    }
    foreach(T l_entity, entityList) {
        if(!m_elementIdsList.contains(l_entity.id())) {
            m_elementIdsList.append(l_entity.id());
        }
    }

    Macaw::DEBUG_OUT("[LeftPannel] Exits updateElementIdsList");
}

/**
 * @brief fill the listWidget based on m_elementIdsList
 * @author Olivier CHURLAUD <olivier@churlaud.com>
 */
void LeftPannel::fillListWidget()
{
    Macaw::DEBUG_IN("[LeftPannel] Enters fillListWidget()");

    m_ui->listWidget->clear();

    DatabaseManager *databaseManager = ServicesManager::instance()->databaseManager();

    // Add the "All" element
    if(m_typeElement != Macaw::isPlaylist) {
        // First space needed for sorting
        Entity l_entity(" All");
        l_entity.setId(0);

        this->addEntityToListWidget(l_entity);
    }

    foreach(int l_objectId, m_elementIdsList) {
        if(l_objectId == -1) {
            // First space needed for sorting
            Entity l_entity(" Unknown");
            l_entity.setId(-1);

            this->addEntityToListWidget(l_entity);
        } else {
            Entity l_entity;
            switch (m_typeElement)
            {
            case Macaw::isPeople:
                l_entity = databaseManager->getOnePeopleById(l_objectId);
                break;
            case Macaw::isTag:
                l_entity = databaseManager->getOneTagById(l_objectId);
                break;
            }

            this->addEntityToListWidget(l_entity);
        }
    }
    if(m_ui->listWidget->selectedItems().isEmpty()) {
        m_ui->listWidget->item(0)->setSelected(true);
    }
    m_ui->listWidget->sortItems();

    Macaw::DEBUG_OUT("[LeftPannel] Exits fillListWidget()");
}

/**
 * @brief Add an element to the list widget, based on an Entity
 * @author Olivier CHURLAUD <olivier@churlaud.com>
 *
 * @param Entity to add in ListWidget
 */
void LeftPannel::addEntityToListWidget(const Entity &entity)
{
    Macaw::DEBUG_IN("[LeftPannel] Enters addEntityToListWidget()");

    QListWidgetItem *l_item = new QListWidgetItem(entity.name());
    l_item->setData(Macaw::ObjectId, entity.id());
    l_item->setData(Macaw::ObjectType, m_typeElement);
    l_item->setData(Macaw::PeopleType, m_typePeople);

    if (m_selectedId == entity.id()) {
        l_item->setSelected(true);
    }

    m_ui->listWidget->addItem(l_item);
    Macaw::DEBUG_OUT("[LeftPannel] Exits addEntityToListWidget()");
}

/**
 * @brief Slot triggered when an option from leftPannelBox is selected.
 * Request to update the mainPannel
 *
 * @author Olivier CHURLAUD <olivier@churlaud.com>
 *
 * @param type of Element (0 = tag, else it's a people, of type `type`)
 */
void LeftPannel::on_leftPannelBox_activated(int type)
{
    Macaw::DEBUG_IN("[LeftPannel] Enters on_leftPannelBox_activated()");

    // 0 = Tags
    // People::typePeople
    int l_peopleType = type;
    if (type == 0) {
        m_typeElement = Macaw::isTag;
    } else {
        m_typeElement = Macaw::isPeople;
    }
    m_typePeople = l_peopleType;

    // We change of Macaw::typeElement,
    // so we reinitialise the selection in the leftPannel
    m_selectedId = 0;

    ServicesManager *servicesManager = ServicesManager::instance();
    servicesManager->pannelsUpdate();

    Macaw::DEBUG_OUT("[LeftPannel] Exits on_leftPannelBox_activated()");
}


/**
 * @brief Slot triggered when the context menu is requested.
 *
 * 1. Create the menu.
 * 2. Check that an editable element is selected
 * 3. Add actions on the menu
 * 4. Display it
 *
 * @author Olivier CHURLAUD <olivier@churlaud.com>
 * @param point: coordinates of the cursor when requested
 */
void LeftPannel::on_customContextMenuRequested(const QPoint &point)
{
    Macaw::DEBUG("[LeftPannel] customContextMenuRequested()");
    QMenu *l_menu = new QMenu(this);

    // One item selected which id is not 0 or -1
    // (not to be "All" or "Unknown")
    if(!m_ui->listWidget->selectedItems().isEmpty()
        && m_ui->listWidget->selectedItems().at(0)->data(Macaw::ObjectId) > 0)
    {
        l_menu->addAction(m_ui->actionEdit_leftPannelMetadata);
        l_menu->exec(m_ui->listWidget->mapToGlobal(point));
    }
}

/**
 * @brief Slot triggered when the edition of the metadata of an element from the leftPannel is asked.
 *
 * 1. Check if editable
 * 2. Check the type of element selected
 * 3. Create and show a PeopleDialog (or a TagDialog or a ...) based on the id of this element
 *
 * @author Olivier CHURLAUD <olivier@churlaud.com>
 */
void LeftPannel::on_actionEdit_leftPannelMetadata_triggered()
{
    Macaw::DEBUG("[LeftPannel] actionEdit_leftPannelMetadata_triggered()");

    // The left pannel must have one item selected which id is not -1 or 0
    // (not to be "All" or "Unknown")
    if(!m_ui->listWidget->selectedItems().isEmpty()) {
        int l_id = m_ui->listWidget->selectedItems().at(0)->data(Macaw::ObjectId).toInt();
        // It's editable only if id is not 0 or -1
        if(l_id > 0) {
            Macaw::DEBUG("Element is editable");
            int l_typeElement = m_ui->listWidget->selectedItems().at(0)->data(Macaw::ObjectType).toInt();
            if (l_typeElement == Macaw::isPeople) {
                PeopleDialog *l_movieDialog = new PeopleDialog(l_id);
                ServicesManager *servicesManager = ServicesManager::instance();
                connect(l_movieDialog, SIGNAL(destroyed()), servicesManager, SLOT(pannelsUpdate()));
                l_movieDialog->show();
            } else if (l_typeElement == Macaw::isTag) {
                qDebug() << "Tag !";
            } else if (l_typeElement == Macaw::isPlaylist) {
                qDebug() << "Playlist !";
            }
        }
    }
}

/**
 * @brief Slot triggered when an element of the listWidget is selected
 * Request to fill the Main Pannel according to the selected element
 *
 * @author Olivier CHURLAUD <olivier@churlaud.com>
 */
void LeftPannel::on_listWidget_itemSelectionChanged()
{
    Macaw::DEBUG("[LeftPannel] item selected on listWidget");
    if (m_ui->listWidget->selectedItems().count() > 0) {
        QListWidgetItem *l_item = m_ui->listWidget->selectedItems().first();

        m_selectedId = l_item->data(Macaw::ObjectId).toInt();
        emit updateMainPannel();
    }
}
