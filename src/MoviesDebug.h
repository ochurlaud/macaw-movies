#ifndef MOVIESDEBUG_H
#define MOVIESDEBUG_H

#include <QtGui>
#include <QtWidgets>

#include <QtDebug>

class MoviesDebug : public QObject
{
public:
    MoviesDebug();
    void print(QString);
    void setDebug(bool value) { m_debug = value; }
    bool isDebug() { return m_debug; }

private:
    bool m_debug;
};

#endif // MOVIESDEBUG_H
