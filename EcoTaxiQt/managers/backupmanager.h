#ifndef BACKUPMANAGER_H
#define BACKUPMANAGER_H

#include <QCoreApplication>
#include <QDir>
#include <QFile>

#include "../classes/investor.h"
#include "reportoperations.h"
#include "operations.h"


class backupManager
{
public:
    backupManager();

    void static createFolders();

    void static createDriverBackup(int id);

    void static createInvestorBackup(int id);

    void static createCarBackup(int id);

    void static createLocationBackup(int id);

    void static createUserBackup(int id);

private:
    QString static getAppDir();
};

#endif // BACKUPMANAGER_H
