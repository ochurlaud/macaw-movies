#include "MetadataFetcher.h"

MetadataFetcher::MetadataFetcher(QObject *parent) :
    QObject(parent)
{
    m_networkManager = new QNetworkAccessManager(this);

}

void MetadataFetcher::checkConnection(QString title)
{
    QNetworkRequest l_request;
    l_request.setUrl(QUrl("http://www.omdbapi.com/?t=then&y=2000&r="));

    m_reply = m_networkManager->get(l_request);
    QObject::connect(m_reply, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
    /*connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotError(QNetworkReply::NetworkError)));
    connect(m_reply, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(slotSslErrors(QList<QSslError>)));*/
}

void MetadataFetcher::slotReadyRead()
{
    QByteArray newData = m_reply->read(2048);

    qDebug() << newData << endl;
}
