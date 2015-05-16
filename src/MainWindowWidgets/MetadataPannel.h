#ifndef METADATAPANNEL_H
#define METADATAPANNEL_H

#include <QWidget>

#include "Application.h"
#include "Entities/Movie.h"
#include "Entities/People.h"

namespace Ui {
class MetadataPannel;
}

class MetadataPannel : public QWidget
{
    Q_OBJECT

public:
    explicit MetadataPannel(QWidget *parent = 0);
    ~MetadataPannel();
    void fill(const Movie &movie);

private:
    Ui::MetadataPannel *m_ui;
};

#endif // METADATAPANNEL_H
