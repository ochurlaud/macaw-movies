#include "MetadataPannel.h"
#include "ui_MetadataPannel.h"

MetadataPannel::MetadataPannel(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::MetadataPannel)
{
    m_ui->setupUi(this);
}

MetadataPannel::~MetadataPannel()
{
    delete m_ui;
}

/**
 * @brief Fill the MetadataPannel with the data of the given Movie
 *
 * @author Olivier CHURLAUD <olivier@churlaud.com>
 * @author Sébastien TOUZE
 * @param movie
 */
void MetadataPannel::fill(const Movie &movie)
{
    Macaw::DEBUG_IN("[MetadataPannel] Enters fill()");
    QString l_title = "<html>"+movie.title()+"<br />";
    QString l_originalTitle = "<i>"+movie.originalTitle()+"</i></br /><br />";
    QString l_directors = tr("<i>Directed by</i><br />");
    QString l_producers = tr("<i>Produced by</i><br />");
    QString l_actors = tr("<i>With</i><br />");

    foreach (People l_people, movie.peopleList()) {
        switch (l_people.type())
        {
        case People::Director:
            l_directors += l_people.name() + ", ";
            break;
        case People::Producer:
            l_producers += l_people.name() + ", ";
            break;
        case People::Actor:
            l_actors += l_people.name() + ", ";
            break;
        }
    }
    // Need to remove ", " = 2 chars
    if (l_directors.right(2) == ", ") {
        l_directors = l_directors.remove(l_directors.size()-2, 2);

    }
    l_directors += "<br /><br />";
    if (l_producers.right(2) == ", ") {
        l_producers = l_producers.remove(l_producers.size()-2,2);
    }
    l_producers += "<br /><br />";
    if (l_actors.right(2) == ", ") {
        l_actors = l_actors.remove(l_actors.size()-2,2);
    }
    l_actors += "<br /><br />";
    l_actors += "</html>";

    m_ui->informationsLabel->setText(l_title+l_originalTitle + l_directors +l_producers + l_actors);
    m_ui->plotLabel->setText(movie.synopsis());
    QPixmap l_poster;
    if (movie.posterPath() != "")
    {
        QString l_posterPath = qApp->property("postersPath").toString()
                               + movie.posterPath();
        l_poster.load(l_posterPath);
        QSize l_size(this->width()-30, this->height()-30);
        l_poster = l_poster.scaled(l_size, Qt::KeepAspectRatio);
    }
    m_ui->posterLabel->setPixmap(l_poster);
    Macaw::DEBUG_OUT("[MetadataPannel] Exits fill()");
}
