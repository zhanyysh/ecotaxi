#include "car.h"
using namespace std;

Car::Car() {}

Car::Car(QVariantList car)
{
    if (car.isEmpty())
    {
        this->id = -1;
        this->sid = "-1";
    }
    else
    {
        this->id = car[0].toInt();
        this->sid = car[1].toString();
        this->brand = car[2].toString();
        this->model = car[3].toString();
        this->licensePlate = car[4].toString();
        this->year = car[5].toInt();
        this->investorId = car[6].toInt();
        this->milleage = car[7].toInt();
        this->description = car[8].toString();
        this->percentage = car[9].toInt();
    }
}

int Car::getId()
{
    return id;
}

QString Car::getSid()
{
    return sid;
}

QString Car::getBrand()
{
    return brand;
}

QString Car::getModel()
{
    return model;
}

QString Car::getLicensePlate()
{
    return licensePlate;
}

int Car::getYear()
{
    return year;
}

int Car::getInvestorId()
{
    return investorId;
}

float Car::getMilleage()
{
    return milleage;
}

QString Car::getDescription()
{
    return description;
}

int Car::getPercentage()
{
    return percentage;
}

void Car::setBrand(QString newBrand)
{
    brand = newBrand;
}

void Car::setModel(QString newModel)
{
    model = newModel;
}

void Car::setLicensePlate(QString newLicensePlate)
{
    licensePlate = newLicensePlate;
}

void Car::setYear(int newYear)
{
    year = newYear;
}

void Car::setInvestor(int newInvestorId)
{
    investorId = newInvestorId;
}

void Car::setMilleage(float newMilleage)
{
    milleage = newMilleage;
}

void Car::setDescription(QString newDescription)
{
    description = newDescription;
}

void Car::setPercentage(int newPercentage)
{
    percentage = newPercentage;
}
