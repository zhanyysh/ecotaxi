#include "driver.h"
#include "iostream"
using namespace std;

Driver::Driver() {}

Driver::Driver(QVariantList driver)
{
    if (driver.isEmpty())
    {
        this->id = -1;
        this->name = "удален";
        this->description = "";
    }
    else
    {
        this->id = driver[0].toInt();
        this->name = driver[1].toString();
        this->description = driver[2].toString();
    }
}

int Driver::getId()
{
    return id;
}

QString Driver::getName()
{
    return name;
}

QString Driver::getDescription()
{
    return description;
}

void Driver::setName(QString newName)
{
    name = newName;
}

void Driver::setDecription(QString newDescription)
{
    description = newDescription;
}
