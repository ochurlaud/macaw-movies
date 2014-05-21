#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    setFixedSize(800,600);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    DatabaseManager databaseManager;
    QSqlQueryModel *model = databaseManager.createModel();
    model->setQuery(databaseManager.getAllMovies());
    QTableView *moviesList = new QTableView;
    moviesList->setModel(model);

    QPushButton *settingsButton = new QPushButton("Settings", this);
    QObject::connect(settingsButton, SIGNAL(clicked()), this, SLOT(showSettingsWindow()));
    QLabel *label = new QLabel("Application::getFilesPath()", this);

    mainLayout->addWidget(settingsButton);
    mainLayout->addWidget(moviesList);
    mainLayout->addWidget(label);
    Application *app = qobject_cast<Application *>(qApp);
    qDebug() << app->getFilesPath();
}

MainWindow::~MainWindow()
{
}

void MainWindow::showSettingsWindow()
{
    SettingsWindow *settingsWindow = new SettingsWindow;
    settingsWindow->show();
    QObject::connect(settingsWindow,SIGNAL(closed()), this, SLOT(update()));

}
