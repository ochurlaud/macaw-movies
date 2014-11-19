/* Copyright (C) 2014 Movie-Project
 * (Olivier CHURLAUD, Sébastien TOUZÉ)
 *
 * This file is part of Movie-Project.
 *
 * Movie-Project is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Movie-Project is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Movie-Project.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "MovieDialog.h"
#include "ui_MovieDialog.h"

/* @TODO:
 *   - Handle tags
 */

MovieDialog::MovieDialog(int id, QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::MovieDialog)
{
    m_app = qobject_cast<Application *>(qApp);
    m_app->debug("[MovieDialog] Constructor called");
    m_movie = m_app->getDatabaseManager()->getOneMovieById(id);

    m_ui->setupUi(this);
    this->setWindowTitle("Edit Metadata of: " + m_movie.getTitle());
    this->setAttribute(Qt::WA_DeleteOnClose);

    m_ui->tagListView->setModel(m_app->getDatabaseManager()->getTagListModel());

    setTitle(m_movie.getTitle());
    setOriginalTitle(m_movie.getOriginalTitle());
    setReleaseDate(m_movie.getReleaseDate());
    setCountry(m_movie.getCountry());
    setSynopsis(m_movie.getSynopsis());
    setDirectors(m_movie.getDirectors());
    setActors(m_movie.getActors());
    setProducers(m_movie.getProducers());

    m_ui->directorsWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(m_ui->directorsWidget, SIGNAL(customContextMenuRequested(QPoint)),
                     this, SLOT(customMenuRequested(QPoint)));
    m_ui->producersWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(m_ui->producersWidget, SIGNAL(customContextMenuRequested(QPoint)),
                     this, SLOT(customMenuRequested(QPoint)));
    m_ui->actorsWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(m_ui->actorsWidget, SIGNAL(customContextMenuRequested(QPoint)),
                     this, SLOT(customMenuRequested(QPoint)));
    m_app->debug("[MovieDialog] Construction done");
}

MovieDialog::~MovieDialog()
{
    delete m_ui;
}

void MovieDialog::setTitle(QString title)
{
    m_ui->titleEdit->setText(title);
    m_movie.setTitle(title);
}

QString MovieDialog::getTitle()
{
    return m_ui->titleEdit->text();
}

void MovieDialog::setOriginalTitle(QString originalTitle)
{
    m_ui->originalTitleEdit->setText(originalTitle);
    m_movie.setOriginalTitle(originalTitle);
}

QString MovieDialog::getOriginalTitle()
{
    return m_ui->originalTitleEdit->text();
}

void MovieDialog::setReleaseDate(QDate releaseDate)
{
    m_ui->releaseDateEdit->setDate(releaseDate);
    m_movie.setReleaseDate(releaseDate);
}

QDate MovieDialog::getReleaseDate()
{
    return m_ui->releaseDateEdit->date();
}

void MovieDialog::setCountry(QString country)
{
    m_ui->countryEdit->setText(country);
    m_movie.setCountry(country);
}

QString MovieDialog::getCountry()
{
    return m_ui->countryEdit->text();
}

void MovieDialog::setSynopsis(QString synopsis)
{
    m_ui->synopsisEdit->setPlainText(synopsis);
    m_movie.setSynopsis(synopsis);
}

QString MovieDialog::getSynopsis()
{
    return m_ui->synopsisEdit->toPlainText();
}

void MovieDialog::setPeople(QVector<People> peopleVector, int type)
{
    QListWidget *l_peopleWidget;
    int l_type;
    switch (type)
    {
    case Director:
        l_peopleWidget = m_ui->directorsWidget;
        l_type = Director;
        break;
    case Producer:
        l_peopleWidget = m_ui->producersWidget;
        l_type = Producer;
        break;
    case Actor:
        l_peopleWidget = m_ui->actorsWidget;
        l_type = Actor;
        break;
    }
    l_peopleWidget->clear();
    foreach(People l_people, peopleVector)
    {
        QListWidgetItem *l_item = new QListWidgetItem(l_people.getFirstname() + " " + l_people.getLastname());
        l_item->setData(Qt::UserRole, l_people.getId());
        l_item->setData(Qt::UserRole+1, l_type);
        l_peopleWidget->addItem(l_item);
    }
}

QVector<People> MovieDialog::getPeople(int type)
{
    QListWidget *l_peopleWidget;
    switch (type)
    {
    case Director:
        l_peopleWidget = m_ui->directorsWidget;
        break;
    case Producer:
        l_peopleWidget = m_ui->producersWidget;
        break;
    case Actor:
        l_peopleWidget = m_ui->actorsWidget;
        break;
    }
    QVector<People> l_peopleVector;
    for (int i = 0 ; i < l_peopleWidget->count() ; i++)
    {
        int l_id = l_peopleWidget->item(i)->data(Qt::UserRole).toInt();
        People l_people = m_app->getDatabaseManager()->getOnePeopleById(l_id, type);
        l_peopleVector.push_back(l_people);
    }

    return l_peopleVector;
}

void MovieDialog::addPeople(People &people, int type)
{
    m_app->debug("[MovieDialog] addPeople()");
    QListWidget *l_peopleWidget;
    int l_type;
    switch (type)
    {
    case Director:
        l_peopleWidget = m_ui->directorsWidget;
        l_type = Director;
        break;
    case Producer:
        l_peopleWidget = m_ui->producersWidget;
        l_type = Producer;
        break;
    case Actor:
        l_peopleWidget = m_ui->actorsWidget;
        l_type = Actor;
        break;
    }
    QListWidgetItem *l_item = new QListWidgetItem(people.getFirstname() + " " + people.getLastname());
    l_item->setData(Qt::UserRole, people.getId());
    l_item->setData(Qt::UserRole+1, l_type);
    l_peopleWidget->addItem(l_item);

    // If we could do an alias of the function above, it would be prettier I think
    switch (type)
    {
    case Director:
        m_movie.addDirector(people);
        break;
    case Producer:
        m_movie.addProducer(people);
        break;
    case Actor:
        m_movie.addActor(people);
        break;
    }
}

void MovieDialog::delPeople(People &people, int type)
{
    switch (type)
    {
    case Director:
        m_movie.removeDirector(people);
        break;
    case Producer:
        m_movie.removeProducer(people);
        break;
    case Actor:
        m_movie.removeActor(people);
        break;
    }
}

bool MovieDialog::updatePeople(People &people)
{
    m_app->debug("[MovieDialog] updatePeople()");
    bool l_result;
    l_result = m_movie.updateDirector(people);
    l_result = l_result || m_movie.updateProducer(people);
    l_result = l_result || m_movie.updateActor(people);
    setDirectors(m_movie.getDirectors());
    setProducers(m_movie.getProducers());
    setActors(m_movie.getActors());

    return l_result;
}

void MovieDialog::setDirectors(QVector<People> directorsVector)
{
    setPeople(directorsVector, Director);
}

QVector<People> MovieDialog::getDirectors()
{
    return getPeople(Director);
}

void MovieDialog::addDirector(People &director)
{
    addPeople(director, Director);
}

void MovieDialog::delDirector(People &director)
{
    delPeople(director, Director);
}

void MovieDialog::setProducers(QVector<People> producersVector)
{
    setPeople(producersVector, Producer);
}

QVector<People> MovieDialog::getProducers()
{
    return getPeople(Producer);
}

void MovieDialog::addProducer(People &producer)
{
    addPeople(producer, Producer);
}

void MovieDialog::delProducer(People &producer)
{
    delPeople(producer, Producer);
}

void MovieDialog::setActors(QVector<People> actorsVector)
{
    setPeople(actorsVector, Actor);
}

QVector<People> MovieDialog::getActors()
{
    return getPeople(Actor);
}

void MovieDialog::addActor(People &actor)
{
    addPeople(actor, Actor);
}

void MovieDialog::delActor(People &actor)
{
    delPeople(actor, Actor);
}

void MovieDialog::on_validationButtons_accepted()
{
    m_app->debug("[MovieDialog] validationButtons accepted");
    m_movie.setTitle(getTitle());
    m_movie.setOriginalTitle(getOriginalTitle());
    m_movie.setReleaseDate(getReleaseDate());
    m_movie.setCountry(getCountry());
    m_movie.setSynopsis(getSynopsis());

    m_app->getDatabaseManager()->updateMovie(m_movie);
    m_app->debug("[MovieDialog] validationButtons method done");
}

void MovieDialog::on_addDirectorButton_clicked()
{
    m_app->debug("[MovieDialog] addDirectorButton clicked()");
    addPeopleButton_clicked(Director);
}

void MovieDialog::on_addProducerButton_clicked()
{
    m_app->debug("[MovieDialog] addProducerButton clicked()");
    addPeopleButton_clicked(Producer);
}

void MovieDialog::on_addActorButton_clicked()
{
    m_app->debug("[MovieDialog] addActorButton clicked()");
    addPeopleButton_clicked(Actor);
}

void MovieDialog::addPeopleButton_clicked(int type)
{
    m_app->debug("[MovieDialog] Enters addPeopleButton_clicked(), type = "+QString::number(type));

    QLineEdit *l_peopleEdit;
    QListWidget *l_peopleWidget;
    switch (type)
    {
    case Director:
        l_peopleEdit = m_ui->directorEdit;
        l_peopleWidget = m_ui->directorsWidget;
        break;
    case Producer:
        l_peopleEdit = m_ui->producerEdit;
        l_peopleWidget = m_ui->producersWidget;
        break;
    case Actor:
        l_peopleEdit = m_ui->actorEdit;
        l_peopleWidget = m_ui->actorsWidget;
        break;
    }

    QString l_text = l_peopleEdit->text();

    // To be simply added, the person must exist and not be already in the list
    if (m_app->getDatabaseManager()->existPeople(l_text))
    {
        if(l_peopleWidget->findItems(l_text, Qt::MatchExactly).size() == 0)
        {
            QVector<People> l_peopleVector = m_app->getDatabaseManager()->getPeopleByFullname(l_text);
            People l_people = l_peopleVector.at(0);
            addPeople(l_people, type);
            m_app->debug("[MovieDialog] " + l_text + " added");
        }
        else
        {
            m_app->debug("[MovieDialog] " + l_text + " already in the list");
        }
    }
    else
    {
        PeopleDialog *l_peopleDialog = new PeopleDialog(None, type);
        // We suppose here that a name is composed by N >= 0 firstnames
        // and 1 lastname, separated by spaces
        QStringList l_textExplosed = l_text.split(" ");
        QString l_lastname = l_textExplosed.last();
        l_textExplosed.removeLast();
        QString l_firstname = l_textExplosed.join(" ");
        l_peopleDialog->setFirstname(l_firstname);
        l_peopleDialog->setLastname(l_lastname);
        l_peopleDialog->show();
        QObject::connect(l_peopleDialog, SIGNAL(peopleCreated(People, int)),
                         this, SLOT(peopleDialog_peopleCreated(People, int)));
    }
}

void MovieDialog::on_delDirectorButton_clicked()
{
    m_app->debug("[MovieDialog] delDirectorButton clicked()");
    delPeopleButton_clicked(Director);
}

void MovieDialog::on_delProducerButton_clicked()
{
    m_app->debug("[MovieDialog] delProducerButton clicked()");
}

void MovieDialog::on_delActorButton_clicked()
{
    m_app->debug("[MovieDialog] delActorButton clicked()");
}

void MovieDialog::delPeopleButton_clicked(int type)
{
    m_app->debug("[MovieDialog] Enters delPeopleButton_clicked()");

    QListWidget *l_peopleWidget;
    switch (type)
    {
    case Director:
        l_peopleWidget = m_ui->directorsWidget;
        break;
    case Producer:
        l_peopleWidget = m_ui->producersWidget;
        break;
    case Actor:
        l_peopleWidget = m_ui->actorsWidget;
        break;
    }

    QList<QListWidgetItem*> l_itemsListToDelete = l_peopleWidget->selectedItems();
    foreach (QListWidgetItem *l_itemToDelete, l_itemsListToDelete)
    {
        int l_peopleId = l_itemToDelete->data(Qt::UserRole).toInt();
        People l_people = m_app->getDatabaseManager()->getOnePeopleById(l_peopleId, type);
        delPeople(l_people, type);
        delete(l_itemToDelete);
    }
    m_app->debug("[MovieDialog] Exits delPeopleButton_clicked()");
}

void MovieDialog::on_directorEdit_textEdited()
{
    m_app->debug("[MovieDialog] directorEdit textEdited()");
    on_peopleEdit_textEdited(Director);
}

void MovieDialog::on_producerEdit_textEdited()
{
    m_app->debug("[MovieDialog] producerEdit textEdited()");
    on_peopleEdit_textEdited(Producer);
}

void MovieDialog::on_actorEdit_textEdited()
{
    m_app->debug("[MovieDialog] actorEdit textEdited()");
    on_peopleEdit_textEdited(Actor);
}

void MovieDialog::on_peopleEdit_textEdited(int type)
{
    m_app->debug("[MovieDialog] Enters on_peopleEdit_textEdited()");

    QLineEdit *l_peopleEdit;
    switch (type)
    {
    case Director:
        l_peopleEdit = m_ui->directorEdit;
        break;
    case Producer:
        l_peopleEdit = m_ui->producerEdit;
        break;
    case Actor:
        l_peopleEdit = m_ui->actorEdit;
        break;
    }

    QString l_text =  l_peopleEdit->text();
    if (l_text.size() > 3)
    {
        QVector<People> l_peopleVector = m_app->getDatabaseManager()->getPeopleByFullname(l_text);
        if(l_peopleVector.size() > 0)
        {
            People l_people;
            QStringList l_propositions;
            foreach (l_people, l_peopleVector)
            {
                l_propositions << l_people.getFirstname() + " " + l_people.getLastname();
            }
            QCompleter *l_completer = new QCompleter(l_propositions);
            l_completer->setCaseSensitivity(Qt::CaseInsensitive);
            l_completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
            l_peopleEdit->setCompleter(l_completer);
        }
    }
    m_app->debug("[MovieDialog] Exits on_peopleEdit_textEdited()");
}

/**
 * @brief add the tag in newTagLineEdit field to the tag list
 *
 * @param QPoint position of the cursor
 */
void MovieDialog::on_addNewTagButton_clicked()
{
    QString newTag =  m_ui->newTagLineEdit->text();

    m_app->getDatabaseManager()->createTag(newTag);

    m_ui->newTagLineEdit->clear();

}

void MovieDialog::peopleDialog_peopleCreated(People people, int type)
{
    m_app->debug("[MovieDialog] peopleDialog_peopleCreated()");
    if (people.getId() != 0)
    {
        updatePeople(people);
    }
    else
    {
        addPeople(people, type);
    }
}

/**
 * @brief Shows the context-menu where the user rightclicks
 *
 * @param QPoint position of the cursor
 */
void MovieDialog::customMenuRequested(QPoint pos)
{
    m_app->debug("[MovieDialog] Enters customMenuRequested()");
    QListWidget *l_widget = getFocusedListWidget();

    // If nothing selected, don't do anything
    if (l_widget->selectedItems().count() != 0)
    {
        QMenu *l_menu = new QMenu(this);
        QAction *l_setMetadataAction = new QAction("Update person", this);
        QObject::connect(l_setMetadataAction, SIGNAL(triggered()), this, SLOT(showPeopleDialog()));
        l_menu->addAction(l_setMetadataAction);
        l_menu->popup(l_widget->viewport()->mapToGlobal(pos));
    }
    m_app->debug("[MovieDialog] Exits customMenuRequested()");
}

/**
 * @brief Shows the window to view/edit the metadata of a people
 */
void MovieDialog::showPeopleDialog()
{
    m_app->debug("[MovieDialog] Enters showPeopleDialog()");
    QListWidget *l_widget = getFocusedListWidget();
    QListWidgetItem *l_selectedItem = l_widget->selectedItems().at(0);
    int l_itemRow = l_widget->row(l_selectedItem);
    int l_type = l_widget->selectedItems().at(0)->data(Qt::UserRole + 1).toInt();
    People l_selectedPeople = this->getFocusedListPeople().at(l_itemRow);
    PeopleDialog *l_peopleDialog = new PeopleDialog(l_selectedPeople, l_type);

    // Because we cannot update a people without id, we recreate it.
    if(l_selectedPeople.getId() == 0)
    {
        delPeople(l_selectedPeople, l_type);
        delete(l_selectedItem);
    }
    l_peopleDialog->show();
    QObject::connect(l_peopleDialog, SIGNAL(peopleCreated(People, int)),
                     this, SLOT(peopleDialog_peopleCreated(People, int)));
    m_app->debug("[MovieDialog] Exits showPeopleDialog()");
}

/**
 * @brief Returns the QListWidget that has focus
 *
 * @return QListWidget*
 */
QListWidget* MovieDialog::getFocusedListWidget()
{
    QListWidget *l_widget;
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
QVector<People> MovieDialog::getFocusedListPeople()
{
    QVector<People> l_people;
    if (m_ui->directorsWidget->hasFocus())
    {
        l_people = m_movie.getDirectors();
    }
    else if (m_ui->producersWidget->hasFocus())
    {
        l_people = m_movie.getProducers();
    }
    else if (m_ui->actorsWidget->hasFocus())
    {
        l_people = m_movie.getActors();
    }

    return l_people;
}
