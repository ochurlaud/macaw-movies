#ifndef PATHFORMOVIES_H
#define PATHFORMOVIES_H

#include <QString>

#include "enumerations.h"

class PathForMovies
{
public:
    PathForMovies();
    int id() const;
    void setId(const int id);
    QString path() const;
    void setPath(const QString path);
    bool hasMovies() const;
    void setMovies(const bool movies);
    bool hasShows() const;
    void setShows(const bool shows);

private:
    int m_id;
    QString m_path;
    int m_type;



};

#endif // PATHFORMOVIES_H
