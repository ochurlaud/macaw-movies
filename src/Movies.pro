# /* Copyright (C) 2014 Movie-Project
#  * (Olivier CHURLAUD)
#  *
#  * This file is part of Movie-Project.
#  *
#  * Movie-Project is free software: you can redistribute it and/or modify
#  * it under the terms of the GNU General Public License as published by
#  * the Free Software Foundation, either version 3 of the License, or
#  * (at your option) any later version.
#  *
#  * Movie-Project is distributed in the hope that it will be useful,
#  * but WITHOUT ANY WARRANTY; without even the implied warranty of
#  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  * GNU General Public License for more details.
#  *
#  * You should have received a copy of the GNU General Public License
#  * along with Movie-Project.  If not, see <http://www.gnu.org/licenses/>.
#  */

#-------------------------------------------------
#
# Project created by QtCreator 2014-05-12T18:53:04
#
#-------------------------------------------------

QT       += core
QT	 += gui
QT       += sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Movies
TEMPLATE = app


SOURCES += main.cpp \
    DatabaseManager.cpp \
    Application.cpp \
    SettingsWindow.cpp \
    MoviesDebug.cpp \
    Entities/Movie.cpp \
    Entities/People.cpp \
    Entities/Tag.cpp \
    MetadataWindow.cpp \
    PeopleWindow.cpp \
    MainWindow.cpp

HEADERS  += \
    DatabaseManager.h \
    Application.h \
    SettingsWindow.h \
    MoviesDebug.h \
    Entities/Movie.h \
    Entities/People.h \
    Entities/Tag.h \
    MetadataWindow.h \
    PeopleWindow.h \
    MainWindow.h

FORMS    += \
    MetadataWindow.ui \
    PeopleWindow.ui \
    MainWindow.ui

OTHER_FILES += \
    ../.gitignore \
    ../COPYING \
    ../README.md \
    ../DDB_SCHEMA.md

RESOURCES += \
    images.qrc
