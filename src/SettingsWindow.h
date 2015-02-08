#ifndef SETTINGSWINDOW2_H
#define SETTINGSWINDOW2_H

#include <QDialog>

namespace Ui {
class SettingsWindow2;
}

class SettingsWindow2 : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsWindow2(QWidget *parent = 0);
    ~SettingsWindow2();

private:
    Ui::SettingsWindow2 *ui;
};

#endif // SETTINGSWINDOW2_H
