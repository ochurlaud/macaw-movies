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

#include "MoviePathsSettings.h"
#include "ui_MoviePathsSettings.h"

MoviePathsSettings::MoviePathsSettings(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::MoviePathsSettings)
{
    m_ui->setupUi(this);

    DatabaseManager *databaseManager = ServicesManager::instance()->databaseManager();
    QList<PathForMovies> l_moviesPathList = databaseManager->getMoviesPaths(false);
    l_moviesPathList << databaseManager->getMoviesPaths(true);

    connect(m_ui->knownPathCollection, SIGNAL(cellChanged(int,int)),
            this, SLOT(updatedCheckboxes(int,int)));

    m_ui->knownPathCollection->setHorizontalHeaderLabels(QStringList() << tr("Path to search")
                                                                       << tr("Movies")
                                                                       << tr("TV Shows")
                                                         );
    m_ui->knownPathCollection->setColumnCount(4);
    m_ui->knownPathCollection->hideColumn(3); // Used for id.

    m_ui->knownPathCollection->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
    m_ui->knownPathCollection->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Fixed);
    m_ui->knownPathCollection->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Fixed);

    int l_colWidth = 65;
    m_ui->knownPathCollection->setColumnWidth(0,l_colWidth);
    m_ui->knownPathCollection->setColumnWidth(1,l_colWidth);


    for (int i = 0 ; i < l_moviesPathList.count() ; i++) {
        PathForMovies l_moviesPath = l_moviesPathList.at(i);
        this->addToKnownPathCollection(l_moviesPath);
    }
}

MoviePathsSettings::~MoviePathsSettings()
{
    delete m_ui;
}

void MoviePathsSettings::saveSettings()
{
    DatabaseManager *databaseManager = ServicesManager::instance()->databaseManager();

    // in case the lazy user didn't click `Add`
    this->addPathFromEdit();

    QList<PathForMovies> l_moviesPathList = databaseManager->getMoviesPaths(true);
    l_moviesPathList << databaseManager->getMoviesPaths(false);
    foreach (PathForMovies l_moviesPath, l_moviesPathList) {
        if (m_ui->knownPathCollection->findItems(QString::number(l_moviesPath.id()),
                                                 Qt::MatchExactly).isEmpty()) {
            Macaw::DEBUG("[MainWindow] Remove path "+l_moviesPath.path());
            // We remove the path and all the movies behind
            databaseManager->deleteMoviesPath(l_moviesPath);
        }
    }

    for (int i=0 ; i < m_ui->knownPathCollection->rowCount() ; i++) {
        QString l_path = m_ui->knownPathCollection->item(i,0)->text();
        bool l_movies = qobject_cast<CenteredCheckbox*>(m_ui->knownPathCollection->cellWidget(i,1))->isChecked();
        bool l_shows = qobject_cast<CenteredCheckbox*>(m_ui->knownPathCollection->cellWidget(i,2))->isChecked();

        PathForMovies l_moviesPath(l_path,l_movies,l_shows);
        l_moviesPath.setId(m_ui->knownPathCollection->item(i,3)->text().toInt());

        if (databaseManager->existMoviesPath(l_moviesPath)) {
            databaseManager->updateMoviesPath(l_moviesPath);
        }
        databaseManager->addMoviesPath(l_moviesPath);
        if (m_ui->recheckBox->isChecked()) {
            databaseManager->setMoviesPathImported(l_path, false);
        }
    }
}

/**
 * @brief Slot triggered when folderPathEdit changed. Enable the `addButton` if it is not empty.
 * @param QString of the folderPathEdit
 */
void MoviePathsSettings::on_folderPathEdit_textChanged(const QString string)
{
    if (!string.isEmpty()) {
        m_ui->addButton->setEnabled(true);
    } else {
        m_ui->addButton->setDisabled(true);
    }
}

/**
 * @brief Slot triggered when the checkboxes are updated. Force one checkbox to be checked.
 * @param int row
 * @param int column
 */
void MoviePathsSettings::updatedCheckboxes(int x, int y)
{
    if (y > 0) {
        CenteredCheckbox *l_moviesCheckbox =
                qobject_cast<CenteredCheckbox*>(m_ui->knownPathCollection->cellWidget(x,1));
        CenteredCheckbox *l_showsCheckbox =
                qobject_cast<CenteredCheckbox*>(m_ui->knownPathCollection->cellWidget(x,2));
        if (!l_moviesCheckbox->isChecked() && !l_showsCheckbox->isChecked()) {
            if (y == 1) {
                l_showsCheckbox->setChecked(true);
            } else {
                l_moviesCheckbox->setChecked(true);
            }
        }
    }
}

/**
 * @brief Slot triggered when the button `Browse` is clicked.
 * Help selecting a directory with a `QFileDialog`.
 */
void MoviePathsSettings::on_browseButton_clicked()
{
    Macaw::DEBUG_IN("[SettingsDialog] Enters browseFilesPathDialog()");
    QString l_folder = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                          QDir::homePath(),
                                                          QFileDialog::ShowDirsOnly);
    m_ui->folderPathEdit->setText(l_folder);
    Macaw::DEBUG_OUT("[SettingsDialog] Exits browseFilesPathDialog()");
}

/**
 * @brief Slot triggered when the `addButton` is clicked.
 * Call the `addPathFromEdit()` function to add the new path to the collection.
 */
void MoviePathsSettings::on_addButton_clicked()
{
    this->addPathFromEdit();
}

/**
 * @brief Slot triggered when the `removeButton` is clicked.
 * Remove the selected rows from knownPathCollection
 */
void MoviePathsSettings::on_removeButton_clicked()
{
    QList<QTableWidgetItem*> l_selectedItemList = m_ui->knownPathCollection->selectedItems();
    foreach (QTableWidgetItem *l_selectedItem, l_selectedItemList) {
        m_ui->knownPathCollection->removeRow(l_selectedItem->row());
    }
}

/**
 * @brief Request to add a new path to the `knownPathCollection` from the `folderPathEdit`
 */
void MoviePathsSettings::addPathFromEdit()
{
    QString l_newPath = m_ui->folderPathEdit->text();
    PathForMovies l_moviePath(l_newPath);
    if (!existsInPathCollection(l_newPath)) {
        addToKnownPathCollection(l_moviePath);
    }
    m_ui->folderPathEdit->clear();
}

/**
 * @brief Check if the given path exists in the `knownPathCollection`
 * @param path to add
 * @return bool
 */
bool MoviePathsSettings::existsInPathCollection(QString path)
{
    if (!QDir(path).exists() || path.isEmpty()) {
        m_ui->folderPathMessage->setText(tr("Choose an existent path"));

        return true;
    } else {
        for (int i = 0 ; i < m_ui->knownPathCollection->rowCount() ; i++) {
            // Do not include folder which are subfolders of known ones
            if (path.startsWith(m_ui->knownPathCollection->itemAt(i,0)->text())) {
                m_ui->folderPathMessage->setText(tr("This path is already included"));

                return true;
            }
        }
    }

    return false;
}

/**
 * @brief Add the new path to the collection.
 * @param PathForMovies to add
 */
void MoviePathsSettings::addToKnownPathCollection(PathForMovies moviePath)
{
    CenteredCheckbox *l_moviesCheckbox = new CenteredCheckbox();
    l_moviesCheckbox->setChecked(moviePath.hasMovies());

    CenteredCheckbox *l_showsCheckbox = new CenteredCheckbox();
    l_showsCheckbox->setChecked(moviePath.hasShows());

    int l_rowCount = m_ui->knownPathCollection->rowCount();
    m_ui->knownPathCollection->setRowCount(l_rowCount+1);
    m_ui->knownPathCollection->setItem(l_rowCount, 0, new QTableWidgetItem(moviePath.path()));
    m_ui->knownPathCollection->setCellWidget(l_rowCount, 1, l_moviesCheckbox);
    m_ui->knownPathCollection->setCellWidget(l_rowCount, 2, l_showsCheckbox);
    m_ui->knownPathCollection->setItem(l_rowCount, 3, new QTableWidgetItem(QString::number(moviePath.id())));
}
