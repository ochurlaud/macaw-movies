/* Copyright (C) 2014 Movie-Project
 * (Olivier CHURLAUD, Sébastien TOUZÉ)
 *
 * This file is part of Movie-Project.
 *
 * Movie-Project is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Movie-Project is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Movie-Project.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SETTINGWINDOW_H
#define SETTINGWINDOW_H

#include "Application.h"

#include <QtGui>
#include <QtWidgets>

class Application;

class SettingsWindow : public QDialog
{
Q_OBJECT

public:
    explicit SettingsWindow(QDialog *parent = 0);
    ~SettingsWindow();


public slots:
    void applySettings();
    void browseFilesPathDialog();

signals:
    void closeAndSave();

private:
    QLabel *m_filesPathLabel;
    QLabel *m_filesPathMessage;
    QLineEdit *m_filesPathEdit;
    QPushButton *m_filesPathSearchButton;
    QPushButton *m_submitButton;
    QPushButton *m_cancelButton;
    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_filesPathLayout;
    QHBoxLayout *m_closeLayout;
    QListView *m_knownPathList;
    Application *m_app;
};

#endif // SETTINGWINDOW_H
