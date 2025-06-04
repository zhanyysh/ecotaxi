#ifndef NAVIGATIONMANAGER_H
#define NAVIGATIONMANAGER_H

#include <functional>

#include <QWidget>

#include "nm.h"

#include "../mainwindow.h"
#include "../pages/settingspage.h"
#include "../pages/eventpage.h"
#include "../pages/generalreport.h"
#include "../pages/reportpage.h"
#include "../pages/editablereport.h"
#include "../pages/paymenthistory.h"

#include "../enums/setting.h"
#include "../enums/eSetting.h"
#include "../enums/Events.h"
#include "../enums/Report.h"

namespace Ui
{
    class navigationManager;
}

class navigationManager : public QWidget, public nm
{
    Q_OBJECT

    Q_INTERFACES(nm)

public:
    static navigationManager &getInstance();

    void changeWindow(int id);

    bool openSettings(int id);

    bool openEvents(int id, QDate = QDate());

    bool openReport(int index, int id = 0, QDate = QDate(), QDate = QDate());

    bool userOpenReport(int index, int id = 0, QDate = QDate(), QDate = QDate());

    bool openFines(int index, int id = 0, QDate from = QDate(), QDate to = QDate());

    bool openPaymentHistory();

private:
    navigationManager(QWidget *parent = nullptr);
    ~navigationManager();

    Ui::navigationManager *ui;

    std::function<void()> lastFunc;

    MainWindow *MainPage;

    SettingsPage *SettingPage;
    eventPage *EventPage;

    ReportPage *ReportsPage;
    GeneralReport *GReportPage;

    EditableReport *EditReport;

    PaymentHistory *paymentHistoryPage = nullptr;

private slots:
    void reload();
};

#endif // NAVIGATIONMANAGER_H
