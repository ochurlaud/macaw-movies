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

#ifndef MAIN
#define MAIN

#include <QApplication>
#include <QDir>

#include "Application.h"
#include "MainWindow.h"
#include "MoviesDebug.h"
#include "MetadataFetcher.h"

int main(int argv, char **args)
{
    MoviesDebug *movieDebug = new MoviesDebug(true);
    Application l_app(movieDebug, argv, args);
    MainWindow  l_mainWindow;
    l_mainWindow.show();


/*/ FOR TESTING PURPOSES
    Movie movie = l_app.getDatabaseManager()->getOneMovieById(2);
    MetadataFetcher metadata(movie);
    metadata.fetchMetadata(movie.getTitle());
/*/
    return l_app.exec();
}

#endif
