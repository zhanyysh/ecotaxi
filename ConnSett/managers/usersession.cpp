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
    if (password == "290284")
    {
        isAdminSession = true;
        id = -1;
    }
    else
    {
        return false;
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
