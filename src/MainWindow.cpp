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

#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow)
{
    m_app = qobject_cast<Application *>(qApp);
    m_app->debug("[MainWindow] Constructor called");

    m_ui->setupUi(this);
    this->setWindowTitle(m_app->getAppName());
    this->setWindowIcon(m_app->getAppIcon());

    connect(m_ui->mainPannel, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(on_customContextMenuRequested(const QPoint &)));
    connect(m_ui->leftPannel, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(on_customContextMenuRequested(const QPoint &)));

    m_moviesVector = m_app->getDatabaseManager()->getAllMovies();
    m_leftPannelSelectedId = 0;
    fillMainPannel();
    fillLeftPannel(isPeople, Director);
    m_app->debug("[MainWindow] Construction done");
}

MainWindow::~MainWindow()
{
    delete m_ui;
    m_app->debug("[MainWindow] Destructed");
}

/**
 * @brief Call and shows the settings window.
 * Calls the SettingsWindow class and waits until it closes to handle the results
 */
void MainWindow::on_actionEdit_Settings_triggered()
{
    m_app->debug("[MainWindow] Enters showSettingsWindow()");
    SettingsWindow *l_settingsWindow = new SettingsWindow;
    l_settingsWindow->show();
    QObject::connect(l_settingsWindow,SIGNAL(destroyed()), this, SLOT(selfUpdate()));
    m_app->debug("[MainWindow] Exits showSettingsWindow()");
}

void MainWindow::fillLeftPannel(int typeElement, int typePeople = 0)
{
    m_ui->leftPannel->clear();
    m_typeElement = typeElement;
    m_typePeople = typePeople;

    QListWidgetItem *l_item = new QListWidgetItem("All");
    l_item->setData(Qt::UserRole, 0);
    m_ui->leftPannel->addItem(l_item);
    if (m_leftPannelSelectedId == 0)
    {
        l_item->setSelected(true);
    }

    if (typeElement == isPeople)
    {
        QListWidgetItem *l_item = new QListWidgetItem("Unknown");
        l_item->setData(Qt::UserRole, -1);
        l_item->setData(Qt::UserRole+1, isPeople);
        m_ui->leftPannel->addItem(l_item);
        if (m_leftPannelSelectedId == -1)
        {
            l_item->setSelected(true);
        }

        QVector<People> l_peopleVector = m_app->getDatabaseManager()->getAllPeople(typePeople);
        foreach (People l_people, l_peopleVector)
        {
            QString l_name(l_people.getLastname());
            if (l_people.getFirstname() != "")
            {
                l_name = l_name + ", " + l_people.getFirstname();
            }
            l_name = l_name  + " (" + QString::number(l_people.getBirthday().year()) + ")";

            QListWidgetItem *l_item = new QListWidgetItem(l_name);
            l_item->setData(Qt::UserRole, l_people.getId());
            l_item->setData(Qt::UserRole+1, isPeople);
            l_item->setData(Qt::UserRole+2, typePeople);
            if (m_leftPannelSelectedId == l_people.getId())
            {
                l_item->setSelected(true);
            }

            m_ui->leftPannel->addItem(l_item);
        }
    }
    else if(typeElement == isTag)
    {
        QListWidgetItem *l_item = new QListWidgetItem("No Tag");
        l_item->setData(Qt::UserRole, -1);
        l_item->setData(Qt::UserRole+1, isTag);
        m_ui->leftPannel->addItem(l_item);
        if (m_leftPannelSelectedId == -1)
        {
            l_item->setSelected(true);
        }

        QVector<Tag> l_tagsVector = m_app->getDatabaseManager()->getAllTags();
        foreach (Tag l_tag, l_tagsVector)
        {
            QString l_name(l_tag.getName());

            QListWidgetItem *l_item = new QListWidgetItem(l_name);
            l_item->setData(Qt::UserRole, l_tag.getId());
            l_item->setData(Qt::UserRole+1, isTag);

            m_ui->leftPannel->addItem(l_item);
            if (m_leftPannelSelectedId == l_tag.getId())
            {
                l_item->setSelected(true);
            }
        }
    }
    this->setLeftPannelLabel();
}

void MainWindow::fillMainPannel()
{
    m_app->debug("[MainWindow] Enters fillMainPannel()");

    m_ui->mainPannel->clear();
    m_ui->mainPannel->setColumnCount(4);

    QStringList l_headers;
    l_headers << "Title" << "Original Title" << "Release Date" << "Path of the file";
    m_ui->mainPannel->setHorizontalHeaderLabels(l_headers);
    m_ui->mainPannel->setRowCount(m_moviesVector.size());
    int l_row = 0;

    foreach (Movie l_movie, m_moviesVector)
    {
        int l_column = 0;
        QStringList l_movieData;
        l_movieData << l_movie.getTitle()
                    << l_movie.getOriginalTitle()
                    << l_movie.getReleaseDate().toString("dd MMM yyyy")
                    << l_movie.getFilePath();
        QVector<QTableWidgetItem*> l_itemsVector(4);
        foreach(QTableWidgetItem *l_item, l_itemsVector)
        {
            l_item = new QTableWidgetItem(l_movieData.at(l_column));
            l_item->setData(Qt::UserRole, l_movie.getId());
            l_item->setData(Qt::UserRole+1, isMovie);
            m_ui->mainPannel->setItem(l_row, l_column, l_item);
            l_column++;
        }
        l_row++;
    }

    m_app->debug("[MainWindow] Exits fillMainPannel()");
}

void MainWindow::on_peopleBox_activated(int type)
{
    m_app->debug("[MainWindow] Enters on_peopleBox_activated()");

    fillLeftPannel(isPeople, type);
    m_leftPannelSelectedId = 0;
}

void MainWindow::on_playlistsButton_clicked()
{

}

void MainWindow::on_toWatchButton_clicked()
{

}

void MainWindow::on_tagsButton_clicked()
{
    m_app->debug("[MainWindow] tagsButton clicked");
    fillLeftPannel(isTag);
    m_leftPannelSelectedId = 0;
}

void MainWindow::on_customContextMenuRequested(const QPoint &point)
{
    m_app->debug("[MainWindow] customContextMenuRequested()");
    QMenu *l_menu = new QMenu(this);
    if(m_ui->leftPannel->indexAt(point).isValid())
    {
        l_menu->addAction(m_ui->actionEdit_leftPannelMetadata);
        l_menu->exec(m_ui->leftPannel->mapToGlobal(point));
    }
    else if (m_ui->mainPannel->indexAt(point).isValid())
    {
        l_menu->addAction(m_ui->actionEdit_mainPannelMetadata);
        l_menu->exec(m_ui->mainPannel->mapToGlobal(point));
    }
}

void MainWindow::on_actionEdit_mainPannelMetadata_triggered()
{
    m_app->debug("[MainWindow] actionEdit_Metadata_triggered()");
    int l_id = m_ui->mainPannel->selectedItems().at(0)->data(Qt::UserRole).toInt();

    MetadataWindow *l_metadataWindow = new MetadataWindow(l_id);
    connect(l_metadataWindow, SIGNAL(destroyed()), this, SLOT(selfUpdate()));
    l_metadataWindow->show();
}

void MainWindow::on_actionEdit_leftPannelMetadata_triggered()
{
    int l_id = m_ui->leftPannel->selectedItems().at(0)->data(Qt::UserRole).toInt();

    // It's editable only if id is not 0
    if(l_id != 0)
    {
        int l_typeElement = m_ui->leftPannel->selectedItems().at(0)->data(Qt::UserRole+1).toInt();
        if (l_typeElement == isPeople)
        {
            PeopleWindow *l_metadataWindow = new PeopleWindow(l_id);
            connect(l_metadataWindow, SIGNAL(destroyed()), this, SLOT(selfUpdate()));
            l_metadataWindow->show();
        }
        else if (l_typeElement == isTag)
        {
            qDebug() << "Tag !";
        }
    }
}

void MainWindow::on_mainPannel_itemDoubleClicked(QTableWidgetItem *item)
{
    m_app->debug("[MainWindow] itemDoubleClicked on mainPannel()");

    int l_movieId = item->data(Qt::UserRole).toInt();
    Movie l_movie = m_app->getDatabaseManager()->getOneMovieById(l_movieId);

    m_app->debug("[MainWindow.startMovie()] Opened movie: " + l_movie.getFilePath());

    QDesktopServices::openUrl(QUrl("file://" + l_movie.getFilePath(), QUrl::TolerantMode));
}

void MainWindow::on_leftPannel_clicked(const QModelIndex &item)
{
    m_app->debug("[MainWindow] itemClicked on leftPannel()");
    m_leftPannelSelectedId = item.data(Qt::UserRole).toInt();
    this->prepareMoviesToDisplay(m_leftPannelSelectedId);
}

void MainWindow::on_mainPannel_clicked(const QModelIndex &index)
{
    m_app->debug("[MainWindow] mainPannel clicked");
}

void MainWindow::prepareMoviesToDisplay(int id)
{
    m_app->debug("[MainWindow] prepareMoviesToDisplay()");

    m_leftPannelSelectedId = id;
    if(m_leftPannelSelectedId == 0)
    {
        m_moviesVector = m_app->getDatabaseManager()->getAllMovies();
    }
    else if(m_typeElement == isPeople)
    {
        if (m_leftPannelSelectedId == -1)
        {
            m_moviesVector = m_app->getDatabaseManager()->getMoviesWithoutPeople(m_typePeople);
        }
        else
        {
            m_moviesVector = m_app->getDatabaseManager()->getMoviesByPeople(m_leftPannelSelectedId, m_typePeople);
        }
    }
    else if (m_typeElement == isTag)
    {
        if (m_leftPannelSelectedId == -1)
        {
            m_moviesVector = m_app->getDatabaseManager()->getMoviesWithoutTag();
        }
        else
        {
            m_moviesVector = m_app->getDatabaseManager()->getMoviesByTag(m_leftPannelSelectedId);
        }
    }
    filterPannels();
}

void MainWindow::selfUpdate()
{
    m_app->debug("[MainWindow] selfUpdate()");
    m_moviesVector.clear();
    fillLeftPannel(m_typeElement, m_typePeople);

    for (int i = 0 ; i < m_ui->leftPannel->count() ; i++)
    {
        QListWidgetItem *l_item = m_ui->leftPannel->item(i);
        if (l_item->data(Qt::UserRole).toInt() == m_leftPannelSelectedId)
        {

            l_item->setSelected(true);
            this->prepareMoviesToDisplay(l_item->data(Qt::UserRole).toInt());
            break;
        }
    }
}

void MainWindow::setLeftPannelLabel()
{
    switch (m_typeElement)
    {
    case isTag:
        m_ui->leftPannelLabel->setText("Tags");
        break;
    case isPeople:
        switch (m_typePeople)
        {
        case Director:
            m_ui->leftPannelLabel->setText("Directors");
            break;
        case Producer:
            m_ui->leftPannelLabel->setText("Producers");
            break;
        case Actor:
            m_ui->leftPannelLabel->setText("Actors");
            break;
        }
    }
}

void MainWindow::on_searchEdit_returnPressed()
{
    m_app->debug("[MainWindow] editing finished on searchEdit");

    QString l_text = m_ui->searchEdit->text();
    filterPannels();
}

void MainWindow::filterPannels()
{
    m_app->debug("[MainWindow] Enters filterPeople()");

    fillMainPannel();
    fillLeftPannel(m_typeElement, m_typePeople);

    QString l_text = m_ui->searchEdit->text();

    if (l_text != "")
    {
        // mainPannel
        QVector<Movie> l_authorizedMoviesVector = m_app->getDatabaseManager()->getMoviesByAny(l_text);
        for (int i = 0 ; i < m_ui->mainPannel->rowCount(); i++)
        {
            bool l_isPresent(false);
            QTableWidgetItem *l_item = m_ui->mainPannel->item(i, 0);
            foreach(Movie l_movie, l_authorizedMoviesVector)
            {
                 if (l_movie.getId() == l_item->data(Qt::UserRole))
                 {
                     l_isPresent = true;
                 }
            }
            if (l_isPresent == false)
            {
                m_ui->mainPannel->removeRow(i);
                i--; // We remove a line, so we have to decrement i...
            }
        }

        // leftPannel
       /* if (m_typeElement == isTag)
        {
            QVector<Movie> l_authorizedMoviesVector = m_app->getDatabaseManager()->getTagsByAny(l_text);
            for (int i = 0 ; i < m_ui->mainPannel->rowCount(); i++)
            {
                bool l_isPresent(false);
                QTableWidgetItem *l_item = m_ui->mainPannel->item(i);
                foreach(Movie l_movie, l_authorizedMoviesVector)
                {
                     if (l_movie.getId() == l_item->data(Qt::UserRole))
                     {
                         l_isPresent = true;
                     }
                }
                if (l_isPresent == false)
                {
                    m_ui->mainPannel->removeRow(i);
                }
            }
        }
        else*/ if (m_typeElement == isPeople)
        {
            QVector<People> l_authorizedPeopleVector = m_app->getDatabaseManager()->getPeopleByAny(l_text, m_typePeople);

            // We begin at 1 because the first item is "All"
            for (int i = 1 ; i < m_ui->leftPannel->count() ; i++)
            {
                bool l_isPresent(false);
                QListWidgetItem *l_item = m_ui->leftPannel->item(i);
                foreach(People l_people, l_authorizedPeopleVector)
                {
                     if (l_people.getId() == l_item->data(Qt::UserRole))
                     {
                         l_isPresent = true;
                     }
                }
                if (l_isPresent == false)
                {
                    delete l_item;
                    i--; // We remove an item so we have to decrement i
                }
            }
        }
    }
    for (int i = 0 ; i < m_ui->leftPannel->count() ; i ++)
    {
        QListWidgetItem *l_item = m_ui->leftPannel->item(i);
        if (m_leftPannelSelectedId == l_item->data(Qt::UserRole))
        {
            l_item->setSelected(true);
        }
    }
}
