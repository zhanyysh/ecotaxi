#include "login.h"
#include "ui_login.h"
#include "QTimer"

login::login(QWidget *parent)
    : QWidget(parent), ui(new Ui::login)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose);

    this->setWindowTitle("Авторизация | EcoTaxi");
    this->setWindowIcon(QIcon());
    ui->password->setProperty("color", "white");
}

login::~login()
{
    delete ui;
}

void login::logIn()
{
    userSession &u = userSession::getInstance();
    if (u.logMe(ui->passwordEdit->text()))
    {
        navigationManager &w = navigationManager::getInstance();
        this->hide();
        w.showMaximized();
    }
    else
    {
        QApplication::quit();
    }
}

void login::on_loginButton_clicked() {
    logIn();
}

void login::on_passwordEdit_returnPressed() {
    logIn();
}
