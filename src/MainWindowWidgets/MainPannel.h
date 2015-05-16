#ifndef MAINPANNEL_H
#define MAINPANNEL_H

#include <QWidget>

#include "Entities/Movie.h"


class MainPannel : public QWidget
{
    Q_OBJECT

public:
    MainPannel(QWidget *parent);
    virtual void fill(QList<Movie> const &moviesList){ moviesList.count(); }

signals:
    void fillMetadataPannel(const Movie&);
    void updatePannels();

};

#endif // MAINPANNEL_H
