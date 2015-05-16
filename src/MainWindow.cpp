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

#include "MainWindow.h"
#include "ui_MainWindow.h"

#ifdef Q_OS_WIN
    #include "windows.h"
#endif

/**
 * @brief Constructor
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow)
{
    Macaw::DEBUG_IN("[MainWindow] Constructor called");

    m_leftPannel = new LeftPannel;
    m_mainPannel = new MoviesPannel;

    m_ui->setupUi(this);
    m_ui->leftPannelLayout->addWidget(m_leftPannel);
    m_ui->mainPannelLayout->addWidget(m_mainPannel);

    connect(m_leftPannel, SIGNAL(updateMainPannel()),
            this, SLOT(updateMainPannel()));

    this->readSettings();

    this->setWindowTitle(APP_NAME);

    this->updatePannels();
    Macaw::DEBUG_OUT("[MainWindow] Construction done");
}

/**
 * @brief Destructor
 */
MainWindow::~MainWindow()
{
    delete m_ui;
    Macaw::DEBUG("[MainWindow] Destructed");
}

/**
 * @brief Call and shows the settings window.
 * Calls the SettingsWindow class and waits until it closes to handle the results.
 */
void MainWindow::on_actionEdit_Settings_triggered()
{
    Macaw::DEBUG_IN("[MainWindow] Enters showSettingsWindow()");
    SettingsWindow *l_settingsWindow = new SettingsWindow;
    l_settingsWindow->show();
    QObject::connect(l_settingsWindow,SIGNAL(closeAndSave()), this, SLOT(addNewMovies()));
    Macaw::DEBUG_OUT("[MainWindow] Exits showSettingsWindow()");
}

/**
 * @brief Slot triggered when the toWatchButton is clicked.
 * Only the movies from the playlist 1 ("To Watch") are displayed.
 */
void MainWindow::on_toWatchButton_clicked()
{
    Macaw::DEBUG("[MainWindow] toWatchButton clicked");

    ServicesManager *servicesManager = ServicesManager::instance();
    servicesManager->setToWatchState(!servicesManager->toWatchState());

    this->updatePannels();
}

/**
 * @brief Returns the movies to display in mainWindow, based on an id and m_typeElement.
 *
 * @param id of the leftPannel element
 * @return QList of movies to display
 */
QList<Movie> MainWindow::moviesToDisplay(int id)
{
    Macaw::DEBUG("[MainWindow] prepareMoviesToDisplay()");
    DatabaseManager *databaseManager = ServicesManager::instance()->databaseManager();


    m_leftPannel->setSelectedId(id);
    if(m_leftPannel->selectedId() == 0) {

        return databaseManager->getAllMovies();
    } else if(m_leftPannel->typeElement() == Macaw::isPeople) {
        if (m_leftPannel->selectedId() == -1) {

            return databaseManager->getMoviesWithoutPeople(m_leftPannel->typePeople());
        } else {

            return databaseManager->getMoviesByPeople(m_leftPannel->selectedId(), m_leftPannel->typePeople());
        }
    } else if (m_leftPannel->typeElement() == Macaw::isTag) {
        if (m_leftPannel->selectedId() == -1) {

            return databaseManager->getMoviesWithoutTag();
        } else {

            return databaseManager->getMoviesByTag(m_leftPannel->selectedId());
        }
    }
    QList<Movie> l_emptyList;

    return  l_emptyList;
}

/**
 * @brief Slot triggered when enter pressed in the search field
 * Call `updatePannels()`
 */
void MainWindow::on_searchEdit_returnPressed()
{
    Macaw::DEBUG("[MainWindow] editing finished on searchEdit");

    this->updatePannels();
}

/**
 * @brief Slot triggered when Dialogs are closed in order to update the pannels
 * Call `updatePannels()`
 */
void MainWindow::selfUpdate()
{
    Macaw::DEBUG("[MainWindow] selfUpdate() triggered");

    this->updatePannels();
}

void MainWindow::updateMainPannel()
{
    Macaw::DEBUG("[MainWindow] updateMainWindow triggered");
    QList<Movie> l_moviesList = moviesToDisplay(m_leftPannel->selectedId());
    m_mainPannel->fill(l_moviesList);
}

/**
 * @brief Filter the pannels to follow the requirements of the search field, playlists...
 * Black magic. To be simplified
 */
void MainWindow::updatePannels()
{
    Macaw::DEBUG_IN("[MainWindow] Enters updatePannels()");

    QString l_text = m_ui->searchEdit->text();

    ServicesManager *servicesManager = ServicesManager::instance();
    servicesManager->setAuthorizedMoviesList(l_text);

    m_leftPannel->fill();
    QList<Movie> l_moviesList = moviesToDisplay(m_leftPannel->selectedId());
    m_mainPannel->fill(l_moviesList);
    Macaw::DEBUG_OUT("[MainWindow] Exits updatePannels()");
}

/**
 * @brief Slot triggered to add the movies of the saved path.
 *
 * 1. Read all the paths
 * 2. For each file:
 *      -# Check that the suffix is correct
 *      -# Check that the file has not been already imported
 *      -# Import the movie in the database
 *      -# Request FetchMetadata to get the metadata on internet
 *      -# Update the movie
 * 3. Request the update of all pannels
 */
void MainWindow::addNewMovies()
{
    Macaw::DEBUG_IN("[MainWindow] Enter addNewMovies");

    DatabaseManager *databaseManager = ServicesManager::instance()->databaseManager();

    bool l_imported = false;
    int l_addedCount(0);
    QStringList l_moviesPathsList = databaseManager->getMoviesPaths(l_imported);

    foreach (QString l_moviesPath, l_moviesPathsList) {
        QDirIterator l_file(l_moviesPath, QDir::NoDotAndDotDot | QDir::Files,QDirIterator::Subdirectories);
        while (l_file.hasNext()) {
            l_file.next();
            QString l_filePath = l_file.fileInfo().absoluteFilePath();
            QString l_fileSuffix = l_file.fileInfo().suffix();
            QStringList l_authorizedSuffixList;
            l_authorizedSuffixList << "mkv"
                                   << "avi"
                                   << "mp4"
                                   << "mpg"
                                   << "flv"
                                   << "mov";
            if (l_authorizedSuffixList.contains(l_fileSuffix, Qt::CaseInsensitive)) {
                Macaw::DEBUG("[MainWindow.updateApp()] Suffix accepted");
                bool l_movieExists = databaseManager->existMovie(l_filePath);
                if (!l_movieExists) {
                    Macaw::DEBUG("[MainWindow.updateApp()] Movie not already known");
                    Movie l_movie;
                    l_movie.setTitle(l_file.fileInfo().completeBaseName());
                    l_movie.setFilePath(l_file.fileInfo().absoluteFilePath());
                    l_movie.setSuffix(l_fileSuffix);
                    databaseManager->insertNewMovie(l_movie);
                    l_addedCount++;
                    if(l_addedCount == 5) {
                        this->updatePannels();
                    }
                } else {
                    Macaw::DEBUG("[MainWindow.updateApp()] Movie already known. Skipped");
                }
            }
        }
        databaseManager->setMoviesPathImported(l_moviesPath,true);
    }

    QList<Movie> l_moviesToFetch = databaseManager->getMoviesNotImported();
    if (!l_moviesToFetch.isEmpty()) {
        emit startFetchingMetadata(l_moviesToFetch);
    }
    this->updatePannels();
    Macaw::DEBUG_OUT("[MainWindow] Exit addNewMovies");
}

/**
 * @brief Fill the Metadata pannel with the data of a given movie
 *
 * @param movie which metadata should be shown
 */
void MainWindow::fillMetadataPannel(Movie movie)
{
    Macaw::DEBUG_IN("[MainWindow] Enter fillMetadataPannel");

    QString l_title = "<html>"+movie.title()+"<br />";
    QString l_originalTitle = "<i>"+movie.originalTitle()+"</i></br /><br />";
    QString l_directors = "<i>Directed by</i><br />";
    QString l_producers = "<i>Produced by</i><br />";
    QString l_actors = "<i>With</i><br />";

    foreach (People l_people, movie.peopleList()) {
        switch (l_people.type())
        {
        case People::Director:
            l_directors += l_people.name() + ", ";
            break;
        case People::Producer:
            l_producers += l_people.name() + ", ";
            break;
        case People::Actor:
            l_actors += l_people.name() + ", ";
            break;
        }
    }
    // Need to remove ", " = 2 chars
    if (l_directors.right(2) == ", ") {
        l_directors = l_directors.remove(l_directors.size()-2, 2);

    }
    l_directors += "<br /><br />";
    if (l_producers.right(2) == ", ") {
        l_producers = l_producers.remove(l_producers.size()-2,2);
    }
    l_producers += "<br /><br />";
    if (l_actors.right(2) == ", ") {
        l_actors = l_actors.remove(l_actors.size()-2,2);
    }
    l_actors += "<br /><br />";
    l_actors += "</html>";

    m_ui->metadataTop->setText(l_title+l_originalTitle + l_directors +l_producers + l_actors);
    m_ui->metadataPlot->setText(movie.synopsis());
    QPixmap l_poster;
    if (movie.posterPath() != "")
    {
        QString l_posterPath = qApp->property("postersPath").toString()
                               + movie.posterPath();
        l_poster.load(l_posterPath);
        QSize l_size(m_ui->metadataPannel->width()-30, m_ui->metadataPannel->height()-30);
        l_poster = l_poster.scaled(l_size, Qt::KeepAspectRatio);
    }
    m_ui->metadataCover->setPixmap(l_poster);

    Macaw::DEBUG_OUT("[MainWindow] Exit fillMetadataPannel");
}

/**
 * @brief Slot triggered when the application is closed.
 * Overload the QMainWindow class: save the window state to QSettings
 *
 * @param event
 */
void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings l_settings("Macaw-Movies", "Macaw-Movies");
    l_settings.setValue("geometry", saveGeometry());
    l_settings.setValue("windowState", saveState());
    l_settings.setValue("mainSplitter/geometry", m_ui->mainSplitter->saveGeometry());
    l_settings.setValue("mainSplitter/state", m_ui->mainSplitter->saveState());
    QMainWindow::closeEvent(event);
}

/**
 * @brief Read the QSettings of the application to give restore the last state of the window
 */
void MainWindow::readSettings()
{
    QSettings l_settings("Macaw-Movies", "Macaw-Movies");
    restoreGeometry(l_settings.value("geometry").toByteArray());
    restoreState(l_settings.value("windowState").toByteArray());
    m_ui->mainSplitter->restoreGeometry(l_settings.value("mainSplitter/geometry").toByteArray());
    m_ui->mainSplitter->restoreState(l_settings.value("mainSplitter/state").toByteArray());
}

/**
 * @brief Slot triggered when the user clicks on the About menu.
 * Show the about menu.
 */
void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, "About " APP_NAME,
                       "<h1>" APP_NAME "</h1><br />"
                       "<em>Copyright (C) 2014 Macaw-Movies<br />"
                       "(Olivier CHURLAUD, Sébastien TOUZÉ)</em>"
                       "<br /><br />"
                       "Compiled with Qt " QT_VERSION_STR ", uses the API of <a href='http://www.themoviedb.org/'>TMDB</a>"
                       "<br /><br />"
                       "Macaw-Movies is distributed in the hope that it will be useful, "
                       "but WITHOUT ANY WARRANTY; without even the implied warranty of "
                       "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.<br />"
                       "See the GNU General Public License for more details."
                       "<br /><hr /><br />"
                       "This application is still under active developpement, it may contain bugs. <br />"
                       "If you find one please create an issue on <a href='https://github.com/macaw-movies/macaw-movies/issues'>our GitHub space</a>");
}
