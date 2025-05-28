#ifndef USER_H
#define USER_H

#include "QVariantList"
#include "QString"

class User
{
private:
    int id;
    QString name;
    QString password;
    QString description;
    bool isAdmin;

public:
    User();
    User(QVariantList user);
    int getId();
    QString getName();
    QString getPassword();
    QString getDescription();
    bool getIsAdmin();

    void setName(QString newName);
    void setPassword(QString newPassword);
    void setDescription(QString newDescription);
    void setIsAdmin(bool newIsAdmin);
};

#endif // USER_H
