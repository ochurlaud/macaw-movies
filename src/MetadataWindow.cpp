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
    connect(m_ui->directorsWidget, SIGNAL(customContextMenuRequested(QPoint)), SLOT(customMenuRequested(QPoint)));
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

void MetadataWindow::setDirectors(QVector<People> directorsVector)
{
    People l_director;
    foreach(l_director, directorsVector)
    {
        QListWidgetItem *l_item = new QListWidgetItem(l_director.getFirstname() + " " + l_director.getLastname());
        l_item->setData(Qt::UserRole, l_director.getId());
        m_ui->directorsWidget->addItem(l_item);
    }
}

QVector<People> MetadataWindow::getDirectors()
{
    QVector<People> l_directorsVector;
    for (int i = 0 ; i < m_ui->directorsWidget->count() ; i++)
    {
        int l_id = m_ui->directorsWidget->item(i)->data(Qt::UserRole).toInt();
        People l_director = m_app->getDatabaseManager()->getOneDirectorById(l_id);
        l_directorsVector.push_back(l_director);
    }

    return l_directorsVector;
}

void MetadataWindow::addDirector(People director)
{
    QListWidgetItem *l_item = new QListWidgetItem(director.getFirstname() + " " + director.getLastname());
    l_item->setData(Qt::UserRole, director.getId());
    m_ui->directorsWidget->addItem(l_item);
    m_movie.addDirector(director);
}

void MetadataWindow::delDirector(People director)
{
    m_movie.removeDirector(director);
}

void MetadataWindow::setProducers(QVector<People> producersVector)
{
    for (int i = 0 ; i < producersVector.size() ; i++)
    {
        People l_producer = producersVector.at(i);
        QListWidgetItem *l_item = new QListWidgetItem(l_producer.getFirstname() + " " + l_producer.getLastname());
        l_item->setData(Qt::UserRole, l_producer.getId());
        m_ui->producersWidget->insertItem(i, l_item);
    }
}

QVector<People> MetadataWindow::getProducers()
{
    QVector<People> l_producersVector;
    for (int i = 0 ; i < m_ui->producersWidget->count() ; i++)
    {
        int l_id = m_ui->producersWidget->item(i)->data(Qt::UserRole).toInt();
        People l_producer = m_app->getDatabaseManager()->getOneProducerById(l_id);
        l_producersVector.push_back(l_producer);
    }

    return l_producersVector;
}

void MetadataWindow::setActors(QVector<People> actorsVector)
{
    for (int i = 0 ; i < actorsVector.size() ; i++)
    {
        People l_actor = actorsVector.at(i);
        QListWidgetItem *l_item = new QListWidgetItem(l_actor.getFirstname() + " " + l_actor.getLastname());
        l_item->setData(Qt::UserRole, l_actor.getId());
        m_ui->actorsWidget->insertItem(i, l_item);
    }
}


QVector<People> MetadataWindow::getActors()
{
    QVector<People> l_actorsVector;
    for (int i = 0 ; i < m_ui->actorsWidget->count() ; i++)
    {
        int l_id = m_ui->actorsWidget->item(i)->data(Qt::UserRole).toInt();
        People l_actor = m_app->getDatabaseManager()->getOneActorById(l_id);
        l_actorsVector.push_back(l_actor);
    }

    return l_actorsVector;
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
    QString l_text = m_ui->directorEdit->text();
    QVector<People> l_directorsVector = m_app->getDatabaseManager()->getPeopleByFullname(l_text, 1);
    if (l_directorsVector.size() == 1)
    {
        People l_director = l_directorsVector.at(0);
        addDirector(l_director);
        m_app->debug(l_text + " added");
    }
    else
    {
        PeopleWindow *l_peopleWindow = new PeopleWindow;
        // We suppose here that a name is composed by N >= 0 firstnames
        // and 1 lastname, separated by spaces
        QStringList l_textExplosed = l_text.split(" ");
        QString l_lastname = l_textExplosed.last();
        l_textExplosed.removeLast();
        QString l_firstname = l_textExplosed.join(" ");
        l_peopleWindow->setFirstname(l_firstname);
        l_peopleWindow->setLastname(l_lastname);
        l_peopleWindow->show();
        QObject::connect(l_peopleWindow, SIGNAL(peopleCreated(People)), this, SLOT(peopleWindow_peopleCreated(People)));
    }
}

void MetadataWindow::on_addProducerButton_clicked()
{
    m_app->debug("[MetadataWindow] addProducerButton clicked()");
}

void MetadataWindow::on_addActorButton_clicked()
{
    m_app->debug("[MetadataWindow] addActorButton clicked()");
}

void MetadataWindow::on_delDirectorButton_clicked()
{
    m_app->debug("[MetadataWindow] delDirectorButton clicked()");
    QList<QListWidgetItem*> l_itemsListToDelete = m_ui->directorsWidget->selectedItems();
    foreach (QListWidgetItem *l_itemToDelete, l_itemsListToDelete)
    {
        int l_directorId = l_itemToDelete->data(Qt::UserRole).toInt();
        People l_director = m_app->getDatabaseManager()->getOneDirectorById(l_directorId);
        delDirector(l_director);
        delete(l_itemToDelete);
    }
}

void MetadataWindow::on_delProducerButton_clicked()
{
    m_app->debug("[MetadataWindow] delProducerButton clicked()");
}

void MetadataWindow::on_delActorButton_clicked()
{
    m_app->debug("[MetadataWindow] delActorButton clicked()");
}

void MetadataWindow::on_directorEdit_textEdited()
{
    m_app->debug("[MetadataWindow] directorEdit textEdited()");
    QString l_text =  m_ui->directorEdit->text();
    if (l_text.size() > 3)
    {
        QVector<People> l_directorsVector = m_app->getDatabaseManager()->getPeopleByFullname(l_text, Director);
        if(l_directorsVector.size() > 0)
        {
            People l_director;
            QStringList l_propositions;
            foreach (l_director, l_directorsVector)
            {
                l_propositions << l_director.getFirstname() + " " + l_director.getLastname();
            }
            QCompleter *l_completer = new QCompleter(l_propositions);
            l_completer->setCaseSensitivity(Qt::CaseInsensitive);
            l_completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
            m_ui->directorEdit->setCompleter(l_completer);
         }
    }
}

void MetadataWindow::on_producerEdit_textEdited()
{
    m_app->debug("[MetadataWindow] producerEdit textEdited()");
}

void MetadataWindow::on_actorEdit_textEdited()
{
    m_app->debug("[MetadataWindow] actorEdit textEdited()");
}

void MetadataWindow::peopleWindow_peopleCreated(People people)
{
    addDirector(people);
}

/**
 * @brief Shows the context-menu where the user rightclicks
 *
 * @param QPoint position of the cursor
 */
void MetadataWindow::customMenuRequested(QPoint pos)
{
    m_app->debug("[MetadataWindow] Enters customMenuRequested()");
    QMenu *l_menu = new QMenu(this);
    QAction *l_setMetadataAction = new QAction("Update person", this);
    QObject::connect(l_setMetadataAction, SIGNAL(triggered()), this, SLOT(showPeopleWindow()));
    l_menu->addAction(l_setMetadataAction);
    l_menu->popup(m_ui->directorsWidget->viewport()->mapToGlobal(pos));
    m_app->debug("[MetadataWindow] Exits customMenuRequested()");
}

/**
 * @brief Shows the window to view/edit the metadata of a people
 */
void MetadataWindow::showPeopleWindow()
{
    m_app->debug("[MetadataWindow] Enters showPeopleWindow()");
    int l_peopleId = m_ui->directorsWidget->selectedItems().at(0)->data(Qt::UserRole).toInt();
    PeopleWindow *l_peopleWindow = new PeopleWindow(l_peopleId);
    l_peopleWindow->show();
    m_app->debug("[MetadataWindow] Exits showPeopleWindow()");
}
