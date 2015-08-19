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

#ifndef CENTEREDCHECKBOX_H
#define CENTEREDCHECKBOX_H

#include <QWidget>

class QCheckBox;

class CenteredCheckbox : public QWidget
{
    Q_OBJECT

public:
    explicit CenteredCheckbox(QWidget *parent = 0);
    ~CenteredCheckbox();
    void setChecked(const bool checked);
    bool isChecked() const;

signals:
    void stateChanged(int state);

private slots:
    void on_checkbox_stateChanged(int state);

private:
    QCheckBox *m_checkbox;
};

#endif // CENTEREDCHECKBOX_H
