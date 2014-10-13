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

#include "MetadataWindow.h"
#include "ui_MetadataWindow.h"

/* @TODO:
 *   - While typing in a peopleEdit, propose existing names. If none: pop-up "Do you want to create ?"
 *   - Add actions to + and - buttons
 *   - Right-click on a ListWidgetItem allow to edit it the people
 *   - Handle tags
 */

MetadataWindow::MetadataWindow(int id, QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::MetadataWindow)
{
    m_app = qobject_cast<Application *>(qApp);
    m_app->debug("[MetadataWindow] Constructor called");
    m_movie = m_app->getDatabaseManager()->getOneMovieById(id);
    m_ui->setupUi(this);
    setTitle(m_movie.getTitle());
    setOriginalTitle(m_movie.getOriginalTitle());
    setReleaseDate(m_movie.getReleaseDate());
    setCountry(m_movie.getCountry());
    setSynopsis(m_movie.getSynopsis());
    setDirectors(m_movie.getDirectors());
    setActors(m_movie.getActors());
    setProducers(m_movie.getProducers());

    m_ui->directorsWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(m_ui->directorsWidget, SIGNAL(customContextMenuRequested(QPoint)), SLOT(customMenuRequested(QPoint)));
    m_ui->producersWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(m_ui->producersWidget, SIGNAL(customContextMenuRequested(QPoint)), SLOT(customMenuRequested(QPoint)));
    m_ui->actorsWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(m_ui->actorsWidget, SIGNAL(customContextMenuRequested(QPoint)), SLOT(customMenuRequested(QPoint)));
    m_app->debug("[MetadataWindow] Construction done");
}

MetadataWindow::~MetadataWindow()
{
    delete m_ui;
}

void MetadataWindow::setTitle(QString title)
{
    m_ui->titleEdit->setText(title);
    m_movie.setTitle(title);
}

QString MetadataWindow::getTitle()
{
    return m_ui->titleEdit->text();
}

void MetadataWindow::setOriginalTitle(QString originalTitle)
{
    m_ui->originalTitleEdit->setText(originalTitle);
    m_movie.setOriginalTitle(originalTitle);
}

QString MetadataWindow::getOriginalTitle()
{
    return m_ui->originalTitleEdit->text();
}

void MetadataWindow::setReleaseDate(QDate releaseDate)
{
    m_ui->releaseDateEdit->setDate(releaseDate);
    m_movie.setReleaseDate(releaseDate);
}

QDate MetadataWindow::getReleaseDate()
{
    return m_ui->releaseDateEdit->date();
}

void MetadataWindow::setCountry(QString country)
{
    m_ui->countryEdit->setText(country);
    m_movie.setCountry(country);
}

QString MetadataWindow::getCountry()
{
    return m_ui->countryEdit->text();
}

void MetadataWindow::setSynopsis(QString synopsis)
{
    m_ui->synopsisEdit->setPlainText(synopsis);
    m_movie.setSynopsis(synopsis);
}

QString MetadataWindow::getSynopsis()
{
    return m_ui->synopsisEdit->toPlainText();
}

void MetadataWindow::setPeople(QVector<People> peopleVector, int type)
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
    l_peopleWidget->clear();
    foreach(People l_people, peopleVector)
    {
        QListWidgetItem *l_item = new QListWidgetItem(l_people.getFirstname() + " " + l_people.getLastname());
        l_item->setData(Qt::UserRole, l_people.getId());
        l_peopleWidget->addItem(l_item);
    }
}

QVector<People> MetadataWindow::getPeople(int type)
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

void MetadataWindow::addPeople(People &people, int type)
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
    QListWidgetItem *l_item = new QListWidgetItem(people.getFirstname() + " " + people.getLastname());
    l_item->setData(Qt::UserRole, people.getId());
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

void MetadataWindow::delPeople(People &people, int type)
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

void MetadataWindow::updatePeople(People &people)
{
    m_movie.updateDirector(people);
    m_movie.updateProducer(people);
    m_movie.updateActor(people);
    setDirectors(m_movie.getDirectors());
    setProducers(m_movie.getProducers());
    setActors(m_movie.getActors());
}

void MetadataWindow::setDirectors(QVector<People> directorsVector)
{
    setPeople(directorsVector, Director);
}

QVector<People> MetadataWindow::getDirectors()
{
    return getPeople(Director);
}

void MetadataWindow::addDirector(People &director)
{
    addPeople(director, Director);
}

void MetadataWindow::delDirector(People &director)
{
    delPeople(director, Director);
}

void MetadataWindow::setProducers(QVector<People> producersVector)
{
    setPeople(producersVector, Producer);
}

QVector<People> MetadataWindow::getProducers()
{
    return getPeople(Producer);
}

void MetadataWindow::addProducer(People &producer)
{
    addPeople(producer, Producer);
}

void MetadataWindow::delProducer(People &producer)
{
    delPeople(producer, Producer);
}

void MetadataWindow::setActors(QVector<People> actorsVector)
{
    setPeople(actorsVector, Actor);
}

QVector<People> MetadataWindow::getActors()
{
    return getPeople(Actor);
}

void MetadataWindow::addActor(People &actor)
{
    addPeople(actor, Actor);
}

void MetadataWindow::delActor(People &actor)
{
    delPeople(actor, Actor);
}

void MetadataWindow::on_validationButtons_accepted()
{
    m_app->debug("[MetadataWindow] validationButtons accepted");
    m_app->getDatabaseManager()->updateMovie(m_movie);
    m_app->debug("[MetadataWindow] validationButtons method done");
}

void MetadataWindow::on_addDirectorButton_clicked()
{
    m_app->debug("[MetadataWindow] addDirectorButton clicked()");
    addPeopleButton_clicked(Director);
}

void MetadataWindow::on_addProducerButton_clicked()
{
    m_app->debug("[MetadataWindow] addProducerButton clicked()");
    addPeopleButton_clicked(Producer);
}

void MetadataWindow::on_addActorButton_clicked()
{
    m_app->debug("[MetadataWindow] addActorButton clicked()");
    addPeopleButton_clicked(Actor);
}

void MetadataWindow::addPeopleButton_clicked(int type)
{
    m_app->debug("[MetadataWindow] Enters addPeopleButton_clicked()");

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
            m_app->debug("[MetadataWindow] " + l_text + " added");
        }
        else
        {
            m_app->debug("[MetadataWindow] " + l_text + " already in the list");
        }
    }
    else
    {
        PeopleWindow *l_peopleWindow = new PeopleWindow(type);
        // We suppose here that a name is composed by N >= 0 firstnames
        // and 1 lastname, separated by spaces
        QStringList l_textExplosed = l_text.split(" ");
        QString l_lastname = l_textExplosed.last();
        l_textExplosed.removeLast();
        QString l_firstname = l_textExplosed.join(" ");
        l_peopleWindow->setFirstname(l_firstname);
        l_peopleWindow->setLastname(l_lastname);
        l_peopleWindow->show();
        QObject::connect(l_peopleWindow, SIGNAL(peopleCreated(People, int)), this, SLOT(peopleWindow_peopleCreated(People, int)));
    }
}

void MetadataWindow::on_delDirectorButton_clicked()
{
    m_app->debug("[MetadataWindow] delDirectorButton clicked()");
    delPeopleButton_clicked(Director);
}

void MetadataWindow::on_delProducerButton_clicked()
{
    m_app->debug("[MetadataWindow] delProducerButton clicked()");
}

void MetadataWindow::on_delActorButton_clicked()
{
    m_app->debug("[MetadataWindow] delActorButton clicked()");
}

void MetadataWindow::delPeopleButton_clicked(int type)
{
    m_app->debug("[MetadataWindow] Enters delPeopleButton_clicked()");

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
    m_app->debug("[MetadataWindow] Exits delPeopleButton_clicked()");
}

void MetadataWindow::on_directorEdit_textEdited()
{
    m_app->debug("[MetadataWindow] directorEdit textEdited()");
    on_peopleEdit_textEdited(Director);
}

void MetadataWindow::on_producerEdit_textEdited()
{
    m_app->debug("[MetadataWindow] producerEdit textEdited()");
    on_peopleEdit_textEdited(Producer);
}

void MetadataWindow::on_actorEdit_textEdited()
{
    m_app->debug("[MetadataWindow] actorEdit textEdited()");
    on_peopleEdit_textEdited(Actor);
}

void MetadataWindow::on_peopleEdit_textEdited(int type)
{
    m_app->debug("[MetadataWindow] Enters on_peopleEdit_textEdited()");

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
    m_app->debug("[MetadataWindow] Exits on_peopleEdit_textEdited()");
}

void MetadataWindow::peopleWindow_peopleCreated(People people, int type)
{
    if(people.getId() == 0)
    {
        addPeople(people, type);
    }
    else
    {
        updatePeople(people);
    }
}

/**
 * @brief Shows the context-menu where the user rightclicks
 *
 * @param QPoint position of the cursor
 */
void MetadataWindow::customMenuRequested(QPoint pos)
{
    m_app->debug("[MetadataWindow] Enters customMenuRequested()");
    QListWidget *l_widget = getFocusedListWidget();
    QMenu *l_menu = new QMenu(this);
    QAction *l_setMetadataAction = new QAction("Update person", this);
    QObject::connect(l_setMetadataAction, SIGNAL(triggered()), this, SLOT(showPeopleWindow()));
    l_menu->addAction(l_setMetadataAction);
    l_menu->popup(l_widget->viewport()->mapToGlobal(pos));
    m_app->debug("[MetadataWindow] Exits customMenuRequested()");
}

/**
 * @brief Shows the window to view/edit the metadata of a people
 */
void MetadataWindow::showPeopleWindow()
{
    m_app->debug("[MetadataWindow] Enters showPeopleWindow()");
    QListWidget *l_widget = getFocusedListWidget();
    int l_peopleId = l_widget->selectedItems().at(0)->data(Qt::UserRole).toInt();
    PeopleWindow *l_peopleWindow = new PeopleWindow(l_peopleId);
    l_peopleWindow->show();
    QObject::connect(l_peopleWindow, SIGNAL(peopleCreated(People, int)), this, SLOT(peopleWindow_peopleCreated(People, int)));
    m_app->debug("[MetadataWindow] Exits showPeopleWindow()");
}

/**
 * @brief Returns the QListWidget that has focus
 *
 * @return QListWidget*
 */
QListWidget* MetadataWindow::getFocusedListWidget()
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
