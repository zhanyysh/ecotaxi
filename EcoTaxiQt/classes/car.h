#ifndef CAR_H
#define CAR_H

#include "QVariantList"
#include "QString"

using namespace std;

class Car {
private:
    int id;
    QString sid;
    QString brand;
    QString model;
    QString licensePlate;
    int year;
    int investorId;
    float milleage;
    QString description;
    int percentage;

public:
    Car();
    Car(QVariantList car);

    int getId();
    QString getSid();
    QString getBrand();
    QString getModel();
    QString getLicensePlate();
    int getYear();
    int getInvestorId();
    float getMilleage();
    QString getDescription();
    int getPercentage();

    void setBrand(QString newBrand);
    void setModel(QString newModel);
    void setLicensePlate(QString newLicensePlate);
    void setYear(int newYear);
    void setInvestor(int newInvestorId);
    void setMilleage(float newMilleage);
    void setDescription(QString newDescription);
    void setPercentage(int newPercentage);
};

#endif // CAR_H
