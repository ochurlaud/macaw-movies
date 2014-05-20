#include "Application.h"

Application::Application(int argc, char **argv) : QApplication(argc, argv)
{
    mainWindow.setFixedSize(800,600);
}

Application::~Application()
{
}

void Application::showMainWindow()
{
    mainWindow.show();
}

void Application::showSettingsWindow()
{
    SettingsWindow *settingsWindow = new SettingsWindow;
    settingsWindow->show();
}
