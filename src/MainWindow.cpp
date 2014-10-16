#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow)
{
    m_app = qobject_cast<Application *>(qApp);
    m_app->debug("[MainWindow] Constructor called");

    m_ui->setupUi(this);
    this->setWindowTitle(m_app->getAppName());
    this->setWindowIcon(m_app->getAppIcon());

    connect(m_ui->mainPannel, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(on_customContextMenuRequested(const QPoint &)));
    connect(m_ui->leftPannel, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(on_customContextMenuRequested(const QPoint &)));

    QVector<Movie> l_moviesVector = m_app->getDatabaseManager()->getAllMovies();
    fillMainPannel(l_moviesVector);
    fillLeftPannel(isPeople, Director);
    m_app->debug("[MainWindow] Construction done");
}

MainWindow::~MainWindow()
{
    delete m_ui;
    m_app->debug("[MainWindow] Destructed");
}

/**
 * @brief Call and shows the settings window.
 * Calls the SettingsWindow class and waits until it closes to handle the results
 */
void MainWindow::on_actionEdit_Settings_triggered()
{
    m_app->debug("[MainWindow] Enters showSettingsWindow()");
    SettingsWindow *l_settingsWindow = new SettingsWindow;
    l_settingsWindow->setModal(true);
    l_settingsWindow->setAttribute(Qt::WA_DeleteOnClose);
    l_settingsWindow->show();
    QObject::connect(l_settingsWindow,SIGNAL(closeAndSave()), this, SLOT(updateApp()));
    m_app->debug("[MainWindow] Exits showSettingsWindow()");
}

void MainWindow::fillLeftPannel(int typeElement, int typePeople = 0)
{
    m_ui->leftPannel->clear();

    QListWidgetItem *l_item = new QListWidgetItem("All");
    l_item->setData(Qt::UserRole, 0);
    m_ui->leftPannel->addItem(l_item);

    if (typeElement == isPeople)
    {
        QListWidgetItem *l_item = new QListWidgetItem("Unknown");
        l_item->setData(Qt::UserRole, -1);
        l_item->setData(Qt::UserRole+1, isPeople);
        m_ui->leftPannel->addItem(l_item);

        QVector<People> l_peopleVector = m_app->getDatabaseManager()->getAllPeople(typePeople);
        foreach (People l_people, l_peopleVector)
        {
            QString l_name(l_people.getLastname());
            if (l_people.getFirstname() != "")
            {
                l_name = l_name + ", " + l_people.getFirstname();
            }
            l_name = l_name  + " (" + QString::number(l_people.getBirthday().year()) + ")";

            QListWidgetItem *l_item = new QListWidgetItem(l_name);
            l_item->setData(Qt::UserRole, l_people.getId());
            l_item->setData(Qt::UserRole+1, isPeople);
            l_item->setData(Qt::UserRole+2, typePeople);

            m_ui->leftPannel->addItem(l_item);
            switch (typePeople)
            {
            case Director:
                m_ui->leftPannelLabel->setText("Director");
                break;
            case Producer:
                m_ui->leftPannelLabel->setText("Producer");
                break;
            case Actor:
                m_ui->leftPannelLabel->setText("Actor");
                break;
            }
        }
    }
    else if(typeElement == isTag)
    {
        QListWidgetItem *l_item = new QListWidgetItem("No Tag");
        l_item->setData(Qt::UserRole, -1);
        l_item->setData(Qt::UserRole+1, isTag);
        m_ui->leftPannel->addItem(l_item);

        QVector<Tag> l_tagsVector = m_app->getDatabaseManager()->getAllTags();
        foreach (Tag l_tag, l_tagsVector)
        {
            QString l_name(l_tag.getName());

            QListWidgetItem *l_item = new QListWidgetItem(l_name);
            l_item->setData(Qt::UserRole, l_tag.getId());
            l_item->setData(Qt::UserRole+1, isTag);

            m_ui->leftPannel->addItem(l_item);
        }
        m_ui->leftPannelLabel->setText("Tags");
    }
}

void MainWindow::fillMainPannel(QVector<Movie> moviesVector)
{
    m_app->debug("[MainWindow] Enters fillMainPannel()");

    m_ui->mainPannel->clear();
    m_ui->mainPannel->setColumnCount(4);

    QStringList l_headers;
    l_headers << "Title" << "Original Title" << "Release Date" << "Path of the file";
    m_ui->mainPannel->setHorizontalHeaderLabels(l_headers);
    m_ui->mainPannel->setRowCount(moviesVector.size());
    int l_row = 0;

    foreach (Movie l_movie, moviesVector)
    {
        int l_column = 0;
        QStringList l_movieData;
        l_movieData << l_movie.getTitle()
                    << l_movie.getOriginalTitle()
                    << l_movie.getReleaseDate().toString("dd MMM yyyy")
                    << l_movie.getFilePath();
        QVector<QTableWidgetItem*> l_itemsVector(4);
        foreach(QTableWidgetItem *l_item, l_itemsVector)
        {
            l_item = new QTableWidgetItem(l_movieData.at(l_column));
            l_item->setData(Qt::UserRole, l_movie.getId());
            l_item->setData(Qt::UserRole+1, isMovie);
            m_ui->mainPannel->setItem(l_row, l_column, l_item);
            l_column++;
        }
        l_row++;
    }

    m_app->debug("[MainWindow] Exits fillMainPannel()");
}

void MainWindow::on_peopleBox_activated(int type)
{
    m_app->debug("[MainWindow] Enters on_peopleBox_activated()");

    fillLeftPannel(isPeople, type);
}

void MainWindow::on_playlistsButton_clicked()
{

}

void MainWindow::on_toWatchButton_clicked()
{

}

void MainWindow::on_tagsButton_clicked()
{
    m_app->debug("[MainWindow] tagsButton clicked");
    fillLeftPannel(isTag);
}

void MainWindow::on_customContextMenuRequested(const QPoint &point)
{
    m_app->debug("[MainWindow] customContextMenuRequested()");
    QMenu *l_menu = new QMenu(this);
    if(m_ui->leftPannel->indexAt(point).isValid())
    {
        l_menu->addAction(m_ui->actionEdit_leftPannelMetadata);
        l_menu->exec(m_ui->leftPannel->mapToGlobal(point));
    }
    else if (m_ui->mainPannel->indexAt(point).isValid())
    {
        l_menu->addAction(m_ui->actionEdit_mainPannelMetadata);
        l_menu->exec(m_ui->mainPannel->mapToGlobal(point));
    }
}


void MainWindow::on_actionEdit_mainPannelMetadata_triggered()
{
    m_app->debug("[MainWindow] actionEdit_Metadata_triggered()");
    int l_id = m_ui->mainPannel->selectedItems().at(0)->data(Qt::UserRole).toInt();

    MetadataWindow *l_metadataWindow = new MetadataWindow(l_id);
    l_metadataWindow->show();
}

void MainWindow::on_actionEdit_leftPannelMetadata_triggered()
{
    int l_id = m_ui->leftPannel->selectedItems().at(0)->data(Qt::UserRole).toInt();

    // It's editable only if id is not 0
    if(l_id != 0)
    {
        int l_typeElement = m_ui->leftPannel->selectedItems().at(0)->data(Qt::UserRole+1).toInt();
        if (l_typeElement == isPeople)
        {
            PeopleWindow *l_metadataWindow = new PeopleWindow(l_id);
            l_metadataWindow->show();
        }
        else if (l_typeElement == isTag)
        {
            qDebug() << "Tag !";
        }
    }
}

void MainWindow::on_mainPannel_itemDoubleClicked(QTableWidgetItem *item)
{
    m_app->debug("[MainWindow] itemDoubleClicked on mainPannel()");

    int l_movieId = item->data(Qt::UserRole).toInt();
    Movie l_movie = m_app->getDatabaseManager()->getOneMovieById(l_movieId);

    m_app->debug("[MainWindow.startMovie()] Opened movie: " + l_movie.getFilePath());

    QDesktopServices::openUrl(QUrl("file://" + l_movie.getFilePath(), QUrl::TolerantMode));
}

void MainWindow::on_leftPannel_clicked(const QModelIndex &index)
{
    QVector<Movie> l_moviesVector;
    int l_id = index.data(Qt::UserRole).toInt();
    int l_typeElement = index.data(Qt::UserRole+1).toInt();
    if(l_id == 0)
    {
        l_moviesVector = m_app->getDatabaseManager()->getAllMovies();
    }
    else if(l_typeElement == isPeople)
    {
        int l_typePeople = index.data(Qt::UserRole+2).toInt();

        if (l_id == -1)
        {
            l_moviesVector = m_app->getDatabaseManager()->getMoviesWithoutPeople(l_typePeople);
        }
        else
        {
            People l_people = m_app->getDatabaseManager()->getOnePeopleById(l_id, l_typePeople);
            l_moviesVector = m_app->getDatabaseManager()->getMoviesByPeople(l_people, l_typePeople);
        }
    }
    else if (l_typeElement == isTag)
    {
        if (l_id == -1)
        {
            l_moviesVector = m_app->getDatabaseManager()->getMoviesWithoutTag();
        }
        else
        {
            Tag l_tag = m_app->getDatabaseManager()->getOneTagById(l_id);
            l_moviesVector = m_app->getDatabaseManager()->getMoviesByTag(l_tag);
        }
    }
    fillMainPannel(l_moviesVector);
}

void MainWindow::on_mainPannel_clicked(const QModelIndex &index)
{
    m_app->debug("[MainWindow] mainPannel clicked");
    m_ui->mainPannel->selectRow(index.row());
}
