#include "LeftPannel.h"
#include "ui_LeftPannel.h"

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

LeftPannel::~LeftPannel()
{
    delete m_ui;
}

void LeftPannel::fill()
{
    m_ui->listWidget->clear();
    m_elementIdsList.clear();
    DatabaseManager *databaseManager = ServicesManager::instance()->databaseManager();

    ServicesManager *servicesManager = ServicesManager::instance();
    QList<Movie> l_authorizedMoviesList = servicesManager->authorizedMoviesList();

    if(m_typeElement != Macaw::isPlaylist) {
        QListWidgetItem *l_item = new QListWidgetItem(" All");
        l_item->setData(Macaw::ObjectId, 0);
        m_ui->listWidget->addItem(l_item);

        if (m_selectedId == 0) {
            l_item->setSelected(true);
        }
    }
    //Create function for the following (code is repeated)
    // ??? Templates ???
    if (m_typeElement == Macaw::isPeople) {
        foreach(Movie l_movie, l_authorizedMoviesList) {
            if( (servicesManager->toWatchState()
                 && databaseManager->isMovieInPlaylist(l_movie.id(), Playlist::ToWatch)
                 ) || !servicesManager->toWatchState()
               ) {
                if(l_movie.peopleList().isEmpty()
                   && !m_elementIdsList.contains(-1)
                  ) {
                    m_elementIdsList.append(-1);
                }
                foreach(People l_people, l_movie.peopleList(m_typePeople)) {
                    if(!m_elementIdsList.contains(l_people.id())) {
                        m_elementIdsList.append(l_people.id());
                    }
                }
            }
        }
    } else if(m_typeElement == Macaw::isTag) {
        foreach(Movie l_movie, l_authorizedMoviesList) {
            if( (servicesManager->toWatchState()
                 && databaseManager->isMovieInPlaylist(l_movie.id(), Playlist::ToWatch)
                 ) || !servicesManager->toWatchState()
               ) {
                if(l_movie.tagList().isEmpty()
                   && !m_elementIdsList.contains(-1)
                  ) {
                    m_elementIdsList.append(-1);
                }
                foreach(Tag l_tag, l_movie.tagList()) {
                    if(!m_elementIdsList.contains(l_tag.id())) {
                        m_elementIdsList.append(l_tag.id());
                    }
                }
            }
        }
    }

    if (m_typeElement == Macaw::isPeople) {
        foreach(int l_objectId, m_elementIdsList) {
            if(l_objectId == -1) {
                QListWidgetItem *l_item = new QListWidgetItem(" Unknown");
                l_item->setData(Macaw::ObjectId, -1);
                l_item->setData(Macaw::ObjectType, Macaw::isPeople);
                m_ui->listWidget->addItem(l_item);
                if (m_selectedId == -1) {
                    l_item->setSelected(true);
                }
            } else {
                People l_people = databaseManager->getOnePeopleById(l_objectId);
                QString l_name(l_people.name());

                QListWidgetItem *l_item = new QListWidgetItem(l_name);
                l_item->setData(Macaw::ObjectId, l_people.id());
                l_item->setData(Macaw::ObjectType, Macaw::isPeople);
                l_item->setData(Macaw::PeopleType, m_typePeople);
                if (m_selectedId == l_people.id())
                {
                    l_item->setSelected(true);
                }

                m_ui->listWidget->addItem(l_item);
            }
        }
    } else if(m_typeElement == Macaw::isTag) {
        foreach(int l_objectId, m_elementIdsList) {
            if(l_objectId == -1) {
                QListWidgetItem *l_item = new QListWidgetItem(" No Tag");
                l_item->setData(Macaw::ObjectId, -1);
                l_item->setData(Macaw::ObjectType, Macaw::isTag);
                m_ui->listWidget->addItem(l_item);
                if (m_selectedId == -1)
                {
                    l_item->setSelected(true);
                }
            } else {
                Tag l_tag = databaseManager->getOneTagById(l_objectId);
                QString l_name(l_tag.name());

                QListWidgetItem *l_item = new QListWidgetItem(l_name);
                l_item->setData(Macaw::ObjectId, l_tag.id());
                l_item->setData(Macaw::ObjectType, Macaw::isTag);
                if (m_selectedId == l_tag.id())
                {
                    l_item->setSelected(true);
                }

                m_ui->listWidget->addItem(l_item);
            }
        }
    }
    if(m_ui->listWidget->selectedItems().isEmpty()) {
        m_ui->listWidget->item(0)->setSelected(true);
    }
    m_ui->listWidget->sortItems();
}

/**
 * @brief Slot triggered when an option from leftPannelBox is selected.
 * Refill all the pannels.
 *
 * @param type of Element (0 = tag, else it's a people, of type `type`)
 */
void LeftPannel::on_leftPannelBox_activated(int type)
{
    Macaw::DEBUG_IN("[LeftPannel] Enters on_leftPannelBox_activated()");

    // 0 = Tags
    // People::typePeople
    int l_peopleType = type;
    if (type==0) {
        m_typeElement = Macaw::isTag;
    } else {
        m_typeElement = Macaw::isPeople;
    }
    m_typePeople = l_peopleType;

    // We change of Macaw::typeElement,
    // so we reinitialise the selection in the leftPannel
    m_selectedId = 0;

    emit updateMainPannel();

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
                connect(l_movieDialog, SIGNAL(destroyed()), this, SLOT(selfUpdate()));
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
