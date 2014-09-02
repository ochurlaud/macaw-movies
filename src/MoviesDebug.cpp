#include "MoviesDebug.h"

MoviesDebug::MoviesDebug()
{
}

void MoviesDebug::print(QString text)
{
    if (isDebug())
    {
        qDebug() << text;
    }
    // else do nothing
}
