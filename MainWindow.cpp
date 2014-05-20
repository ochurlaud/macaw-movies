#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    DatabaseManager databaseManager;
    QSqlQueryModel *model = databaseManager.createModel();
    model->setQuery(databaseManager.getAllMovies());
    QTableView *moviesList = new QTableView;
    moviesList->setModel(model);

    QPushButton *settingsButton = new QPushButton("Settings", this);
    QObject::connect(settingsButton, SIGNAL(clicked()), this->parent(), SLOT(showSettingsWindow()));
    mainLayout->addWidget(settingsButton);
    mainLayout->addWidget(moviesList);

}



MainWindow::~MainWindow()
{
}
