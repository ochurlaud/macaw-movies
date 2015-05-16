#include "ServicesManager.h"

Q_GLOBAL_STATIC(ServicesManager, servicesManager)

ServicesManager::ServicesManager(QObject *parent) : QObject(parent)
{
    m_databaseManager = new DatabaseManager;
    this->setAuthorizedMoviesList("");
}

ServicesManager *ServicesManager::instance()
{
    return servicesManager;
}

void ServicesManager::setAuthorizedMoviesList(QString pattern)
{
    m_authorizedMoviesList = m_databaseManager->getMoviesByAny(pattern);
}
