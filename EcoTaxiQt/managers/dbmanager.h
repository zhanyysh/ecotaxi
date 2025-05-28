#ifndef DBMANAGER_H
#define DBMANAGER_H

#include "logger.h"
#include "encryptionmanager.h"
#include "../pages/nointernet.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QVariantList>
#include <QSqlRecord>
#include <QDir>
#include <QDebug>

class dbManager : public QObject
{
    Q_OBJECT
public:
    static dbManager &getInstance();

    bool connectDB();
    bool isConnected() const;
    bool executeSet(const QString query);
    QVariantList executeGet(const QString query);

    void openError();

    void resetDB();

private:
    dbManager();
    ~dbManager();

    QVariantList getValuesFromRecord(const QSqlRecord &record);

    void createTables();

    void createInvestorTable();
    void createCarTable();
    void createDriverTable();
    void createEventTable();
    void createChargeTable();
    void createTypeTable();
    void createLocationTable();
    void createUserTable();
    void createLoginTable();
    void createRepairsTable();
    void createFinesTable();

    NoInternet *ni;

    QString dbName;
    QSqlDatabase db;

signals:
    void reload();

private slots:
    bool onReload();
};

#endif // DBMANAGER_H
