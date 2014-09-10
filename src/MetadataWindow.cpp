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

void MetadataWindow::updateMovie()
{
    m_movie.setTitle(getTitle());
    m_movie.setOriginalTitle(getOriginalTitle());
    m_movie.setYear(getYear());
    m_movie.setCountry(getCountry());
    m_movie.setSynopsis(getSynopsis());

    m_app->getDatabaseManager()->updateMovie(m_movie);
}
