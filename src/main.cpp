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
//#include "FetchMetadata/FetchMetadata.h"

int main(int argv, char **args)
{
    MoviesDebug *movieDebug = new MoviesDebug(true);
    Application l_app(argv, args, movieDebug);

    QCommandLineParser l_parser;
    l_parser.setApplicationDescription("DESCRIPTION");
    l_parser.addHelpOption();
    l_parser.addVersionOption();

    // --authors option
    const QCommandLineOption l_author(QStringList() << QStringLiteral("author"), QApplication::tr("Show author informations"));
    l_parser.addOption(l_author);
    // --license option
    const QCommandLineOption l_license(QStringList() << QStringLiteral("license"), QApplication::tr("Show license informations"));
    l_parser.addOption(l_license);

    /**
     * do the command line parsing
     */
    l_parser.process(l_app);

    if (l_parser.isSet(QStringLiteral("license")))
    {

        //the installer should put our license in the good folder and read it
        printf("%s\n",qPrintable(QApplication::tr("Licence dsds")));
        ::exit(EXIT_SUCCESS);
    }
    if (l_parser.isSet(QStringLiteral("author")))
    {
        printf("%s\n",qPrintable(QApplication::tr(APP_NAME " was written by")));
        printf("%s\n",qPrintable("    Olivier Churlaud <olivier@churlaud.com>"));
        printf("%s\n",qPrintable("    Sébastien Touzé"));
        ::exit(EXIT_SUCCESS);
    }

    MainWindow  l_mainWindow;
    l_mainWindow.show();

/*/ FOR TESTING PURPOSES
    Movie movie = l_app.getDatabaseManager()->getOneMovieById(8);
    movie.setTitle("La menace.xds98IU");
    FetchMetadata metadata(movie);
/*/
    return l_app.exec();
}

#endif
