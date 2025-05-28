#include "location.h"
using namespace std;

Location::Location() {}

Location::Location(QVariantList location)
{
    if (location.isEmpty())
    {
        this->id = -1;
        this->name = "удален";
        this->description = "";
    }
    else
    {
        this->id = location[0].toInt();
        this->name = location[1].toString();
        this->description = location[2].toString();
    }
}

int Location::getId()
{
    return id;
}

QString Location::getName()
{
    return name;
}

QString Location::getDescription()
{
    return description;
}

void Location::setName(QString newName)
{
    name = newName;
}

void Location::setDescription(QString newDescription)
{
    description = newDescription;
}
