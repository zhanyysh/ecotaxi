#ifndef HASH_H
#define HASH_H

#include <QMessageAuthenticationCode>
#include <QByteArray>
#include <QString>
#include <QDebug>

class HASH
{
public:
    HASH();

    QString static generateHMAC(const QString &data);

    bool static compareHMACs(const QString &hmac1, const QString &hmac2);
};

#endif // HASH_H
