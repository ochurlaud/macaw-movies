#include "MetadataFetcherDialog.h"
#include "ui_MetadataFetcherDialog.h"

MetadataFetcherDialog::MetadataFetcherDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MetadataFetcherDialog)
{
    ui->setupUi(this);
}

MetadataFetcherDialog::~MetadataFetcherDialog()
{
    delete ui;
}
