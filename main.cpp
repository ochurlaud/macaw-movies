#include <QApplication>
#include <QDir>
#include "MainWindow.h"
#include "DatabaseManager.h"
#include "Application.h"

#include <QtDebug>

int main(int argc, char *argv[])
{
    Application app(argc, argv);
    MainWindow mainWindow;

    QDir directory("c:/Users/Olivier/Downloads/Movies");
    QFileInfoList list = directory.entryInfoList();
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        qDebug() << QString(fileInfo.fileName());
    }

    mainWindow.show();

    return app.exec();
}
