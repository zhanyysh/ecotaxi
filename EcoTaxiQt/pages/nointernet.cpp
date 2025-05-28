#include "nointernet.h"
#include "ui_nointernet.h"

NoInternet::NoInternet(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::NoInternet)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose);

    this->setWindowModality(Qt::ApplicationModal);
}

NoInternet::~NoInternet()
{
    QApplication::quit();
    delete ui;
}

void NoInternet::on_ReloadButton_clicked()
{
    ui->ReloadButton->setText("Проверка...");
    if(emit reload())
    {
        this->hide();
    }
    ui->ReloadButton->setText("Обновить");
}

void NoInternet::on_ExitButton_clicked()
{
    QApplication::quit();
}
