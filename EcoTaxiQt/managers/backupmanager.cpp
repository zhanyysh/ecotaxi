#include "backupmanager.h"


backupManager::backupManager() {}



QString backupManager::getAppDir()
{
    return QCoreApplication::applicationDirPath();
}

void backupManager::createFolders()
{
    QString appDir = getAppDir();
    QDir folder(appDir + "/backup");
    if (!folder.exists()) {
        folder.mkdir(appDir + "/backup");
    }
    QDir carsFolder(appDir + "/backup/cars");
    if (!carsFolder.exists()) {
        carsFolder.mkdir(appDir + "/backup/cars");
    }
    QDir investorsFolder(appDir + "/backup/investors");
    if (!investorsFolder.exists()) {
        investorsFolder.mkdir(appDir + "/backup/investors");
    }
    QDir driversFolder(appDir + "/backup/drivers");
    if (!driversFolder.exists()) {
        driversFolder.mkdir(appDir + "/backup/drivers");
    }
    QDir locationsFolder(appDir + "/backup/locations");
    if (!locationsFolder.exists()) {
        locationsFolder.mkdir(appDir + "/backup/locations");
    }
    QDir usersFolder(appDir + "/backup/users");
    if (!usersFolder.exists()) {
        usersFolder.mkdir(appDir + "/backup/users");
    }
}


void backupManager::createInvestorBackup(int id) {
    // Create the folder path
    QString dir = getAppDir() + "/backup/investors/";

    // Get the investor's report
    QVariantList investorReport = ReportOperations::getInvestorReport(id, QDate::currentDate().addYears(-20), QDate::currentDate().addDays(1));
    QVariantList allInvestorReport = ReportOperations::getAllInvestorReport(id, QDate::currentDate().addYears(-20), QDate::currentDate().addDays(1));

    Investor investor = Operations::getInvestor(id);

    // Create the CSV file
    QFile file(dir + QString::number(id) + "_" + investor.getName() + ".csv");

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Error opening file";
        return;
    }

    // Write the header to the CSV file
    QTextStream stream(&file);
    stream.setEncoding(QStringConverter::Utf8);
    stream << QString::number(id) + "," + investor.getName() + "," + investor.getDescription() << "\n";

    // Write the investor's data to the CSV file
    for (QVariant car : investorReport) {
        QVariantList row = car.toList();
        stream << row[0].toInt() << "," << row[1].toString() << "," << row[2].toString() << "," << row[3].toString() << "\n";
    }

    // Write the investor's report to the CSV file
    if (!allInvestorReport.empty()) {
        stream << "ИТОГО," << allInvestorReport[0].toInt() << "," << allInvestorReport[1].toInt() << "," << allInvestorReport[2].toInt() << "\n";
    }
    else {
        stream << "ИТОГО,0,0,0";
    }

    // Close the file
    file.close();
}



void backupManager::createCarBackup(int id) {
    QString dir = getAppDir() + "/backup/cars/";

    QVariantList carsReport = ReportOperations::getCarReport(id, QDate::currentDate().addYears(-20), QDate::currentDate().addDays(1));
    QVariantList allCarsReport = ReportOperations::getAllCarReport(id, QDate::currentDate().addYears(-20), QDate::currentDate().addDays(1));

    Car car = Operations::getCar(id);

    QFile file(dir + QString::number(id) + "_" + car.getBrand() + ".csv");

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Error opening file";
        return;
    }

    qDebug() << "test 0";

    QTextStream stream(&file);
    stream.setEncoding(QStringConverter::Utf8);
    stream << QString::number(id) + "," + car.getBrand() + "," + car.getModel() + "," + car.getLicensePlate() + "," + QString::number(car.getYear()) + ","  + Operations::getInvestor(car.getInvestorId()).getName() + "(" + QString::number(car.getInvestorId()) + ")" + "," + QString::number(car.getMilleage()) + "," + car.getDescription() << "\n";

    qDebug() << "test 1";

    for (QVariant car : carsReport) {
        QVariantList row = car.toList();
        stream << row[0].toString() << "," << row[1].toString() << "," << row[2].toString() << "," << row[3].toString() << row[4].toString() << "\n";
    }

    qDebug() << "test 2";

    if (!allCarsReport.empty()) {
        stream << "ИТОГО," << allCarsReport[0].toInt() << "," << allCarsReport[1].toInt() << "," << allCarsReport[2].toInt() << "\n";
    }
    else {
        stream << "ИТОГО,0,0,0";
    }

    file.close();
}

void backupManager::createDriverBackup(int id) {
    QString dir = getAppDir() + "/backup/drivers/";

    QVariantList driversReport = ReportOperations::getDriverReport(id, QDate::currentDate().addYears(-20), QDate::currentDate().addDays(1));
    QVariantList allDriversReport = ReportOperations::getAllDriverReport(id, QDate::currentDate().addYears(-20), QDate::currentDate().addDays(1));

    Driver driver = Operations::getDriver(id);

    QFile file(dir + QString::number(id) + "_" + driver.getName() + ".csv");

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Error opening file";
        return;
    }

    QTextStream stream(&file);
    stream.setEncoding(QStringConverter::Utf8);
    stream << QString::number(id) + "," + driver.getName() + "," + driver.getDescription() << "\n";

    for (QVariant car : driversReport) {
        QVariantList row = car.toList();
        stream << row[0].toString() << "," << row[1].toString() << "," << row[2].toString() << "," << row[3].toString() << row[4].toString() << "\n";
    }

    if (!allDriversReport.empty()) {
        stream << "ИТОГО," << allDriversReport[0].toInt() << "," << allDriversReport[1].toInt() << "," << allDriversReport[2].toInt() << "\n";
    }
    else {
        stream << "ИТОГО,0,0,0";
    }

    file.close();
}


void backupManager::createLocationBackup(int id) {
    QString dir = getAppDir() + "/backup/locations/";

    QVariantList locationsReport = ReportOperations::getLocationReport(id, QDate::currentDate().addYears(-20), QDate::currentDate().addDays(1));
    QVariantList allLocationsReport = ReportOperations::getAllLocationReport(id, QDate::currentDate().addYears(-20), QDate::currentDate().addDays(1));

    Location location = Operations::getLocation(id);

    QFile file(dir + QString::number(id) + "_" + location.getName() + ".csv");

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Error opening file";
        return;
    }

    QTextStream stream(&file);
    stream.setEncoding(QStringConverter::Utf8);
    stream << QString::number(id) + "," + location.getName() + "," + location.getDescription() << "\n";

    for (QVariant car : locationsReport) {
        QVariantList row = car.toList();
        stream << row[0].toString() << "," << row[1].toString() << "," << row[2].toString() << "," << row[3].toString() << row[4].toString() << "\n";
    }
    if (!allLocationsReport.empty()) {
        stream << "ИТОГО," << allLocationsReport[0].toInt() << "\n";
    }
    else {
        stream << "ИТОГО,0,0,0";
    }

    file.close();
}


void backupManager::createUserBackup(int id) {
    QString dir = getAppDir() + "/backup/users/";

    QVariantList usersReport = ReportOperations::getUserReport(id, QDate::currentDate().addYears(-20), QDate::currentDate().addDays(1));
    QVariantList allUsersReport = ReportOperations::getAllUserReport(id, QDate::currentDate().addYears(-20), QDate::currentDate().addDays(1));

    User user = Operations::getUser(id);

    QFile file(dir + QString::number(id) + "_" + user.getName() + ".csv");

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Error opening file";
        return;
    }

    QTextStream stream(&file);
    stream.setEncoding(QStringConverter::Utf8);
    stream << QString::number(id) + "," + user.getName() + "," + user.getDescription() << "\n";

    for (QVariant user : usersReport) {
        QVariantList row = user.toList();
        stream << row[0].toString() << "," << row[1].toString() << "\n";
    }
    if (!allUsersReport.empty()) {
        stream << "ИТОГО," << allUsersReport[0].toList()[0].toString() << "\n";
    }
    else {
        stream << "ИТОГО,0";
    }
    
    file.close();
}
