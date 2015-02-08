#ifndef METADATAFETCHERDIALOG_H
#define METADATAFETCHERDIALOG_H

#include <QDialog>

namespace Ui {
class MetadataFetcherDialog;
}

class MetadataFetcherDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MetadataFetcherDialog(QWidget *parent = 0);
    ~MetadataFetcherDialog();

private:
    Ui::MetadataFetcherDialog *m_ui;
};

#endif // METADATAFETCHERDIALOG_H
