#ifndef USERSESSION_H
#define USERSESSION_H

#include <QObject>
#include <QDebug>
#include "QString"

#include "../scripts/hash.h"

#include "../managers/operations.h"

class userSession : public QObject
{
    Q_OBJECT
public:
    static userSession &getInstance();

    int getId();

    bool checkIsAdmin();
    bool logMe(QString password);

    const int getSecretKey();

private:
    explicit userSession(QObject *parent = nullptr);

    userSession(const userSession &) = delete;
    userSession &operator=(const userSession &) = delete;

    int id;

    const int secretKey = 2134353453;

    bool isAdminSession;
};

#endif // USERSESSION_H
