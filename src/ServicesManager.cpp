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

/**
 * @brief send a signal to show provided message in MainWindow statusbar
 * The signal shoul be connected to the proper action in MainWindow
 * the time parameter is optional if not given time = 0
 * @author Sébastien TOUZÉ <sebastien.touze@yahoo.fr>
 *
 * @param QString message
 * @param int time how long (in milliseconds) message shoud be showed.
 * If not givent time=0 and the message will persist until a new message is sent.
 */
void ServicesManager::showTempStatusBarMessage(QString message, int time)
{
    emit requestTempStatusBarMessage(message, time);
}
