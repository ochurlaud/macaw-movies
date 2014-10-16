#ifndef MainWindow_H
#define MainWindow_H

#include <QtGui>
#include <QtWidgets>
#include "Application.h"
#include "Entities/Movie.h"
#include "SettingsWindow.h"
#include "MetadataWindow.h"
#include "PeopleWindow.h"

enum typeElement {None, isMovie, isPeople, isTag};

class Application;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void fillMainPannel(QVector<Movie> moviesVector);
    void fillLeftPannel(int typeElement, int typePeople);
    ~MainWindow();

private slots:
    void on_actionEdit_Settings_triggered();
    void on_peopleBox_activated(int type);
    void on_playlistsButton_clicked();
    void on_toWatchButton_clicked();
    void on_tagsButton_clicked();
    void on_customContextMenuRequested(const QPoint &);
    void on_mainPannel_itemDoubleClicked(QTableWidgetItem *item);
    void on_leftPannel_clicked(const QModelIndex &index);
    void on_mainPannel_clicked(const QModelIndex &index);
    void on_actionEdit_leftPannelMetadata_triggered();
    void on_actionEdit_mainPannelMetadata_triggered();

private:
    Ui::MainWindow *m_ui;
    Application *m_app;
};

#endif // MainWindow_H
