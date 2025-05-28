#ifndef EVENT_H
#define EVENT_H

#include "QVariantList"
#include "QString"
#include "QDateTime"

using namespace std;

class Event
{

private:
    int id;
    int carId;
    int driverId;
    int typeId;
    float amount;
    QString description;
    QDateTime date;

public:
    Event();
    Event(QVariantList event);
    int getId();
    int getCarId();
    int getDriverId();
    int getTypeId();
    float getAmount();
    QString getDescription();
    QDateTime getDate();

    void setCarId(int newCarId);
    void setDriverId(int newDriverId);
    void setTypeId(int newTypeId);
    void setAmount(float newAmount);
    void setDescription(QString newDescription);
    void setDate(QDateTime newDate);
};

#endif // EVENT_H
