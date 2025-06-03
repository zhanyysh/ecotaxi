#ifndef EXCELMANAGER_H
#define EXCELMANAGER_H

#include <QCoreApplication>
#include <QTextDocument>
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QMimeData>
#include <QApplication>
#include <QClipboard>
#include <QStandardPaths>
#include <QStandardItemModel>
#include <QAbstractItemModel>

class ExcelManager
{
public:
    ExcelManager();

    // Export table data to Excel
    static void exportToExcel(QString title, QString type, QList<QAbstractItemModel *> models);

private:
    static QString getAppDir();
    static QString getDesktopDir();
    static void createExcel(QString title, QString type, QList<QAbstractItemModel *> models);
};

#endif // EXCELMANAGER_H 