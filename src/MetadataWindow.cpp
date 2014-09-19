#include "MetadataWindow.h"
#include "ui_MetadataWindow.h"

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

    QObject::connect(this, SIGNAL(accepted()), this, SLOT(updateMovie()));
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
    for (int i = 0 ; i < directorsVector.size() ; i++)
    {
        People l_director = directorsVector.at(i);
        QListWidgetItem *l_item = new QListWidgetItem(l_director.getFirstname() + " " + l_director.getLastname());
        l_item->setData(Qt::UserRole, l_director.getId());
        m_ui->directorsWidget->insertItem(i, l_item);
    }
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

void MetadataWindow::updateMovie()
{
    m_app->debug("[MetadataWindow] Enters updateMovie()");
    m_movie.setTitle(getTitle());
    m_movie.setOriginalTitle(getOriginalTitle());
    m_movie.setYear(getYear());
    m_movie.setCountry(getCountry());
    m_movie.setSynopsis(getSynopsis());

    m_app->getDatabaseManager()->updateMovie(m_movie);
    m_app->debug("[MetadataWindow] Exits updateMovie()");
}
