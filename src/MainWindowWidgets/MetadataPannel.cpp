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
 * @author Sébastien TOUZÉ <sebastien.touze@yahoo.fr>
 * @param movie
 */
void MetadataPannel::fill(const Movie &movie)
{
    m_movie = movie;
    Macaw::DEBUG_IN("[MetadataPannel] Enters fill()");

    // without title, it means we don't want to show the metadata
    if (m_movie.title() == "") {
        this->hide();

    } else {
        this->show();

        QString l_title = "<b>"+m_movie.title()+"</b>"+"<br />";
        QString l_directors, l_producers, l_actors, l_originalTitle;
        if (m_movie.originalTitle() != m_movie.title())
        {
            l_originalTitle = "<i>"+m_movie.originalTitle()+"</i><br />";
        }
        if (!m_movie.peopleList(People::Director).isEmpty()) {
            l_directors = tr("<i>Directed by</i><br />");
        }
        if (!m_movie.peopleList(People::Producer).isEmpty()) {
            l_producers = tr("<i>Produced by</i><br />");
        }
        if (!m_movie.peopleList(People::Actor).isEmpty()) {
            l_actors = tr("<i>With</i><br />");
        }

        foreach (People l_people, m_movie.peopleList()) {
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
            l_directors += "<br /><br />";
        }

        if (l_producers.right(2) == ", ") {
            l_producers = l_producers.remove(l_producers.size()-2,2);
            l_producers += "<br /><br />";
        }

        if (l_actors.right(2) == ", ") {
            l_actors = l_actors.remove(l_actors.size()-2,2);
            l_actors += "<br /><br />";
        }


        m_ui->informationsLabel->setText(l_title
                                         + l_originalTitle
                                         + "<br />"
                                         + l_directors
                                         + l_producers
                                         + l_actors);
        m_ui->plotLabel->setText(m_movie.synopsis());
        this->setPoster();
    }
    Macaw::DEBUG_OUT("[MetadataPannel] Exits fill()");
}

void MetadataPannel::resizeEvent(QResizeEvent *event)
{
    (void)event;
    this->setPoster();
}

void MetadataPannel::setPoster()
{
    QPixmap l_poster;
    if (m_movie.posterPath() != "")
    {
        QString l_posterPath = qApp->property("postersPath").toString()
                               + m_movie.posterPath();
        l_poster.load(l_posterPath);

        QSize l_size(this->width()*0.9, this->height()/2.5);
        l_poster = l_poster.scaled(l_size, Qt::KeepAspectRatio);
    }
    m_ui->posterLabel->setPixmap(l_poster);
}
