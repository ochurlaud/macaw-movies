#ifndef METADATAWINDOW_H
#define METADATAWINDOW_H

#include <QDialog>
#include "Application.h"

class Application;

namespace Ui {
class MetadataWindow;
}

class MetadataWindow : public QDialog
{
    Q_OBJECT

    public:
        explicit MetadataWindow(int, QWidget *parent = 0);
        ~MetadataWindow();
        void setTitle(QString);
        QString getTitle();
        void setOriginalTitle(QString);
        QString getOriginalTitle();
        void setYear(int);
        int getYear();
        void setCountry(QString);
        QString getCountry();
        void setSynopsis(QString);
        QString getSynopsis();
        void setDirectors(QVector<People>);
        QVector<People> getDirectors();
        void setActors(QVector<People>);
        QVector<People> getActors();
        void setProducers(QVector<People>);
        QVector<People> getProducers();

    private slots:
        void updateMovie();

    private:
        Ui::MetadataWindow *m_ui;
        Movie m_movie;
        Application *m_app;
};

#endif // METADATAWINDOW_H
