#ifndef CENTEREDCHECKBOX_H
#define CENTEREDCHECKBOX_H

#include <QCheckBox>
#include <QLayout>

class CenteredCheckbox : public QWidget
{
    Q_OBJECT

public:
    explicit CenteredCheckbox(QWidget *parent = 0);
    ~CenteredCheckbox();
    void setChecked(const bool checked);
    bool isChecked() const;

signals:
    void stateChanged(int state);

private slots:
    void on_checkbox_stateChanged(int state);

private:
    QCheckBox *m_checkbox;
};

#endif // CENTEREDCHECKBOX_H
