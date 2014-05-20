#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include "DatabaseManager.h"
#include "MainWindow.h"
#include "SettingsWindow.h"

class Application : public QApplication
{
    public:
        Application(int, char**);
        ~Application();
        void showMainWindow();

    public slots:
        void setDodo(){qDebug()<< "dodo";}
        void showSettingsWindow();


    private:
        MainWindow mainWindow;
};

#endif // APPLICATION_H
