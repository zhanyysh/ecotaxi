#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <QString>
#include <QDate>
#include <QTime>
#include <QVariantList>
#include <QList>
#include "car.h"
#include "driver.h"
#include "investor.h"
#include "location.h"
#include "type.h"
#include "user.h"

class Operations
{
public:
    static void addCar(QString sid, int investorId, int percentage);
    static void updateCar(int id, QString sid, int investorId, int percentage);
    static void deleteCar(int id);
    static QVariantList selectCar(int id);
    static QList<Car> selectAllCars();

    static void addDriver(QString name, QString phone, QString address);
    static void updateDriver(int id, QString name, QString phone, QString address);
    static void deleteDriver(int id);
    static QVariantList selectDriver(int id);
    static QList<Driver> selectAllDrivers();

    static void addInvestor(QString name, QString phone, QString address);
    static void updateInvestor(int id, QString name, QString phone, QString address);
    static void deleteInvestor(int id);
    static QVariantList selectInvestor(int id);
    static QList<Investor> selectAllInvestors();

    static void addLocation(QString name);
    static void updateLocation(int id, QString name);
    static void deleteLocation(int id);
    static QVariantList selectLocation(int id);
    static QList<Location> selectAllLocations();

    static void addType(QString name);
    static void updateType(int id, QString name);
    static void deleteType(int id);
    static QVariantList selectType(int id);
    static QList<Type> selectAllTypes();

    static void addUser(QString name, QString password);
    static void updateUser(int id, QString name, QString password);
    static void deleteUser(int id);
    static QVariantList selectUser(int id);
    static QList<User> selectAllUsers();

    static void addEvent(QDate date, int carId, int driverId, int typeId, int amount, QString description, int userId);
    static void updateEvent(int id, QDate date, int carId, int driverId, int typeId, int amount, QString description, int userId);
    static void deleteEvent(int id);
    static QVariantList selectEvent(int id);

    static void addCharge(QDate date, int carId, int driverId, double kwh, int duration, int locationId);
    static void updateCharge(int id, QDate date, int carId, int driverId, double kwh, int duration, int locationId);
    static void deleteCharge(int id);
    static QVariantList selectCharge(int id);

    static void addRepair(int carId, QDate fromDate, QDate toDate, QString description);
    static void updateRepair(int id, int carId, QDate fromDate, QDate toDate, QString description);
    static void deleteRepair(int id);
    static QVariantList selectRepair(int id);

    static void addFine(QDate date, int carId, int driverId, int amount, bool isPaid, QString description, QTime time);
    static void updateFine(int id, QDate date, int carId, int driverId, int amount, bool isPaid, QString description, QTime time);
    static void deleteFine(int id);
    static QVariantList getFine(int id);
};

#endif // OPERATIONS_H
