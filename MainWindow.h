#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "DatabaseManager.h"
#include "SettingsWindow.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QSqlQueryModel>
#include <QTableView>
#include <QPushButton>
#include <QtDebug>



class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:

private:
    QPushButton *settingsButton;
    QVBoxLayout *mainLayout;
    QTableView *moviesList;
};

#endif // MAINWINDOW_H
