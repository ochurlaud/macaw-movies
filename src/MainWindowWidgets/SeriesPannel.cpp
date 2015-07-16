#include "SeriesPannel.h"
#include "ui_SeriesPannel.h"

SeriesPannel::SeriesPannel(QWidget *parent) :
    MainPannel(parent),
    m_ui(new Ui::SeriesPannel)
{
    m_ui->setupUi(this);
}

SeriesPannel::~SeriesPannel()
{
    delete m_ui;
}
