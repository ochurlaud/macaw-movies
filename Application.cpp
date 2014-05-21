#include "Application.h"

Application::Application(int argc, char **argv) : QApplication(argc, argv)
{
    filesPath="totor";
 //   mainWindow.setFixedSize(800,600);
}

Application::~Application()
{
}

/*void Application::showMainWindow()
{
    mainWindow.show();
}*/

bool Application::setFilesPath(QString path)
{
    filesPath = path;
    return true;
}
QString Application::getFilesPath()
{
    return filesPath;
}

