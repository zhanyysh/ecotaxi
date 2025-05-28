#ifndef INVESTOR_H
#define INVESTOR_H

#include "QVariantList"

using namespace std;

class   Investor
{
private:
    int id;
    QString name;
    QString description;
    QString password;

public:
    Investor();
    Investor(QVariantList investor);

    int getId();
    QString getName();
    QString getDescription();
    QString getPassword();
    void setName(QString newName);
    void setDescription(QString newDescription);
    void setPassword(QString newPassword);
};

#endif // INVESTOR_H
