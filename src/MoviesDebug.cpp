#include "MoviesDebug.h"

MoviesDebug::MoviesDebug()
{
}

void MoviesDebug::print(QString text)
{
    if (isDebug())
    {
        qDebug() << qPrintable(text);
    }
    // else do nothing
}
