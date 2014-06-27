/* Copyright (C) 2014 Movie-Project
 * (Olivier CHURLAUD)
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
#include "DatabaseManager.h"

#include <QtGui>
#include <QtWidgets>

#include <QtDebug>

class SettingsWindow : public QDialog
{
    Q_OBJECT

    public:
        explicit SettingsWindow(QDialog *parent = 0);
        ~SettingsWindow();
        void closeEvent(QCloseEvent *event);


    public slots:
        void applySetting();
        void browseFilesPathDialog();

    signals:
        void closed();

    private:
        QLabel *m_filesPathLabel;
        QLabel *m_filesPathMessage;
        QLineEdit *m_filesPathEdit;
        QPushButton *m_filesPathSearchButton;
        QPushButton *m_submitButton;
        QVBoxLayout *m_mainLayout;
        QHBoxLayout *m_filesPathLayout;
};

#endif // SETTINGWINDOW_H
