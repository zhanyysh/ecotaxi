#ifndef ADDUPDATEWINDOWEVENTS_H
#define ADDUPDATEWINDOWEVENTS_H

#include <QWidget>
#include <QVariantList>
#include <QTimer>


#include "../managers/operations.h"

#include "../enums/Events.h"

#include "../classes/charge.h"
#include "../classes/event.h"

namespace Ui {
class addupdatewindowEvents;
}

class addupdatewindowEvents : public QWidget
{
    Q_OBJECT

public:
    explicit addupdatewindowEvents(Events mode, int id = -1, QWidget *parent = nullptr);
    ~addupdatewindowEvents();

signals:
    void closed();

private slots:
    void on_pushButton_clicked();

private:
    Ui::addupdatewindowEvents *ui;

    void addRecord();

    bool checkFill();

    void resetInputColor();

    QList<Type> types_list;
    QList<Driver> driversId_list;
    QList<Car> carsId_list;
    QList<Location> locations_list;

    QMap<QString, int> types;
    QMap<QString, int> driversId;
    QMap<QString, int> carsId;
    QMap<QString, int> locations;

    Events mode;
    int id;

    QDateTime date;
};

#endif // ADDUPDATEWINDOWEVENTS_H
