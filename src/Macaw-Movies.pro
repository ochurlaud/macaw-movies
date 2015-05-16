# /* Copyright (C) 2014 Macaw-Movies
#  * (Olivier CHURLAUD)
#  *
#  * This file is part of Macaw-Movies.
#  *
#  * Macaw-Movies is free software: you can redistribute it and/or modify
#  * it under the terms of the GNU General Public License as published by
#  * the Free Software Foundation, either version 3 of the License, or
#  * (at your option) any later version.
#  *
#  * Macaw-Movies is distributed in the hope that it will be useful,
#  * but WITHOUT ANY WARRANTY; without even the implied warranty of
#  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  * GNU General Public License for more details.
#  *
#  * You should have received a copy of the GNU General Public License
#  * along with Macaw-Movies.  If not, see <http://www.gnu.org/licenses/>.
#  */

#-------------------------------------------------
#
# Project created by QtCreator 2014-05-12T18:53:04
#
#-------------------------------------------------

QT       += core
QT	 += gui
QT       += sql
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Macaw-Movies
TEMPLATE = app

ICON = img/logov0_256.ico
RC_ICONS = img/logov0_256.ico

SOURCES += main.cpp \
    DatabaseManager.cpp \
    Application.cpp \
    Entities/Movie.cpp \
    Entities/People.cpp \
    Entities/Tag.cpp \
    MainWindow.cpp \
    PeopleDialog.cpp \
    MovieDialog.cpp \
    Entities/Playlist.cpp \
    DatabaseManager_getters.cpp \
    DatabaseManager_insert.cpp \
    DatabaseManager_update.cpp \
    DatabaseManager_delete.cpp \
    SettingsWindow.cpp \
    FetchMetadata/FetchMetadata.cpp \
    FetchMetadata/FetchMetadataDialog.cpp \
    FetchMetadata/FetchMetadataQuery.cpp \
    MacawDebug.cpp \
    MainWindowWidgets/LeftPannel.cpp \
    ServicesManager.cpp \
    MainWindowWidgets/MoviesPannel.cpp \
    MainWindowWidgets/MainPannel.cpp

HEADERS  += \
    DatabaseManager.h \
    Application.h \
    Entities/Movie.h \
    Entities/People.h \
    Entities/Tag.h \
    MainWindow.h \
    MovieDialog.h \
    PeopleDialog.h \
    Entities/Playlist.h \
    include_var.h \
    SettingsWindow.h \
    FetchMetadata/FetchMetadataDialog.h \
    FetchMetadata/FetchMetadata.h \
    FetchMetadata/FetchMetadataQuery.h \
    MacawDebug.h \
    MainWindowWidgets/LeftPannel.h \
    ServicesManager.h \
    MainWindowWidgets/MoviesPannel.h \
    MainWindowWidgets/MainPannel.h

FORMS    += \
    MainWindow.ui \
    MovieDialog.ui \
    PeopleDialog.ui \
    FetchMetadata/FetchMetadataDialog.ui \
    SettingsWindow.ui \
    MainWindowWidgets/LeftPannel.ui \
    MainWindowWidgets/MoviesPannel.ui

OTHER_FILES += \
    ../.gitignore \
    ../COPYING \
    ../README.md \
    ../DDB_SCHEMA.md

RESOURCES += \
    images.qrc

DISTFILES +=
