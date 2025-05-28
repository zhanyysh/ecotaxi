#ifndef CHARGE_H
#define CHARGE_H

#include "QVariantList"
#include "QString"
#include "QDateTime"

using namespace std;

class Charge
{
private:
    int id;
    int carId;
    int driverId;
    int locationId;
    float kwh;
    float duration;
    QDateTime date;

public:
    Charge();
    Charge(QVariantList charge);
    int getId();
    int getCarId();
    int getDriverId();
    int getLocationId();
    float getKwh();
    float getDuration();
    QDateTime getDate();

    void setCarId(int newCarId);
    void setDriverId(int newDriverId);
    void setLocationId(int newLocationId);
    void setKwh(float newKwh);
    void setDuration(float newDuration);
    void setDate(QDateTime newDate);
};

#endif // CHARGE_H
