#include "removedb.h"
#include "ui_removedb.h"

RemoveDB::RemoveDB(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RemoveDB)
{
    ui->setupUi(this);

    this->setWindowModality(Qt::ApplicationModal);
    setAttribute(Qt::WA_DeleteOnClose);
}

RemoveDB::~RemoveDB()
{
    delete ui;
}

void RemoveDB::on_pushButton_clicked()
{
    if (ui->lineEdit->text() == "2902")
    {
        dbManager &db = dbManager::getInstance();
        db.resetDB();
        emit yesClicked();
        this->close();
    }
    else
    {
        QApplication::quit();
    }
}
