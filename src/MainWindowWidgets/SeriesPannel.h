#ifndef SERIESPANNEL_H
#define SERIESPANNEL_H

#include <QWidget>

#include "MainWindowWidgets/MainPannel.h"

namespace Ui {
class SeriesPannel;
}

class SeriesPannel : public MainPannel
{
    Q_OBJECT

public:
    explicit SeriesPannel(QWidget *parent = 0);
    ~SeriesPannel();

private:
    Ui::SeriesPannel *m_ui;
};

#endif // SERIESPANNEL_H
