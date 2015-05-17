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
    QList<int> m_elementIdsList;

    void setElementIdsList();
    template<typename T> void updateElementIdsList(const QList<T> &list);
    void fillListWidget();
    void addEntityToListWidget(const Entity &entity);
};

#endif // LEFTPANNEL_H
