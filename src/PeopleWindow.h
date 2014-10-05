#ifndef PEOPLEWINDOW_H
#define PEOPLEWINDOW_H

#include <QDialog>
#include "Application.h"

class Application;

namespace Ui {
class PeopleWindow;
}

class PeopleWindow : public QDialog
{
    Q_OBJECT

    public:
        explicit PeopleWindow(int = 0, QWidget *parent = 0);
        ~PeopleWindow();
        void setFirstname(QString);
        QString getFirstname();
        void setLastname(QString);
        QString getLastname();
        void setRealname(QString);
        QString getRealname();
        void setBirthday(QDate);
        QDate getBirthday();
        void setBiography(QString);
        QString getBiography();

    public slots:
        void on_validationButtons_accepted();

    signals:
        void peopleCreated(People);

    private:
        Ui::PeopleWindow *m_ui;
        People m_people;
        Application *m_app;
};

#endif // PEOPLEWINDOW_H
