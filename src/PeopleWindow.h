#ifndef PEOPLEWINDOW_H
#define PEOPLEWINDOW_H

#include <QDialog>

namespace Ui {
class PeopleWindow;
}

class PeopleWindow : public QDialog
{
    Q_OBJECT

public:
    explicit PeopleWindow(QWidget *parent = 0);
    ~PeopleWindow();

private:
    Ui::PeopleWindow *ui;
};

#endif // PEOPLEWINDOW_H
