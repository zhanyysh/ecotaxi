#ifndef ADDUPDATEREPAIR_H
#define ADDUPDATEREPAIR_H

#include <QWidget>
#include <QVariantList>
#include <QTimer>
#include "calendarpage.h"

#include "../enums/eSetting.h"

#include "../managers/operations.h"

namespace Ui {
class AddUpdateRepair;
}

class AddUpdateRepair : public QWidget
{
    Q_OBJECT

public:
    explicit AddUpdateRepair(eSetting mode, int id = -1, QWidget *parent = nullptr);
    ~AddUpdateRepair();

signals:
    void closed();

private slots:
    void on_pushButton_clicked();

    void on_fromButton_clicked();
    void on_toButton_clicked();

    void setFromDate(QDate date);
    void setToDate(QDate date);

private:
    Ui::AddUpdateRepair *ui;

    void addRecord();

    bool checkFill();

    void resetInputColor();

    eSetting mode;

    QList<Car> carsId_list;
    QMap<QString, int> carsId;
    QMap<int, QString> carsSid;

    QList<Driver> driversId_list;
    QMap<QString, int> driversId;
    QMap<int, QString> driversName;

    QDate from;
    QDate to;

    int id;
};

#endif // ADDUPDATEREPAIR_H
