#include "dbmanager.h"
#include "qapplication.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QCoreApplication>
#include <QDir>

dbManager &dbManager::getInstance()
{
    static dbManager instance;
    return instance;
}

dbManager::dbManager()
{
    ni = new NoInternet();
    connect(ni, &NoInternet::reload, this, &dbManager::onReload);
}

dbManager::~dbManager()
{
    if (db.isOpen())
    {
        db.close();
    }
}

bool dbManager::connectDB()
{
    db = QSqlDatabase::addDatabase("QMYSQL");

    // QFile file("conn.data");
    // if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    //     qDebug() << "Failed to open file for reading.";
    //     return false;
    // }

    // QTextStream in(&file);
    // EncryptionManager& manager = EncryptionManager::getInstance();

    // QString hostData = manager.decrypt(in.readLine());
    // QString portData = manager.decrypt(in.readLine());
    // QString dbName = manager.decrypt(in.readLine());
    // QString username = manager.decrypt(in.readLine());
    // QString password = manager.decrypt(in.readLine());

    db.setHostName("localhost");
    db.setPort(3306);
    db.setDatabaseName("ecotaxidb");
    db.setUserName("root");
    db.setPassword("");

    if (!db.open())
    {
        qDebug() << "database connection error: " << db.lastError().text();
        // Logger::instance()->logInfo("database connection error: " + db.lastError().text());
        openError();
        return false;
    }
    else
    {
        qDebug() << "database connected successfully!";
        // Logger::instance()->logInfo("database connected successfully!");
        createTables();
        return true;
    }
}

void dbManager::resetDB()
{
    executeSet("DELETE FROM events;");
    executeSet("DELETE FROM charges;");
    executeSet("DELETE FROM drivers;");
    executeSet("DELETE FROM investors;");
    executeSet("DELETE FROM cars;");
    executeSet("DELETE FROM locations;");
    executeSet("DELETE FROM users;");
    executeSet("DELETE FROM types;");
    executeSet("DELETE FROM logins;");
    createTables();
}

void dbManager::createTables()
{
    createCarTable();
    createChargeTable();
    createDriverTable();
    createEventTable();
    createInvestorTable();
    createTypeTable();
    createLocationTable();
    createUserTable();
    createLoginTable();
    createRepairsTable();
    createFinesTable();
    createPaymentsTable();
}

bool dbManager::isConnected() const
{
    return db.isOpen();
}

bool dbManager::onReload()
{
    if (connectDB()) {
        emit reload();
        return true;
    }
    return false;
}

void dbManager::openError()
{
    if (ni->isHidden())
        ni->show();
}

bool dbManager::executeSet(const QString query)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    qDebug() << "set query execution: " << query;
    QSqlQuery q;
    if (!q.exec(query))
    {
        qDebug() << "set query error: " << q.lastError();
        openError();
        QApplication::restoreOverrideCursor();
        return false;
    }
    QApplication::restoreOverrideCursor();
    return true;
}

QVariantList dbManager::executeGet(const QString query)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    qDebug() << "get query execution: " << query;
    QSqlQuery q;
    QVariantList out;
    if (q.exec(query))
    {
        while (q.next())
        {
            out.push_back(getValuesFromRecord(q.record()));
        }
    }
    else
    {
        qDebug() << "get query error: " << q.lastError();
        openError();
    }
    QApplication::restoreOverrideCursor();
    return out;
}

QVariantList dbManager::getValuesFromRecord(const QSqlRecord &record)
{
    QVariantList values;
    for (int i = 0; i < record.count(); ++i)
    {
        values.append(record.value(i));
    }
    return values;
}

void dbManager::createInvestorTable()
{
    QString createTableQuery = R"Q(
        CREATE TABLE IF NOT EXISTS investors (
            id INTEGER PRIMARY KEY AUTO_INCREMENT,
            name TEXT NOT NULL,
            description TEXT,
            password TEXT
        )
    )Q";

    this->executeSet(createTableQuery);
}

void dbManager::createCarTable()
{
    QString createTableQuery = R"Q(
        CREATE TABLE IF NOT EXISTS cars (
            id INTEGER PRIMARY KEY AUTO_INCREMENT,
            sid VARCHAR(255) NOT NULL,
            brand TEXT NOT NULL,
            model TEXT NOT NULL,
            licensePlate TEXT NOT NULL,
            year INTEGER NOT NULL,
            investorId INTEGER NOT NULL,
            mileage FLOAT NOT NULL,
            description TEXT,
            percentage INT NOT NULL
        )
    )Q";
    this->executeSet(createTableQuery);
}

void dbManager::createDriverTable()
{
    QString createTableQuery = R"Q(
        CREATE TABLE IF NOT EXISTS drivers (
            id INTEGER PRIMARY KEY AUTO_INCREMENT,
            name TEXT NOT NULL,
            description TEXT
        )
    )Q";
    this->executeSet(createTableQuery);
}

void dbManager::createEventTable()
{
    QString createTableQuery = R"Q(
        CREATE TABLE IF NOT EXISTS events (
            id INTEGER PRIMARY KEY AUTO_INCREMENT,
            carId INTEGER NOT NULL,
            driverId INTEGER NOT NULL,
            typeId INTEGER NOT NULL,
            dolg FLOAT NOT NUll,
            amount FLOAT NOT NULL,
            description TEXT,
            date DATETIME DEFAULT CURRENT_TIMESTAMP,
            userId INTEGER NOT NULL
        )
    )Q";
    this->executeSet(createTableQuery);
}

void dbManager::createChargeTable()
{
    QString createTableQuery = R"Q(
        CREATE TABLE IF NOT EXISTS charges (
            id INTEGER PRIMARY KEY AUTO_INCREMENT,
            carId INTEGER NOT NULL,
            driverId INTEGER NOT NULL,
            locationId INTEGER NOT NULL,
            kwh FLOAT NOT NULL,
            duration FLOAT NOT NULL,
            date DATETIME DEFAULT CURRENT_TIMESTAMP,
            userId INTEGER NOT NULL
        )
    )Q";
    this->executeSet(createTableQuery);
}

void dbManager::createTypeTable()
{
    QString createTableQuery = R"Q(
        CREATE TABLE IF NOT EXISTS types (
            id INTEGER PRIMARY KEY AUTO_INCREMENT,
            name TEXT NOT NULL,
            description TEXT,
            forAdmin BOOLEAN NOT NULL DEFAULT false
        )
    )Q";

    QString checkColumnQuery = R"Q(
        SELECT COUNT(*) AS count
        FROM information_schema.columns
        WHERE table_name = 'types'
        AND column_name = 'forAdmin'
        AND table_schema = DATABASE();
    )Q";
    QVariantList columnExists = this->executeGet(checkColumnQuery);
    qDebug() << columnExists;
    if (!columnExists.isEmpty() && !columnExists[0].toList().isEmpty() && columnExists[0].toList()[0].toInt() == 0)
    {
        QString addColumnQuery = R"Q(
            ALTER TABLE types
            ADD COLUMN forAdmin BOOLEAN NOT NULL DEFAULT false;
        )Q";
        this->executeSet(addColumnQuery);
    }
    else
    {
        qDebug() << "Column 'userId' already exists or error in query (types).";
    }

    this->executeSet(createTableQuery);
}

void dbManager::createLocationTable()
{
    QString createTableQuery = R"Q(
        CREATE TABLE IF NOT EXISTS locations (
            id INTEGER PRIMARY KEY AUTO_INCREMENT,
            name TEXT NOT NULL,
            description TEXT
        )
    )Q";
    this->executeSet(createTableQuery);
}

void dbManager::createUserTable()
{
    QString createTableQuery = R"Q(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTO_INCREMENT,
            name TEXT NOT NULL,
            password TEXT NOT NULL,
            description TEXT,
            isAdmin BOOLEAN NOT NULL DEFAULT false
        )
    )Q";
    this->executeSet(createTableQuery);
}

void dbManager::createLoginTable()
{
    QString createTableQuery = R"Q(
        CREATE TABLE IF NOT EXISTS logins (
            id INTEGER PRIMARY KEY AUTO_INCREMENT,
            date DATETIME DEFAULT CURRENT_TIMESTAMP,
            userId INTEGER NOT NULL
        )
    )Q";
    this->executeSet(createTableQuery);
}

void dbManager::createRepairsTable()
{
    QString createTableQuery = R"Q(
        CREATE TABLE IF NOT EXISTS repairs (
            id INTEGER PRIMARY KEY AUTO_INCREMENT,
            carId INTEGER NOT NULL,
            fromDate DATE NOT NULL,
            toDate DATE,
            description TEXT
        )
    )Q";
    this->executeSet(createTableQuery);
}

void dbManager::createFinesTable()
{
    QString createTableQuery = R"Q(
        CREATE TABLE IF NOT EXISTS fines (
            id INTEGER PRIMARY KEY AUTO_INCREMENT,
            date DATE NOT NULL,
            carId INTEGER NOT NULL,
            driverId INTEGER,
            time TIME NOT NULL,
            FID TEXT NOT NULL,
            amount FLOAT NOT NULL,
            isPaid BOOLEAN NOT NULL DEFAULT false,
            description TEXT
        )
    )Q";
    this->executeSet(createTableQuery);
}

void dbManager::createPaymentsTable()
{
    QString createTableQuery = R"Q(
        CREATE TABLE IF NOT EXISTS payments (
            id INTEGER PRIMARY KEY AUTO_INCREMENT,
            investor_id INTEGER NOT NULL,
            date DATE NOT NULL,
            amount DOUBLE NOT NULL,
            FOREIGN KEY (investor_id) REFERENCES investors(id)
        )
    )Q";
    this->executeSet(createTableQuery);
}
