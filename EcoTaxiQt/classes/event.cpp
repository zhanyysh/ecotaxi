#include "event.h"

using namespace std;

Event::Event() {}

Event::Event(QVariantList event)
{
    this->id = event[0].toInt();
    this->carId = event[1].toInt();
    this->driverId = event[2].toInt();
    this->typeId = event[3].toInt();
    this->amount = event[4].toFloat();
    this->description = event[5].toString();
    this->date = event[6].toDateTime();
}

int Event::getId()
{
    return id;
}

int Event::getCarId()
{
    return carId;
}

int Event::getDriverId()
{
    return driverId;
}

int Event::getTypeId()
{
    return typeId;
}

float Event::getAmount()
{
    return amount;
}

QDateTime Event::getDate() {
    return date;
}

QString Event::getDescription()
{
    return description;
}

void Event::setCarId(int newCarId)
{
    carId = newCarId;
}

void Event::setDriverId(int newDriverId)
{
    driverId = newDriverId;
}

void Event::setTypeId(int newTypeId)
{
    typeId = newTypeId;
}

void Event::setAmount(float newAmount)
{
    amount = newAmount;
}

void Event::setDescription(QString newDescription)
{
    description = newDescription;
}

void Event::setDate(QDateTime newdate) {
    date = newdate;
}
