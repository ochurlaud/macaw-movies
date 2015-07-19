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

#ifndef LEFTPANNEL_H
#define LEFTPANNEL_H

#include <QWidget>
#include "Application.h"

namespace Ui {
    class LeftPannel;
}

class LeftPannel : public QWidget
{
    Q_OBJECT

public:
    explicit LeftPannel(QWidget *parent = 0);
    ~LeftPannel();
    void fill();
    int selectedId() const { return m_selectedId; }
    int typeElement() const { return m_typeElement; }
    int typePeople() const { return m_typePeople; }
    void setSelectedId(const int id){ m_selectedId = id; }

signals:
    void updateMainPannel();

private slots:
    void on_leftPannelBox_activated(int type);
    void on_customContextMenuRequested(const QPoint &point);
    void on_actionEdit_leftPannelMetadata_triggered();
    void on_listWidget_itemSelectionChanged();

private:
    Ui::LeftPannel *m_ui;

    /**
     * @brief type of elements lastly shown in the leftPannel (Tags/People)
     */
    int m_typeElement;

    /**
     * @brief type of People (if People) lastly shown in the leftPannel
     */
    int m_typePeople;

    /**
     * @brief id of the selected element in leftPannel
     */
    int m_selectedId;

    /**
     * @brief QList of ids of the leftPannel
     */
    QList<int> m_elementIdList;

    void setElementIdList();
    template<typename T> void updateElementIdList(const QList<T> &list);
    void fillListWidget();
    void addEntityToListWidget(const Entity &entity);
};

#endif // LEFTPANNEL_H
