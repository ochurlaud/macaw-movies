#ifndef PATHFORMOVIES_H
#define PATHFORMOVIES_H

#include <QString>

class PathForMovies
{
public:
    PathForMovies();
    int id() const;
    void setId(const int id);
    QString path() const;
    void setPath(const QString path);

private:
    int m_id;
    QString m_path;


};

#endif // PATHFORMOVIES_H
