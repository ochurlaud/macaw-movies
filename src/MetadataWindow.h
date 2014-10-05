#ifndef METADATAWINDOW_H
#define METADATAWINDOW_H

#include <QDialog>
#include "Application.h"
#include "PeopleWindow.h"

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
        void setReleaseDate(QDate);
        QDate getReleaseDate();
        void setCountry(QString);
        QString getCountry();
        void setSynopsis(QString);
        QString getSynopsis();
        void setDirectors(QVector<People>);
        QVector<People> getDirectors();
        void addDirector(People);
        void delDirector(int);
        void setActors(QVector<People>);
        QVector<People> getActors();
        void setProducers(QVector<People>);
        QVector<People> getProducers();

    private slots:
        void on_validationButtons_accepted();
        void on_addDirectorButton_clicked();
        void on_addProducerButton_clicked();
        void on_addActorButton_clicked();
        void on_delDirectorButton_clicked();
        void on_delProducerButton_clicked();
        void on_delActorButton_clicked();
        void on_directorEdit_textEdited();
        void on_producerEdit_textEdited();
        void on_actorEdit_textEdited();
        void peopleWindow_peopleCreated(People);


    private:
        Ui::MetadataWindow *m_ui;
        Movie m_movie;
        Application *m_app;
};

#endif // METADATAWINDOW_H
