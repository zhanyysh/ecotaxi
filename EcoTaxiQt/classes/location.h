#ifndef LOCATION_H
#define LOCATION_H

#include "QVariantList"
#include "QString"

class Location {
private:
    int id;
    QString name;
    QString description;
public:
    Location();
    Location(QVariantList location);
    int getId();
    QString getName();
    QString getDescription();

    void setName(QString newName);
    void setDescription(QString newDescription);
};

#endif // LOCATION_H
