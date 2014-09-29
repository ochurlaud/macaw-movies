#include "PeopleWindow.h"
#include "ui_PeopleWindow.h"

PeopleWindow::PeopleWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PeopleWindow)
{
    ui->setupUi(this);
}

PeopleWindow::~PeopleWindow()
{
    delete ui;
}
