#include "CenteredCheckbox.h"

CenteredCheckbox::CenteredCheckbox(QWidget *parent) :
    QWidget(parent)
{
    this->setContentsMargins(10,0,0,0);
    m_checkbox = new QCheckBox;
    QHBoxLayout *l_layout = new QHBoxLayout(this);
    l_layout->addWidget(m_checkbox);
    l_layout->setAlignment(Qt::AlignCenter);
    l_layout->setContentsMargins(0,0,0,0);
    this->setLayout(l_layout);

    connect(m_checkbox, SIGNAL(stateChanged(int)),
            this, SLOT(on_checkbox_stateChanged(int)));
}

CenteredCheckbox::~CenteredCheckbox()
{
}

void CenteredCheckbox::setChecked(const bool checked)
{
    m_checkbox->setChecked(checked);
}

bool CenteredCheckbox::isChecked() const
{
    return m_checkbox->isChecked();
}

void CenteredCheckbox::on_checkbox_stateChanged(int state)
{
    emit stateChanged(state);
}
