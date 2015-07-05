#ifndef SERVICESMANAGER_H
#define SERVICESMANAGER_H

#include <QWidget>

#include "DatabaseManager.h"
#include "Entities/Movie.h"

class ServicesManager : public QObject
{
    Q_OBJECT
public:
    explicit ServicesManager(QObject *parent = 0);
    static ServicesManager* instance();
    QList<Movie> matchingMoviesList() const { return m_matchingMoviesList; }
    void setMatchingMoviesList(const QString pattern);
    bool toWatchState() const { return m_toWatchState; }
    void setToWatchState(const bool state) { m_toWatchState = state; }
    DatabaseManager* databaseManager() { return m_databaseManager; }


signals:
    void requestPannelsUpdate();
    void requestTempStatusBarMessage(QString message, int time = 0);

public slots:
    void pannelsUpdate();
    void showTempStatusBarMessage(QString message, int time);

private:
    QList<Movie> m_matchingMoviesList;
    DatabaseManager *m_databaseManager;
    bool m_toWatchState;
};

#endif // SERVICESMANAGER_H
