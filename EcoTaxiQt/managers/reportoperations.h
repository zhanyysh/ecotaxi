#ifndef REPORTOPERATIONS_H
#define REPORTOPERATIONS_H

#include <QDate>
#include <QVariantList>
#include "QString"
#include "dbmanager.h"

class ReportOperations
{

public:
    ReportOperations();

    // general reports
    QVariantList static getCarsReport(QDate fromDate, QDate toDate);
    QVariantList static getAllCarsReport(QDate fromDate, QDate toDate);

    QVariantList static getTypesReport(QDate fromDate, QDate toDate);
    QVariantList static getAllTypesReport(QDate fromDate, QDate toDate);

    QVariantList static getDriversReport(QDate fromDate, QDate todate);
    QVariantList static getAllDriversReport(QDate fromDate, QDate toDate);

    QVariantList static getInvestorsReport(QDate fromDate, QDate toDate);
    QVariantList static getAllInvestorsReport(QDate fromDate, QDate toDate);

    QVariantList static getLocationsReport(QDate fromDate, QDate toDate);
    QVariantList static getAllLocationsReport(QDate fromDate, QDate toDate);

    QVariantList static getChargesReport(QDate fromDate, QDate toDate);
    QVariantList static getAllChargesReport(QDate fromDate, QDate toDate);

    QVariantList static getUsersReport(QDate fromDate, QDate toDate);
    QVariantList static getAllUsersReport(QDate fromDate, QDate toDate);

    QVariantList static getUsers2Report(QDate fromDate, QDate toDate);
    QVariantList static getAllUsers2Report(QDate fromDate, QDate toDate);

    QVariantList static getDebtsReport(QDate fromDate, QDate toDate);
    QVariantList static getAllDebtsReport(QDate fromDate, QDate toDate);

    QVariantList static getFinesByCarsReport(QDate fromDate, QDate toDate);
    QVariantList static getAllFinesByCarsReport(QDate fromDate, QDate toDate);

    QVariantList static getFinesByDriversReport(QDate fromDate, QDate toDate);
    QVariantList static getAllFinesByDriversReport(QDate fromDate, QDate toDate);

    // reports by item
    QVariantList static getCarReport(int carId, QDate fromDate, QDate toDate);
    QVariantList static getAllCarReport(int carId, QDate fromDate, QDate toDate);

    QVariantList static getTypeReport(int typeId, QDate fromDate, QDate toDate);
    QVariantList static getAllTypeReport(int typeId, QDate fromDate, QDate toDate);

    QVariantList static getDriverReport(int driverId, QDate fromDate, QDate toDate);
    QVariantList static getAllDriverReport(int driverId, QDate fromDate, QDate toDate);

    QVariantList static getInvestorReport(int investorId, QDate fromDate, QDate toDate);
    QVariantList static getAllInvestorReport(int investorId, QDate fromDate, QDate toDate);

    QVariantList static getLocationReport(int locationId, QDate fromDate, QDate toDate);
    QVariantList static getAllLocationReport(int locationId, QDate fromDate, QDate toDate);

    QVariantList static getChargesByCarReport(int carId, QDate fromDate, QDate toDate);
    QVariantList static getAllChargesByCarReport(int carId, QDate fromDate, QDate toDate);

    QVariantList static getUserReport(int userId, QDate fromDate, QDate toDate);
    QVariantList static getAllUserReport(int userId, QDate fromDate, QDate toDate);

    QVariantList static getFinesByCarReport(int carId, QDate fromDate, QDate toDate);
    QVariantList static getAllFinesByCarReport(int carId, QDate fromDate, QDate toDate);

    QVariantList static getFinesByDriverReport(int driverId, QDate fromDate, QDate toDate);
    QVariantList static getAllFinesByDriverReport(int driverId, QDate fromDate, QDate toDate);

    // other reports
    QVariantList static getRepairsReport();
    QVariantList static getNotDoneRepairsReport();

    QVariantList static getFinesReport();
    QVariantList static getNotPaidFinesReport();
};

#endif // REPORTOPERATIONS_H
