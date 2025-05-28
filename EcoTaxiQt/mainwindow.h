#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDate>
#include <QTime>

#include "managers/nm.h"
#include "managers/usersession.h"
#include "managers/operations.h"

#include "classes/car.h"
#include "classes/driver.h"
#include "classes/location.h"
#include "classes/type.h"

#include "classes/event.h"
#include "classes/charge.h"

#include "pages/calendarpage.h"
#include "pages/removedb.h"

#include "scripts/awss3uploader.h"

#include <QFileDialog>
#include "QTimer"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(nm *nav, QWidget *parent = nullptr);
    ~MainWindow();

    void openWidnow();

private slots:
    void on_SettingsButton_currentIndexChanged(int index);
    void on_ReportsButton_currentIndexChanged(int index);
    void on_FinesButton_currentIndexChanged(int index);

    void on_addEventButton_clicked();
    void on_addChargeButton_clicked();

    void on_eventsButton_clicked();
    void on_chargesButton_clicked();
    void on_repairsButton_clicked();

    // void on_addImagesButton_clicked();

    void on_dateButton_clicked();

    void on_systemTimeCheck_checkStateChanged(Qt::CheckState state);

    void onChangeDate(QDate date);


private:
    Ui::MainWindow *ui;

    dbManager *db;

    nm *nav;

    QMap<QString, int> cars;
    QMap<QString, int> types;
    QMap<QString, int> locations;
    QMap<QString, int> drivers;

    void setComboBoxesData();

    void setSettingIndex();
    void setReportIndex();
    void setFinesIndex();

    bool checkEventFill();
    bool checkChargeFill();
    void resetInputColor();

    void clearAllInputsEvent();
    void clearAllInputsCharge();

    QDate date;

    bool useSystemTime = true;
    QTime time;
};
#endif // MAINWINDOW_H
