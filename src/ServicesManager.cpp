#include "ServicesManager.h"

Q_GLOBAL_STATIC(ServicesManager, servicesManager)

ServicesManager::ServicesManager(QObject *parent) : QObject(parent)
{
    m_authorizedMoviesList = m_databaseManager.getMoviesByAny("");
}

ServicesManager *ServicesManager::instance()
{
    return servicesManager;
}

void ServicesManager::setAuthorizedMoviesList(QString pattern)
{
    m_authorizedMoviesList = m_databaseManager.getMoviesByAny(pattern);
}
