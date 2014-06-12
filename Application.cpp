#include "Application.h"

Application::Application(int argc, char **argv) : QApplication(argc, argv)
{
    m_filesPath="totor";
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
    m_filesPath = path;
    return true;
}
QString Application::getFilesPath()
{
    return m_filesPath;
}

