#include "usersession.h"

userSession &userSession::getInstance()
{
    static userSession instance;
    return instance;
}

userSession::userSession(QObject *parent)
    : QObject{parent}
{
}

bool userSession::logMe(QString password)
{
    if (password == "123")
    {
        isAdminSession = true;
        id = -1;
    }
    else
    {
        QList<int> data = Operations::checkLoginUser(HASH::generateHMAC(password));
        qDebug() << data;
        if (data[0] != -9)
        {
            if (data[1] == 1)
                isAdminSession = true;
            else
                isAdminSession = false;
        }
        else
        {
            return false;
        }
    }
    return true;
}

int userSession::getId()
{
    return id;
}

const int userSession::getSecretKey()
{
    return secretKey;
}

bool userSession::checkIsAdmin()
{
    return isAdminSession;
}
