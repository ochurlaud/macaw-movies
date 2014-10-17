#ifndef METADATAFETCHER_H
#define METADATAFETCHER_H

#include <QObject>
#include <QtNetwork>

class MetadataFetcher : public QObject
{
    Q_OBJECT

    public:
        explicit MetadataFetcher(QObject *parent = 0);
        void checkConnection(QString title);

    signals:

    public slots:
        void slotReadyRead();

    private:
        QNetworkAccessManager *m_networkManager;
        QNetworkReply *m_reply;

};

#endif // METADATAFETCHER_H
