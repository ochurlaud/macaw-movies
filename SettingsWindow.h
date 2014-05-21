#ifndef SETTINGWINDOW_H
#define SETTINGWINDOW_H

#include "DatabaseManager.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QPushButton>
#include <QtDebug>

class SettingsWindow : public QWidget
{
    Q_OBJECT

    public:
        explicit SettingsWindow(QWidget *parent = 0);
        ~SettingsWindow();
        void closeEvent(QCloseEvent *event);


    public slots:
        bool applySetting();

    signals:
        void closed();

    private:
        QVBoxLayout *mainLayout;
        QPushButton *submitButton;
};

#endif // SETTINGWINDOW_H
