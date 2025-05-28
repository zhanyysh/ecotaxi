#include "customdialog.h"
#include "ui_customdialog.h"

CustomDialog::CustomDialog(QString text, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CustomDialog)
{
    ui->setupUi(this);
    ui->Header->setText(text);

    this->setWindowModality(Qt::ApplicationModal);
}

CustomDialog::~CustomDialog()
{
    delete ui;
}

void CustomDialog::on_YesButton_clicked() {
    emit yesClicked();
    this->close();
}

void CustomDialog::on_NoButton_clicked() {
    emit noClicked();
    this->close();
}



