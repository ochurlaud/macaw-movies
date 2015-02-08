#include "SettingsWindow2.h"
#include "ui_SettingsWindow2.h"

SettingsWindow2::SettingsWindow2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsWindow2)
{
    ui->setupUi(this);
}

SettingsWindow2::~SettingsWindow2()
{
    delete ui;
}
