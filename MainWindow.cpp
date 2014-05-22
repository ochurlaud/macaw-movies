#include "MainWindow.h"


MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    setFixedSize(800,600);
    mainLayout = new QVBoxLayout(this);
    hLayout = new QHBoxLayout;
    DatabaseManager databaseManager;

    // leftPannel : list of movies or realisators or ...
    leftPannel = new QListWidget;
    QSqlQuery titlesRequest = databaseManager.getAllTitles();
    int i(0);
    while(titlesRequest.next())
    {
        QString title = titlesRequest.value(0).toString();
        moviesTitles.push_back(new QListWidgetItem(title));
        leftPannel->insertItem(i, moviesTitles[i]);
        i++;
    }
    leftPannel->setMaximumWidth(this->width()*.3);

    // movieList : central list. Would be great to be able to how it looks like
    moviesList = new QTableView;
    QSqlQueryModel *modelMoviesList = new QSqlQueryModel;
    modelMoviesList = databaseManager.createModel();
    modelMoviesList->setQuery(databaseManager.getAllMovies());
    moviesList->setModel(modelMoviesList);
    moviesList->setShowGrid(false);
    moviesList->verticalHeader()->hide();
    moviesList->setAlternatingRowColors(true);

    settingsButton = new QPushButton("Settings");
    QObject::connect(settingsButton, SIGNAL(clicked()), this, SLOT(showSettingsWindow()));

    mainLayout->addWidget(settingsButton);
    mainLayout->addLayout(hLayout);
    hLayout->addWidget(leftPannel);
    hLayout->addWidget(moviesList);
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
