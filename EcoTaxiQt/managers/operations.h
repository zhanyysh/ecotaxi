#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <QVariantList>
#include <QList>
#include "QString"
#include "QDate"

#include "dbmanager.h"
#include "backupmanager.h"
#include "usersession.h"

#include "../classes/car.h"
#include "../classes/investor.h"
#include "../classes/driver.h"
#include "../classes/event.h"
#include "../classes/charge.h"
#include "../classes/type.h"
#include "../classes/location.h"
#include "../classes/user.h"

using namespace std;

class Operations
{
public:
    Operations();
    QList<Investor> static selectAllInvestors();

    QList<Car> static selectAllCars();
    QVariantList static selectAllCarsQuick();

    QList<Driver> static selectAllDrivers();

    QList<Event> static selectAllEvents();
    QVariantList static selectEventsByDate(QDate date);
    QVariantList static getAllEventsReport(QDate fromDate);

    QList<Charge> static selectAllCharges();
    QVariantList static selectChargesByDate(QDate date);
    QVariantList static getAllChargesReport(QDate fromDate);

    QList<Type> static selectAllTypes();

    QList<Location> static selectAllLocations();

    QList<User> static selectAllUsers();

    bool static addInvestor(Investor investor);
    bool static updateInvestor(Investor investor);
    bool static deleteInvestor(int id);
    Investor static getInvestor(int id);
    void static deleteInvestorCars(int id);

    bool static addCar(Car car);
    bool static updateCar(int id, Car car);
    bool static deleteCar(int id);
    Car static getCar(int id);

    bool static addDriver(Driver driver);
    bool static updateDriver(Driver driver);
    bool static deleteDriver(int id);
    Driver static getDriver(int id);

    bool static addEvent(Event event);
    bool static updateEvent(Event newEvent);
    bool static deleteEvent(int id);
    Event static getEvent(int id);

    bool static addCharge(Charge charge);
    bool static updateCharge(Charge charge);
    bool static deleteCharge(int id);
    Charge static getCharge(int id);

    bool static addType(Type type);
    bool static updateType(Type type);
    bool static deleteType(int id);
    Type static getType(int id);

    bool static addLocation(Location location);
    bool static updateLocation(Location location);
    bool static deleteLocation(int id);
    Location static getLocation(int id);

    bool static addUser(User user);
    bool static updateUser(User user);
    bool static deleteUser(int id);
    User static getUser(int id);
    QList<int> static checkLoginUser(QString password);

    bool static addRepair(int carId, QDate fromDate, QDate toDate, QString description);
    bool static updateRepair(int id, int carId, QDate fromDate, QDate toDate, QString description);
    bool static deleteRepair(int id);
    QVariantList static getRepair(int id);

    bool static addFine(QDate date, int carId, int driverId, int amount, bool isPaid, QString description);
    bool static updateFine(int id, QDate date, int carId, int driverId, int amount, bool isPaid, QString description);
    bool static deleteFine(int id);
    QVariantList static getFine(int id);
};

#endif // OPERATIONS_H
