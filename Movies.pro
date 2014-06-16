# /* Copyright (C) 2014 MusicProject
#  * (Olivier CHURLAUD)
#  *
#  * This file is part of MusicProject.
#  *
#  * MusicProject is free software: you can redistribute it and/or modify
#  * it under the terms of the GNU General Public License as published by
#  * the Free Software Foundation, either version 3 of the License, or
#  * (at your option) any later version.
#  *
#  * MusicProject is distributed in the hope that it will be useful,
#  * but WITHOUT ANY WARRANTY; without even the implied warranty of
#  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  * GNU General Public License for more details.
#  *
#  * You should have received a copy of the GNU General Public License
#  * along with MusicProject.  If not, see <http://www.gnu.org/licenses/>.
#  */

#-------------------------------------------------
#
# Project created by QtCreator 2014-05-12T18:53:04
#
#-------------------------------------------------

QT       += core gui
QT       += sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Movies
TEMPLATE = app


SOURCES += main.cpp \
    DatabaseManager.cpp \
    MainWindow.cpp \
    Application.cpp \
    SettingsWindow.cpp

HEADERS  += \
    DatabaseManager.h \
    MainWindow.h \
    Application.h \
    SettingsWindow.h

FORMS    += \
