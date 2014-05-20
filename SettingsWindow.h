#ifndef SETTINGWINDOW_H
#define SETTINGWINDOW_H

#include "DatabaseManager.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QtDebug>

class SettingsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = 0);
    ~SettingsWindow();

private:
    QVBoxLayout *mainLayout;
    QPushButton *submitButton;
};

#endif // SETTINGWINDOW_H
