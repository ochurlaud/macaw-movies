#include <QApplication>
#include <QDir>
#include "MainWindow.h"
#include "DatabaseManager.h"
#include "Application.h"

#include <QtDebug>

int main(int argc, char *argv[])
{
    Application l_app(argc, argv);
    MainWindow l_mainWindow;

    QDir l_directory("c:/Users/Olivier/Downloads/Movies");
    QFileInfoList l_list = l_directory.entryInfoList();
    for (int i = 0; i < l_list.size(); ++i) {
        QFileInfo l_fileInfo = l_list.at(i);
        qDebug() << QString(l_fileInfo.fileName());
    }

    l_mainWindow.show();

    return l_app.exec();
}
