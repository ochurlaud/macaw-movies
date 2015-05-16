#ifndef MOVIESPANNEL_H
#define MOVIESPANNEL_H

#include <QWidget>

#include "Application.h"

#include "MainWindowWidgets/MainPannel.h"

namespace Ui {
class MoviesPannel;
}

class MoviesPannel : public MainPannel
{
    Q_OBJECT

public:
    explicit MoviesPannel(QWidget *parent = 0);
    ~MoviesPannel();
    void fill(const QList<Movie> &moviesList);

private slots:
    void on_customContextMenuRequested(const QPoint &point);
    void on_actionEdit_mainPannelMetadata_triggered();
    void on_actionDelete_triggered();
    void on_tableWidget_itemDoubleClicked(QTableWidgetItem *item);
    void on_tableWidget_itemSelectionChanged();
    void addPlaylistMenu_triggered(QAction* action);

private:
    Ui::MoviesPannel *m_ui;
    void setHeaders();
    void addMovieToPannel(const Movie &movie);
    void removeMovieFromPlaylist(const QList<Movie> &movieList, Playlist &playlist);
    bool permanentlyDeleteFile(QFile *movieFileToDelete);
    bool moveFileToTrash(QList<Movie> &movieList);
    QString unix_findTrashFolder(QString movieFilePath);
    bool linux_moveFileToTrash(QString movieFilePath);
    bool windows_moveFileToTrash(QString movieFilePath);
    bool macosx_moveFileToTrash(QString movieFilePath);
};

#endif // MOVIESPANNEL_H
