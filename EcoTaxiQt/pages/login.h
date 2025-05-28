#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include "QLabel"

#include "../managers/usersession.h"
#include "../managers/navigationmanager.h"

namespace Ui
{
    class login;
}

class login : public QWidget
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    ~login();

private slots:
    void on_loginButton_clicked();
    void on_passwordEdit_returnPressed();

private:
    Ui::login *ui;

    void logIn();
};

#endif // LOGIN_H
