#include <QApplication>
#include <QDir>
#include "MainWindow.h"
#include "DatabaseManager.h"
#include "Application.h"

#include <QtDebug>

int main(int argc, char *argv[])
{
    Application app(argc, argv);

    QDir directory("c:/Users/Olivier/Downloads/Movies");
    QFileInfoList list = directory.entryInfoList();
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        qDebug() << QString(fileInfo.fileName());
    }

    app.showMainWindow();
    return app.exec();
}
