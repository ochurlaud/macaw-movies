#include "SettingsWindow.h"

SettingsWindow::SettingsWindow(QWidget *parent) : QWidget(parent)
{
    setFixedSize(400,300);
    setWindowTitle("Settings");
    mainLayout = new QVBoxLayout(this);
    filesPathLayout = new QHBoxLayout;

    filesPathLabel = new QLabel("Files path");
    filesPathEdit = new QLineEdit;
    filesPathEdit->setText("tttt");
    filesPathSearchButton = new QPushButton("Browse");
    QObject::connect(filesPathSearchButton, SIGNAL(clicked()), this, SLOT(browseFilesPathDialog()));


    submitButton = new QPushButton("Submit", this);
    QObject::connect(submitButton, SIGNAL(clicked()), this, SLOT(applySetting()));

    mainLayout->addLayout(filesPathLayout);
    mainLayout->addWidget(submitButton);
    filesPathLayout->addWidget(filesPathLabel);
    filesPathLayout->addWidget(filesPathEdit);
    filesPathLayout->addWidget(filesPathSearchButton);
}

SettingsWindow::~SettingsWindow()
{
}

void SettingsWindow::applySetting()
{
    Application *app = qobject_cast<Application *>(qApp);
    app->setFilesPath(filesPathEdit->text());
    close();
    qDebug()<< app->getFilesPath();
}

void SettingsWindow::browseFilesPathDialog()
{
    QString folder = QFileDialog::getExistingDirectory(this);
    filesPathEdit->setText(folder);
}
void SettingsWindow::closeEvent(QCloseEvent *event)
{
  emit closed();
  event->accept();
}
