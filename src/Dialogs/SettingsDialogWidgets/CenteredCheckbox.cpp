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

#include "CenteredCheckbox.h"

#include <QCheckBox>
#include <QLayout>

CenteredCheckbox::CenteredCheckbox(QWidget *parent) :
    QWidget(parent)
{
    this->setContentsMargins(10,0,0,0);
    m_checkbox = new QCheckBox;
    QHBoxLayout *l_layout = new QHBoxLayout(this);
    l_layout->addWidget(m_checkbox);
    l_layout->setAlignment(Qt::AlignCenter);
    l_layout->setContentsMargins(0,0,0,0);
    this->setLayout(l_layout);

    connect(m_checkbox, SIGNAL(stateChanged(int)),
            this, SLOT(on_checkbox_stateChanged(int)));
}

CenteredCheckbox::~CenteredCheckbox()
{
}

void CenteredCheckbox::setChecked(const bool checked)
{
    m_checkbox->setChecked(checked);
}

bool CenteredCheckbox::isChecked() const
{
    return m_checkbox->isChecked();
}

void CenteredCheckbox::on_checkbox_stateChanged(int state)
{
    emit stateChanged(state);
}
