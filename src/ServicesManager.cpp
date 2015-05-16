#include "ServicesManager.h"

Q_GLOBAL_STATIC(ServicesManager, servicesManager)

ServicesManager::ServicesManager(QObject *parent) : QObject(parent)
{
    m_databaseManager = new DatabaseManager;
    this->setMatchingMoviesList("");
}

ServicesManager *ServicesManager::instance()
{
    return servicesManager;
}

void ServicesManager::setMatchingMoviesList(QString pattern)
{
    m_matchingMoviesList = m_databaseManager->getMoviesByAny(pattern);
}

void ServicesManager::pannelsUpdate()
{
    emit requestPannelsUpdate();
}
