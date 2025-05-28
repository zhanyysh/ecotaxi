#ifndef DRIVER_H
#define DRIVER_H

#include "QVariantList"
#include "QString"

using namespace std;

class Driver
{
private:
    int id;
    QString name;
    QString description;

public:
    Driver();
    Driver(QVariantList driver);
    int getId();
    QString getName();
    QString getDescription();
    void setName(QString newName);
    void setDecription(QString newDescription);
};

#endif // DRIVER_H
