#include "charge.h"
#include "iostream"
using namespace std;

Charge::Charge() {}

Charge::Charge(QVariantList charge)
{
    this->id = charge[0].toInt();
    this->carId = charge[1].toInt();
    this->driverId = charge[2].toInt();
    this->locationId = charge[3].toInt();
    this->kwh = charge[4].toFloat();
    this->duration = charge[5].toFloat();
    this->date = charge[6].toDateTime();
    
    // Проверяем, есть ли поле kwh_multiplier в данных
    if (charge.size() > 7) {
        this->kwh_multiplier = charge[7].toFloat();
    } else {
        this->kwh_multiplier = 10.0; // Значение по умолчанию
    }
}

int Charge::getId()
{
    return id;
}

int Charge::getCarId()
{
    return carId;
}

int Charge::getDriverId()
{
    return driverId;
}

int Charge::getLocationId()
{
    return locationId;
}

float Charge::getKwh()
{
    return kwh;
}

float Charge::getDuration()
{
    return duration;
}

QDateTime Charge::getDate() {
    return date;
}

float Charge::getKwhMultiplier()
{
    return kwh_multiplier;
}

void Charge::setCarId(int newCarId)
{
    carId = newCarId;
}

void Charge::setDriverId(int newDriverId)
{
    driverId = newDriverId;
}

void Charge::setLocationId(int newLocationId)
{
    locationId = newLocationId;
}

void Charge::setKwh(float newKwh)
{
    kwh = newKwh;
}

void Charge::setDuration(float newDuration)
{
    duration = newDuration;
}

void Charge::setDate(QDateTime newDate) {
    date = newDate;
};

void Charge::setKwhMultiplier(float newKwhMultiplier)
{
    kwh_multiplier = newKwhMultiplier;
}
