#include "PeopleWindow.h"
#include "ui_PeopleWindow.h"

PeopleWindow::PeopleWindow(int id, QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::PeopleWindow)
{
    m_app = qobject_cast<Application *>(qApp);
    m_app->debug("[PeopleWindow] Constructor called");

    m_ui->setupUi(this);

    // If id is 0 it means we create a new People
    if (id != 0)
    {
        m_people = m_app->getDatabaseManager()->getOnePeopleById(id);
        setFirstname(m_people.getFirstname());
        setLastname(m_people.getLastname());
        setRealname(m_people.getRealname());
        setBirthday(m_people.getBirthday());
        setBiography(m_people.getBiography());
    }
    m_app->debug("[PeopleWindow] Construction done");
}

PeopleWindow::~PeopleWindow()
{
    delete m_ui;
}

void PeopleWindow::setFirstname(QString firstname)
{
    m_ui->firstnameEdit->setText(firstname);
}

QString PeopleWindow::getFirstname()
{
    return m_ui->firstnameEdit->text();
}

void PeopleWindow::setLastname(QString lastname)
{
    m_ui->lastnameEdit->setText(lastname);
}

QString PeopleWindow::getLastname()
{
    return m_ui->lastnameEdit->text();
}

void PeopleWindow::setRealname(QString realname)
{
    m_ui->realnameEdit->setText(realname);
}

QString PeopleWindow::getRealname()
{
    return m_ui->realnameEdit->text();
}

void PeopleWindow::setBirthday(QDate birthday)
{
    m_ui->birthdayEdit->setDate(birthday);
}

QDate PeopleWindow::getBirthday()
{
    return m_ui->birthdayEdit->date();
}

void PeopleWindow::setBiography(QString biography)
{
    m_ui->biographyEdit->setPlainText(biography);
}

QString PeopleWindow::getBiography()
{
    return m_ui->biographyEdit->toPlainText();
}

void PeopleWindow::on_validationButtons_accepted()
{
    m_app->debug("[PeopleWindow] validationButtons accepted");
    m_people.setFirstname(getFirstname());
    m_people.setLastname(getLastname());
    m_people.setRealname(getRealname());
    m_people.setBirthday(getBirthday());
    m_people.setBiography(getBiography());
    emit(peopleCreated(m_people));

    m_app->debug("[PeopleWindow] validationButtons method done");
}
