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
    Application.cpp \
    DatabaseManager.cpp \
    DatabaseManager_getters.cpp \
    DatabaseManager_insert.cpp \
    DatabaseManager_update.cpp \
    DatabaseManager_delete.cpp \
    MacawDebug.cpp \    
    MainWindow.cpp \    
    ServicesManager.cpp \
    Dialogs/PeopleDialog.cpp \
    Dialogs/MovieDialog.cpp \
    Entities/Movie.cpp \
    Entities/People.cpp \
    Entities/Playlist.cpp \
    Entities/Tag.cpp \
    FetchMetadata/FetchMetadata.cpp \
    FetchMetadata/FetchMetadataDialog.cpp \
    FetchMetadata/FetchMetadataQuery.cpp \
    MainWindowWidgets/LeftPannel.cpp \
    MainWindowWidgets/MoviesPannel.cpp \
    MainWindowWidgets/MainPannel.cpp \
    MainWindowWidgets/MetadataPannel.cpp \
    Entities/Entity.cpp \
    Entities/Episode.cpp \
    Entities/PathForMovies.cpp \
    Dialogs/SettingsDialog.cpp \
    Dialogs/SettingsDialogWidgets/MoviePathsSettings.cpp \
    Dialogs/SettingsDialogWidgets/CenteredCheckbox.cpp \
    Entities/Show.cpp \
    MainWindowWidgets/ShowsPannel.cpp

HEADERS  += \
    include_var.h \
    Application.h \
    DatabaseManager.h \
    MacawDebug.h \
    MainWindow.h \
    ServicesManager.h \
    Dialogs/MovieDialog.h \
    Dialogs/PeopleDialog.h \
    Entities/Movie.h \
    Entities/People.h \
    Entities/Playlist.h \
    Entities/Tag.h \
    FetchMetadata/FetchMetadataDialog.h \
    FetchMetadata/FetchMetadata.h \
    FetchMetadata/FetchMetadataQuery.h \
    MainWindowWidgets/LeftPannel.h \
    MainWindowWidgets/MoviesPannel.h \
    MainWindowWidgets/MainPannel.h \
    MainWindowWidgets/MetadataPannel.h \
    Entities/Entity.h \
    Entities/Episode.h \
    Entities/PathForMovies.h \
    Dialogs/SettingsDialog.h \
    enumerations.h \
    Dialogs/SettingsDialogWidgets/MoviePathsSettings.h \
    Dialogs/SettingsDialogWidgets/CenteredCheckbox.h \
    Entities/Show.h \
    MainWindowWidgets/ShowsPannel.h

FORMS    += \
    MainWindow.ui \
    Dialogs/MovieDialog.ui \
    Dialogs/PeopleDialog.ui \
    FetchMetadata/FetchMetadataDialog.ui \
    MainWindowWidgets/LeftPannel.ui \
    MainWindowWidgets/MoviesPannel.ui \
    MainWindowWidgets/MetadataPannel.ui \
    Dialogs/SettingsDialog.ui \
    Dialogs/SettingsDialogWidgets/MoviePathsSettings.ui \
    MainWindowWidgets/ShowsPannel.ui

TRANSLATIONS += \
    locales/macaw_fr.ts \
    locales/macaw_de.ts

OTHER_FILES += \
    ../.gitignore \
    ../COPYING \
    ../README.md \
    ../DDB_SCHEMA.md

RESOURCES += \
    resources.qrc

DISTFILES +=
