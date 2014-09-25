#include "MetadataWindow.h"
#include "ui_MetadataWindow.h"

/* @TODO:
 *   - While typing in a peopleEdit, propose existing names. If none: pop-up "Do you want to create ?"
 *   - Add actions to + and - buttons
 *   - Fill the ListWidget with the people added
 *   - Right-click on a ListWidgetItem allow to edit it the people
 *   - Handle tags
 */

MetadataWindow::MetadataWindow(int id, QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::MetadataWindow)
{
    m_app = qobject_cast<Application *>(qApp);
    m_app->debug("[MetadataWindow] Constructor called");
    m_movie = m_app->getDatabaseManager()->getOneMovieById(id);
    m_ui->setupUi(this);
    setTitle(m_movie.getTitle());
    setOriginalTitle(m_movie.getOriginalTitle());
    setYear(m_movie.getYear());
    setCountry(m_movie.getCountry());
    setSynopsis(m_movie.getSynopsis());
    setDirectors(m_movie.getDirectors());
    setActors(m_movie.getActors());
    setProducers(m_movie.getProducers());
    m_app->debug("[MetadataWindow] Construction done");
}

MetadataWindow::~MetadataWindow()
{
    delete m_ui;
}

void MetadataWindow::setTitle(QString title)
{
    m_ui->titleEdit->setText(title);
}

QString MetadataWindow::getTitle()
{
    return m_ui->titleEdit->text();
}

void MetadataWindow::setOriginalTitle(QString originalTitle)
{
    m_ui->originalTitleEdit->setText(originalTitle);
}

QString MetadataWindow::getOriginalTitle()
{
    return m_ui->originalTitleEdit->text();
}

void MetadataWindow::setYear(int year)
{
    m_ui->yearEdit->setValue(year);
}

int MetadataWindow::getYear()
{
    return m_ui->yearEdit->value();
}

void MetadataWindow::setCountry(QString country)
{
    m_ui->countryEdit->setText(country);
}

QString MetadataWindow::getCountry()
{
    return m_ui->countryEdit->text();
}

void MetadataWindow::setSynopsis(QString synopsis)
{
    m_ui->synopsisEdit->setPlainText(synopsis);
}

QString MetadataWindow::getSynopsis()
{
    return m_ui->synopsisEdit->toPlainText();
}

void MetadataWindow::setDirectors(QVector<People> directorsVector)
{
    People l_director;
    foreach(l_director, directorsVector)
    {
        QListWidgetItem *l_item = new QListWidgetItem(l_director.getFirstname() + " " + l_director.getLastname());
        l_item->setData(Qt::UserRole, l_director.getId());
        m_ui->directorsWidget->addItem(l_item);
    }
}

QVector<People> MetadataWindow::getDirectors()
{
    QVector<People> l_directorsVector;
    for (int i = 0 ; i < m_ui->directorsWidget->count() ; i++)
    {
        int l_id = m_ui->directorsWidget->item(i)->data(Qt::UserRole).toInt();
        People l_director = m_app->getDatabaseManager()->getOneDirectorById(l_id);
        l_directorsVector.push_back(l_director);
    }

    return l_directorsVector;
}

void MetadataWindow::addDirector(People director)
{
    QListWidgetItem *l_item = new QListWidgetItem(director.getFirstname() + " " + director.getLastname());
    l_item->setData(Qt::UserRole, director.getId());
    m_ui->directorsWidget->addItem(l_item);
}

void MetadataWindow::delDirector(int directorId)
{

}

void MetadataWindow::setProducers(QVector<People> producersVector)
{
    for (int i = 0 ; i < producersVector.size() ; i++)
    {
        People l_producer = producersVector.at(i);
        QListWidgetItem *l_item = new QListWidgetItem(l_producer.getFirstname() + " " + l_producer.getLastname());
        l_item->setData(Qt::UserRole, l_producer.getId());
        m_ui->producersWidget->insertItem(i, l_item);
    }
}


QVector<People> MetadataWindow::getProducers()
{
    QVector<People> l_producersVector;
    for (int i = 0 ; i < m_ui->producersWidget->count() ; i++)
    {
        int l_id = m_ui->producersWidget->item(i)->data(Qt::UserRole).toInt();
        People l_producer = m_app->getDatabaseManager()->getOneProducerById(l_id);
        l_producersVector.push_back(l_producer);
    }

    return l_producersVector;
}

void MetadataWindow::setActors(QVector<People> actorsVector)
{
    for (int i = 0 ; i < actorsVector.size() ; i++)
    {
        People l_actor = actorsVector.at(i);
        QListWidgetItem *l_item = new QListWidgetItem(l_actor.getFirstname() + " " + l_actor.getLastname());
        l_item->setData(Qt::UserRole, l_actor.getId());
        m_ui->actorsWidget->insertItem(i, l_item);
    }
}


QVector<People> MetadataWindow::getActors()
{
    QVector<People> l_actorsVector;
    for (int i = 0 ; i < m_ui->actorsWidget->count() ; i++)
    {
        int l_id = m_ui->actorsWidget->item(i)->data(Qt::UserRole).toInt();
        People l_actor = m_app->getDatabaseManager()->getOneActorById(l_id);
        l_actorsVector.push_back(l_actor);
    }

    return l_actorsVector;
}


void MetadataWindow::on_validationButtons_accepted()
{
    m_app->debug("[MetadataWindow] validationButtons accepted");
    m_movie.setTitle(getTitle());
    m_movie.setOriginalTitle(getOriginalTitle());
    m_movie.setYear(getYear());
    m_movie.setCountry(getCountry());
    m_movie.setSynopsis(getSynopsis());
    m_movie.setDirectors(getDirectors());
    m_movie.setProducers(getProducers());
    m_movie.setActors(getActors());

    m_app->getDatabaseManager()->updateMovie(m_movie);
    m_app->debug("[MetadataWindow] validationButtons method done");
}

void MetadataWindow::on_addDirectorButton_clicked()
{
    m_app->debug("[MetadataWindow] addDirectorButton clicked()");
    QString l_text = m_ui->directorEdit->text();
    QVector<People> l_directorsVector = m_app->getDatabaseManager()->getPeopleByFullname(l_text, 1);
    if (l_directorsVector.size() == 1)
    {
        People l_director = l_directorsVector.at(0);
        addDirector(l_director);
        m_app->debug(l_text + " added");
    }
    else
    {
        // Open a window with new director, firstname/lastname already filled
    }
}

void MetadataWindow::on_addProducerButton_clicked()
{
    m_app->debug("[MetadataWindow] addProducerButton clicked()");
}

void MetadataWindow::on_addActorButton_clicked()
{
    m_app->debug("[MetadataWindow] addActorButton clicked()");
}

void MetadataWindow::on_delDirectorButton_clicked()
{
    m_app->debug("[MetadataWindow] delDirectorButton clicked()");
    QList<QListWidgetItem*> l_itemsListToDelete = m_ui->directorsWidget->selectedItems();
    QListWidgetItem *l_itemToDelete = new QListWidgetItem;
    foreach (l_itemToDelete,l_itemsListToDelete)
    {
        m_ui->directorsWidget->removeItemWidget(l_itemToDelete);
    }
}

void MetadataWindow::on_delProducerButton_clicked()
{
    m_app->debug("[MetadataWindow] delProducerButton clicked()");
}

void MetadataWindow::on_delActorButton_clicked()
{
    m_app->debug("[MetadataWindow] delActorButton clicked()");
}

void MetadataWindow::on_directorEdit_textEdited()
{
    m_app->debug("[MetadataWindow] directorEdit textEdited()");
    QString l_text =  m_ui->directorEdit->text();
    if (l_text.size() > 3)
    {
        QVector<People> l_directorsVector = m_app->getDatabaseManager()->getPeopleByFullname(l_text, 1);
        if(l_directorsVector.size() > 0)
        {
            People l_director;
            QStringList l_propositions;
            foreach (l_director, l_directorsVector)
            {
                l_propositions << l_director.getFirstname() + " " + l_director.getLastname();
            }
            QCompleter *l_completer = new QCompleter(l_propositions);
            l_completer->setCaseSensitivity(Qt::CaseInsensitive);
            l_completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
            m_ui->directorEdit->setCompleter(l_completer);
         }
    }
}

void MetadataWindow::on_producerEdit_textEdited()
{
    m_app->debug("[MetadataWindow] producerEdit textEdited()");
}

void MetadataWindow::on_actorEdit_textEdited()
{
    m_app->debug("[MetadataWindow] actorEdit textEdited()");
}
