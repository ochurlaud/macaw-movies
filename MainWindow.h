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
        QPushButton *m_settingsButton;
        QHBoxLayout *m_hLayout;
        QVBoxLayout *m_mainLayout;
        QTextEdit *m_searchEdit;
        QListWidget *m_leftPannel;
        QTableView *m_moviesList;
        QVector<QListWidgetItem*> m_moviesTitles;
};

#endif // MAINWINDOW_H
