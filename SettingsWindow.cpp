#include "SettingsWindow.h"

SettingsWindow::SettingsWindow(QWidget *parent) : QWidget(parent)
{
    setFixedSize(400,300);
    setWindowTitle("Settings");
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QPushButton *submitButton = new QPushButton("Submit", this);
    QObject::connect(submitButton, SIGNAL(clicked()), this->parentWidget()->parentWidget(), SLOT(setDodo()));

    //mainLayout->addWidget(pathInput);
    mainLayout->addWidget(submitButton);
}

SettingsWindow::~SettingsWindow()
{
}
