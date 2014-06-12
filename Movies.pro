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
    MainWindow.ui \
