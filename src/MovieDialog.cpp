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

#include "MovieDialog.h"
#include "ui_MovieDialog.h"

/**
 * @brief Constructor
 * @param id of the movie to edit
 * @param parent
 */
MovieDialog::MovieDialog(int id, QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::MovieDialog)
{
    Macaw::DEBUG("[MovieDialog] Constructor called");
    DatabaseManager *databaseManager = DatabaseManager::instance();

    m_movie = databaseManager->getOneMovieById(id);

    m_ui->setupUi(this);
    this->setWindowTitle("Edit Metadata of: " + m_movie.title());
    this->setAttribute(Qt::WA_DeleteOnClose);

    m_ui->tagListWidget->setSelectionMode(QAbstractItemView::NoSelection);

    QList<Tag> l_tagList;
    l_tagList = databaseManager->getAllTags();

    foreach (Tag tag, l_tagList)
    {
        QListWidgetItem * item = new QListWidgetItem(tag.name(), m_ui->tagListWidget);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
        item->setData(Macaw::ObjectId, tag.id());
    }

    setTitle(m_movie.title());
    setOriginalTitle(m_movie.originalTitle());
    setReleaseDate(m_movie.releaseDate());
    setCountry(m_movie.country());
    setSynopsis(m_movie.synopsis());
    setPeopleList(m_movie.peopleList());
    setMovieSelectedTagList(m_movie.tagList());

    m_ui->directorsWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(m_ui->directorsWidget, SIGNAL(customContextMenuRequested(QPoint)),
                     this, SLOT(customMenuRequested(QPoint)));
    m_ui->producersWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(m_ui->producersWidget, SIGNAL(customContextMenuRequested(QPoint)),
                     this, SLOT(customMenuRequested(QPoint)));
    m_ui->actorsWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(m_ui->actorsWidget, SIGNAL(customContextMenuRequested(QPoint)),
                     this, SLOT(customMenuRequested(QPoint)));
    Macaw::DEBUG("[MovieDialog] Construction done");
}

/**
 * @brief Destructor
 */
MovieDialog::~MovieDialog()
{
    delete m_ui;
}

void MovieDialog::setTitle(const QString title)
{
    m_ui->titleEdit->setText(title);
    m_movie.setTitle(title);
}

QString MovieDialog::getTitle() const
{
    return m_ui->titleEdit->text();
}

void MovieDialog::setOriginalTitle(const QString originalTitle)
{
    m_ui->originalTitleEdit->setText(originalTitle);
    m_movie.setOriginalTitle(originalTitle);
}

QString MovieDialog::getOriginalTitle() const
{
    return m_ui->originalTitleEdit->text();
}

void MovieDialog::setReleaseDate(const QDate releaseDate)
{
    m_ui->releaseDateEdit->setSpecialValueText("--/--/----");

    if (releaseDate.isNull() || releaseDate == m_ui->releaseDateEdit->minimumDate())
    {
        // The following line is a hack for setting the specialValueText instead of defaultDate
        m_ui->releaseDateEdit->setDate(QDate::fromString("01/01/0001", "dd/MM/yyyy"));
    }
    else
    {
        m_ui->releaseDateEdit->setDate(releaseDate);
        m_movie.setReleaseDate(releaseDate);
    }
}

QDate MovieDialog::getReleaseDate() const
{
    // if the date is the minimum date, it means it's not the good one
    // so we return a NULL date
    if (m_ui->releaseDateEdit->date() == m_ui->releaseDateEdit->minimumDate())
    {

        return QDate();
    }
    else
    {

        return m_ui->releaseDateEdit->date();
    }
}

void MovieDialog::setCountry(const QString country)
{
    m_ui->countryEdit->setText(country);
    m_movie.setCountry(country);
}

QString MovieDialog::getCountry() const
{
    return m_ui->countryEdit->text();
}

void MovieDialog::setSynopsis(const QString synopsis)
{
    m_ui->synopsisEdit->setPlainText(synopsis);
    m_movie.setSynopsis(synopsis);
}

QString MovieDialog::getSynopsis() const
{
    return m_ui->synopsisEdit->toPlainText();
}

void MovieDialog::setPeopleList(const QList<People> &peopleList)
{
    m_ui->directorsWidget->clear();
    m_ui->actorsWidget->clear();
    m_ui->producersWidget->clear();

    QListWidget *l_peopleWidget;
    foreach(People l_people, peopleList)
    {
        switch (l_people.type())
        {
        case People::Director:
            l_peopleWidget = m_ui->directorsWidget;
            break;
        case People::Producer:
            l_peopleWidget = m_ui->producersWidget;
            break;
        case People::Actor:
            l_peopleWidget = m_ui->actorsWidget;
            break;
        }

        QListWidgetItem *l_item = new QListWidgetItem(l_people.name());
        l_item->setData(Macaw::ObjectId, l_people.id());
        l_item->setData(Macaw::PeopleType, l_people.type());
        l_peopleWidget->addItem(l_item);
    }
}

QList<People> MovieDialog::getPeopleList(int type)
{
    DatabaseManager *databaseManager = DatabaseManager::instance();
    QListWidget *l_peopleWidget = NULL;
    switch (type)
    {
    case People::Director:
        l_peopleWidget = m_ui->directorsWidget;
        break;
    case People::Producer:
        l_peopleWidget = m_ui->producersWidget;
        break;
    case People::Actor:
        l_peopleWidget = m_ui->actorsWidget;
        break;
    }

    QList<People> l_peopleList;

    if (l_peopleWidget != NULL) {
        for (int i = 0 ; i < l_peopleWidget->count() ; i++)
        {
            int l_id = l_peopleWidget->item(i)->data(Macaw::ObjectId).toInt();
            People l_people = databaseManager->getOnePeopleById(l_id, type);
            l_peopleList.push_back(l_people);
        }
    }
    return l_peopleList;
}

/**
 * @brief Set the tag list of selected movie in the MovieDialog as checked
 * @param tag list for the movie
 */
void MovieDialog::setMovieSelectedTagList(const QList<Tag> &tagList)
{
    int l_tagId;

    foreach(const Tag tagToSelect, tagList)
    {
        l_tagId = tagToSelect.id();

        for(int i=0; i < m_ui->tagListWidget->count(); i++)
        {
            if(m_ui->tagListWidget->item(i)->data(Macaw::ObjectId).toInt() == l_tagId)
            {
                m_ui->tagListWidget->item(i)->setCheckState(Qt::Checked);
                break;
            }
        }
    }

}

void MovieDialog::addPeople(const People &people)
{
    Macaw::DEBUG("[MovieDialog] Enters addPeople()");
    QListWidget *l_peopleWidget;

    switch (people.type())
    {
    case People::Director:
        l_peopleWidget = m_ui->directorsWidget;
        break;
    case People::Producer:
        l_peopleWidget = m_ui->producersWidget;
        break;
    case People::Actor:
        l_peopleWidget = m_ui->actorsWidget;
        break;
    }
    QListWidgetItem *l_item = new QListWidgetItem(people.name());
    l_item->setData(Macaw::ObjectId, people.id());
    l_item->setData(Macaw::PeopleType, people.type());
    l_peopleWidget->addItem(l_item);

    m_movie.addPeople(people);
    Macaw::DEBUG("[MovieDialog] Exists addPeople()");
}

void MovieDialog::delPeople(const People &people)
{
    m_movie.removePeople(people);
}

void MovieDialog::updatePeople(const People &people)
{
    Macaw::DEBUG("[MovieDialog] updatePeople()");
    m_movie.updatePeople(people);
    setPeopleList(m_movie.peopleList());
}

/**
 * @brief SIGNAL called when user click on the "OK" button.
 * Saves metadata in the db.
 *
 * No changed are applied before the "OK" is clicked as the user cas cancel modifications.
 *
 *
 */
void MovieDialog::on_validationButtons_accepted()
{
    Macaw::DEBUG("[MovieDialog] validationButtons accepted");

    DatabaseManager *databaseManager = DatabaseManager::instance();
    m_movie.setTitle(getTitle());
    m_movie.setOriginalTitle(getOriginalTitle());
    m_movie.setReleaseDate(getReleaseDate());
    m_movie.setCountry(getCountry());
    m_movie.setSynopsis(getSynopsis());

    QList<QListWidgetItem*> l_selectedItemList;

    for(int i=0; i < m_ui->tagListWidget->count();i++)
    {
        if(m_ui->tagListWidget->item(i)->checkState() == Qt::Checked)
        {
            l_selectedItemList.append(m_ui->tagListWidget->item(i));
        }
    }

    QList<Tag> l_tagList;
    int l_id;
    foreach(const QListWidgetItem *l_item, l_selectedItemList)
    {
        l_id = l_item->data(Macaw::ObjectId).toInt();
        l_tagList.append(databaseManager->getOneTagById(l_id));
    }

    m_movie.setTagList(l_tagList);

    databaseManager->updateMovie(m_movie);
    Macaw::DEBUG("[MovieDialog] validationButtons method done");
}

void MovieDialog::on_addDirectorButton_clicked()
{
    Macaw::DEBUG("[MovieDialog] addDirectorButton clicked()");
    addPeopleButton_clicked(People::Director);
}

void MovieDialog::on_addProducerButton_clicked()
{
    Macaw::DEBUG("[MovieDialog] addProducerButton clicked()");
    addPeopleButton_clicked(People::Producer);
}

void MovieDialog::on_addActorButton_clicked()
{
    Macaw::DEBUG("[MovieDialog] addActorButton clicked()");
    addPeopleButton_clicked(People::Actor);
}

void MovieDialog::addPeopleButton_clicked(int type)
{
    Macaw::DEBUG("[MovieDialog] Enters addPeopleButton_clicked(), type = "+QString::number(type));

    DatabaseManager *databaseManager = DatabaseManager::instance();

    QLineEdit *l_peopleEdit = NULL;
    QListWidget *l_peopleWidget = NULL;
    switch (type)
    {
    case People::Director:
        l_peopleEdit = m_ui->directorEdit;
        l_peopleWidget = m_ui->directorsWidget;
        break;
    case People::Producer:
        l_peopleEdit = m_ui->producerEdit;
        l_peopleWidget = m_ui->producersWidget;
        break;
    case People::Actor:
        l_peopleEdit = m_ui->actorEdit;
        l_peopleWidget = m_ui->actorsWidget;
        break;
    }

    if (l_peopleEdit != NULL
            && l_peopleWidget != NULL) {
        QString l_text = l_peopleEdit->text();

        // To be simply added, the person must exist and not be already in the list
        if (databaseManager->existPeople(l_text)) {
            if(l_peopleWidget->findItems(l_text, Qt::MatchExactly).size() == 0) {
                QList<People> l_peopleList = databaseManager->getPeopleByName(l_text);
                People l_people = l_peopleList.at(0);
                l_people.setType(type);
                addPeople(l_people);
                Macaw::DEBUG("[MovieDialog] " + l_text + " added");
            } else {
                Macaw::DEBUG("[MovieDialog] " + l_text + " already in the list");
            }
        } else {
            People l_people;
            l_people.setName(l_text);
            l_people.setType(type);
            PeopleDialog *l_peopleDialog = new PeopleDialog(l_people);
            l_peopleDialog->show();
            QObject::connect(l_peopleDialog, SIGNAL(peopleCreated(People)),
                             this, SLOT(peopleDialog_peopleCreated(People)));
        }
        l_peopleEdit->clear();
    }
}

void MovieDialog::on_delDirectorButton_clicked()
{
    Macaw::DEBUG("[MovieDialog] delDirectorButton clicked()");
    delPeopleButton_clicked(People::Director);}

void MovieDialog::on_delProducerButton_clicked()
{
    Macaw::DEBUG("[MovieDialog] delProducerButton clicked()");
    delPeopleButton_clicked(People::Producer);

}

void MovieDialog::on_delActorButton_clicked()
{
    Macaw::DEBUG("[MovieDialog] delActorButton clicked()");
    delPeopleButton_clicked(People::Actor);
}

void MovieDialog::delPeopleButton_clicked(int type)
{
    Macaw::DEBUG("[MovieDialog] Enters delPeopleButton_clicked()");

    DatabaseManager *databaseManager = DatabaseManager::instance();
    QListWidget *l_peopleWidget = NULL;
    switch (type)
    {
    case People::Director:
        l_peopleWidget = m_ui->directorsWidget;
        break;
    case People::Producer:
        l_peopleWidget = m_ui->producersWidget;
        break;
    case People::Actor:
        l_peopleWidget = m_ui->actorsWidget;
        break;
    }

    if (l_peopleWidget != NULL) {
    QList<QListWidgetItem*> l_itemsListToDelete = l_peopleWidget->selectedItems();
        foreach (QListWidgetItem *l_itemToDelete, l_itemsListToDelete)
        {
            int l_peopleId = l_itemToDelete->data(Macaw::ObjectId).toInt();
            People l_people = databaseManager->getOnePeopleById(l_peopleId, type);
            l_people.setType(type);
            delPeople(l_people);
            delete(l_itemToDelete);
        }
    }
    Macaw::DEBUG("[MovieDialog] Exits delPeopleButton_clicked()");
}

void MovieDialog::on_directorEdit_textEdited()
{
    Macaw::DEBUG("[MovieDialog] directorEdit textEdited()");
    on_peopleEdit_textEdited(People::Director);
}

void MovieDialog::on_producerEdit_textEdited()
{
    Macaw::DEBUG("[MovieDialog] producerEdit textEdited()");
    on_peopleEdit_textEdited(People::Producer);
}

void MovieDialog::on_actorEdit_textEdited()
{
    Macaw::DEBUG("[MovieDialog] actorEdit textEdited()");
    on_peopleEdit_textEdited(People::Actor);
}

void MovieDialog::on_peopleEdit_textEdited(int type)
{
    Macaw::DEBUG("[MovieDialog] Enters on_peopleEdit_textEdited()");
    DatabaseManager *databaseManager = DatabaseManager::instance();

    QLineEdit *l_peopleEdit = NULL;
    switch (type)
    {
    case People::Director:
        l_peopleEdit = m_ui->directorEdit;
        break;
    case People::Producer:
        l_peopleEdit = m_ui->producerEdit;
        break;
    case People::Actor:
        l_peopleEdit = m_ui->actorEdit;
        break;
    }

    QString l_text =  l_peopleEdit->text();
    if (l_text.size() > 3)
    {
        QList<People> l_peopleList = databaseManager->getPeopleByName(l_text);
        if(l_peopleList.size() > 0)
        {
            People l_people;
            QStringList l_propositions;
            foreach (l_people, l_peopleList)
            {
                l_propositions << l_people.name();
            }
            QCompleter *l_completer = new QCompleter(l_propositions);
            l_completer->setCaseSensitivity(Qt::CaseInsensitive);
            l_completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
            l_peopleEdit->setCompleter(l_completer);
        }
    }
    Macaw::DEBUG("[MovieDialog] Exits on_peopleEdit_textEdited()");
}

/**
 * @brief when NewTagButton is clicked
 * Adds the tag in newTagLineEdit field to the tag list and clear the lineEdit, selects the tag
 * @param QPoint position of the cursor
 */
void MovieDialog::on_addNewTagButton_clicked()
{
    DatabaseManager *databaseManager = DatabaseManager::instance();
    QString newTag =  m_ui->newTagLineEdit->text();

    if (newTag != "" && newTag != " ") {
        int newTagId = databaseManager->createTag(newTag);

        if(newTagId > 0)  {
            QListWidgetItem * item = new QListWidgetItem(newTag, m_ui->tagListWidget);
            item->setData(Macaw::ObjectId, newTagId);
            item->setCheckState(Qt::Checked);
        }

        m_ui->newTagLineEdit->clear();
    }
}

void MovieDialog::peopleDialog_peopleCreated(People people)
{
    Macaw::DEBUG("[MovieDialog] peopleDialog_peopleCreated()");
    if (people.id() != 0) {
        updatePeople(people);
    }
    else
    {
        addPeople(people);
    }
}

/**
 * @brief Shows the context-menu where the user rightclicks
 *
 * @param QPoint position of the cursor
 */
void MovieDialog::customMenuRequested(QPoint pos)
{
    Macaw::DEBUG("[MovieDialog] Enters customMenuRequested()");
    QListWidget *l_widget = getFocusedListWidget();

    // If nothing selected, don't do anything
    if (!l_widget->selectedItems().isEmpty())
    {
        QMenu *l_menu = new QMenu(this);
        QAction *l_setMetadataAction = new QAction("Update person", this);
        QObject::connect(l_setMetadataAction, SIGNAL(triggered()), this, SLOT(showPeopleDialog()));
        l_menu->addAction(l_setMetadataAction);
        l_menu->popup(l_widget->viewport()->mapToGlobal(pos));
    }
    Macaw::DEBUG("[MovieDialog] Exits customMenuRequested()");
}

/**
 * @brief Shows the window to view/edit the metadata of a people
 */
void MovieDialog::showPeopleDialog()
{
    Macaw::DEBUG("[MovieDialog] Enters showPeopleDialog()");
    QListWidget *l_widget = getFocusedListWidget();
    QListWidgetItem *l_selectedItem = l_widget->selectedItems().at(0);
    int l_itemRow = l_widget->row(l_selectedItem);
    People l_selectedPeople = this->getFocusedListPeople().at(l_itemRow);
    PeopleDialog *l_peopleDialog = new PeopleDialog(l_selectedPeople);

    // Because we cannot update a people without id, we recreate it.
    if(l_selectedPeople.id() == 0)
    {
        delPeople(l_selectedPeople);
        delete(l_selectedItem);
    }
    l_peopleDialog->show();
    QObject::connect(l_peopleDialog, SIGNAL(peopleCreated(People)),
                     this, SLOT(peopleDialog_peopleCreated(People)));
    Macaw::DEBUG("[MovieDialog] Exits showPeopleDialog()");
}

/**
 * @brief Returns the QListWidget that has focus
 *
 * @return QListWidget*
 */
QListWidget* MovieDialog::getFocusedListWidget()
{
    QListWidget *l_widget = NULL;
    if (m_ui->directorsWidget->hasFocus())
    {
        l_widget = m_ui->directorsWidget;
    }
    else if (m_ui->producersWidget->hasFocus())
    {
        l_widget = m_ui->producersWidget;
    }
    else if (m_ui->actorsWidget->hasFocus())
    {
        l_widget = m_ui->actorsWidget;
    }

    return l_widget;
}

/**
 * @brief Returns the QList<People> that has focus
 *
 * @return QList<People>
 */
QList<People> MovieDialog::getFocusedListPeople()
{
    QList<People> l_people;
    if (m_ui->directorsWidget->hasFocus())
    {
        l_people = m_movie.peopleList(People::Director);
    }
    else if (m_ui->producersWidget->hasFocus())
    {
        l_people = m_movie.peopleList(People::Producer);
    }
    else if (m_ui->actorsWidget->hasFocus())
    {
        l_people = m_movie.peopleList(People::Actor);
    }

    return l_people;
}

/**
 * @brief Slot triggered when the ResetReleaseDate Button is clicked
 * Set the date to a date < date_min to print the specialValueText
 */
void MovieDialog::on_resetReleaseDateBtn_clicked()
{
    m_ui->releaseDateEdit->setDate(QDate::fromString("01/01/0001", "dd/MM/yyyy"));
}
