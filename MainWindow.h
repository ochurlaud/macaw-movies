#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "DatabaseManager.h"
#include "SettingsWindow.h"
#include "Application.h"

#include <QListWidget>
#include <QVBoxLayout>
#include <QSqlQueryModel>
#include <QTableView>
#include <QHeaderView>
#include <QTextEdit>
#include <QPushButton>
#include <QtDebug>



class MainWindow : public QWidget
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    public slots:
        void setDodo(){qDebug()<< "dodo";}
        void showSettingsWindow();

    private:
        QPushButton *settingsButton;
        QHBoxLayout *hLayout;
        QVBoxLayout *mainLayout;
        QTextEdit *searchEdit;
        QListWidget *leftPannel;
        QTableView *moviesList;
        QVector<QListWidgetItem*> moviesTitles;
};

#endif // MAINWINDOW_H
