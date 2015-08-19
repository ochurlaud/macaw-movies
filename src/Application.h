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

#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>

class FetchMetadata;
class MainWindow;
class Movie;
class People;
class Tag;

/**
 * @brief The Application class. Core of the application
 *
 * @author Olivier CHURLAUD <olivier@churlaud.com>
 */
class Application : public QApplication
{
Q_OBJECT

public:
    Application(int &argc, char **argv);
    ~Application();
    int exec();
    QString tmdbkey() { return m_tmdbkey; }

signals:
    void updateMainWindow();

private slots:
    void askForOrphanTagDeletion(const Tag &orphanTag);
    void askForOrphanPeopleDeletion(const People &orphanPeople);
    void on_startFetchingMetadata(const QList<Movie> &movieList);
    void on_fethMetadataJobDone();
    void on_fethMetadataUpdatedMovie();

private:

    /**
     * @brief API key for The Movie Database (TMDb)
     *
     * should be called with `m_app->tmdbkey()`
     */
    QString m_tmdbkey;

    /**
     * @brief MainWindow: the widget where everything happens
     */
    MainWindow *m_mainWindow;

    /**
     * @brief Metadata-fetching manager
     */
    FetchMetadata *m_fetchMetadata;

    /**
     * @brief Define the paths used in the app
     */
    void definePaths();

};

#endif // APPLICATION_H
