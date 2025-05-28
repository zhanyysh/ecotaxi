#ifndef TYPE_H
#define TYPE_H

#include "QVariantList"
#include "QString"

class Type
{
private:
    int id;
    QString name;
    QString description;
    bool forAdmin;
public:
    Type();
    Type(QVariantList type);
    int getId();
    QString getName();
    QString getDescription();
    bool getForAdmin();

    void setName(QString newName);
    void setDescription(QString newDescription);
    void setForAdmin(bool newForAdmin);
};

#endif // TYPE_H
