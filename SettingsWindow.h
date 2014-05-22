#ifndef SETTINGWINDOW_H
#define SETTINGWINDOW_H

#include "Application.h"
#include "DatabaseManager.h"

#include <QCloseEvent>
#include <QFileDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QtDebug>

class SettingsWindow : public QWidget
{
    Q_OBJECT

    public:
        explicit SettingsWindow(QWidget *parent = 0);
        ~SettingsWindow();
        void closeEvent(QCloseEvent *event);


    public slots:
        void applySetting();
        void browseFilesPathDialog();

    signals:
        void closed();

    private:
        QLabel *filesPathLabel;
        QLineEdit *filesPathEdit;
        QPushButton *filesPathSearchButton;
        QPushButton *submitButton;
        QVBoxLayout *mainLayout;
        QHBoxLayout *filesPathLayout;

};

#endif // SETTINGWINDOW_H
