#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include "MainWindow.h"

class Application : public QApplication
{
    Q_OBJECT

    public:
        Application(int, char**);
        ~Application();
        void showMainWindow();
        bool setFilesPath(QString path);
        QString getFilesPath();

    private:
    //    MainWindow mainWindow;
        QString m_filesPath;
};

#endif // APPLICATION_H
