#include <QApplication>
#include <QTime>
#include "MainWindow.h"
#include "DatabaseManager.h"

#include <QtDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow window;
    window.setFixedSize(800,600);

    DatabaseManager dbManager;
    QSqlQuery query = dbManager.getMovie("id", 1);

    while (query.next()) {
            int name = query.value(0).toInt();
            QString salary = query.value(1).toString();
            qDebug() << name << salary;
        }

    window.show();


    return app.exec();
}
