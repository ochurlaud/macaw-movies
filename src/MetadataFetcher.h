#ifndef METADATAFETCHER_H
#define METADATAFETCHER_H

#include <QObject>
#include <QtNetwork>
#include <QtXml>
#include <Application.h>

class MetadataFetcher : public QObject
{
    Q_OBJECT

    public:
        explicit MetadataFetcher(Movie movie, QObject *parent = 0);
        void fetchMetadata(QString title);

    signals:
        void movieHydrated(Movie&);

    public slots:
        void replyRelatedMovies(QNetworkReply *reply);
        void replyHydrateMovie(QNetworkReply *reply);
        bool updateMovieInDatabase(Movie &movie);

    private:
        QNetworkAccessManager *m_networkManager;
        Application *m_app;
        Movie m_movie;
        void getRelatedMovies(QString title);
        void getMetadata(QString imdbID);

};

#endif // METADATAFETCHER_H
