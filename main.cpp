#include <QApplication>
#include <QWidget>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QFile>

//QFileSystemWatcher
//QMetaDataReaderControl <--  N'existe pas
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget window;
    window.setFixedSize(800,600);


    window.show();

    return app.exec();
}
