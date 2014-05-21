#include "SettingsWindow.h"
#include <QSettings>
#include <QCloseEvent>
SettingsWindow::SettingsWindow(QWidget *parent) : QWidget(parent)
{
    setFixedSize(400,300);
    setWindowTitle("Settings");
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QPushButton *submitButton = new QPushButton("Submit", this);
    QObject::connect(submitButton, SIGNAL(clicked()), this, SLOT(applySetting()));

    //mainLayout->addWidget(pathInput);
    mainLayout->addWidget(submitButton);
}

SettingsWindow::~SettingsWindow()
{
}

bool SettingsWindow::applySetting()
{
QSettings settings("tutu", "tto");
settings.setValue("FilePath", "toto");

    close();

    return true;
}

void SettingsWindow::closeEvent(QCloseEvent *event)
{
  emit closed();
  event->accept();
}
