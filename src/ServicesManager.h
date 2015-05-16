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
    QList<Movie> authorizedMoviesList() const { return m_authorizedMoviesList; }
    void setAuthorizedMoviesList(const QString pattern);
    bool toWatchState() const { return m_toWatchState; }
    void setToWatchState(const bool state) { m_toWatchState = state; }
    DatabaseManager* databaseManager() { return m_databaseManager; }

signals:

public slots:

private:
    QList<Movie> m_authorizedMoviesList;
    DatabaseManager *m_databaseManager;
    bool m_toWatchState;
};

#endif // SERVICESMANAGER_H
