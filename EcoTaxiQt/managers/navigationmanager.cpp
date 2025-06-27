#include "navigationmanager.h"
#include "ui_navigationmanager.h"

navigationManager &navigationManager::getInstance()
{
    static navigationManager instance;
    return instance;
}

navigationManager::navigationManager(QWidget *parent)
    : QWidget(parent), nm(), ui(new Ui::navigationManager), isGoingBack(false)
{
    dbManager &db = dbManager::getInstance();

    connect(&db, &dbManager::reload, this, &navigationManager::reload);

    ui->setupUi(this);

    lastFunc = [this]() { this->MainPage->openWidnow(); };

    this->MainPage = new MainWindow(this);

    this->SettingPage = new SettingsPage(this);
    this->EventPage = new eventPage(this);

    this->ReportsPage = new ReportPage(this);
    this->GReportPage = new GeneralReport(this);

    this->EditReport = new EditableReport(this);

    ui->pages->insertWidget(0, this->MainPage);

    ui->pages->insertWidget(1, this->SettingPage);
    ui->pages->insertWidget(2, this->EventPage);

    ui->pages->insertWidget(3, this->GReportPage);
    ui->pages->insertWidget(4, this->ReportsPage);

    ui->pages->insertWidget(5, this->EditReport);

    this->changeWindow(0);
}

navigationManager::~navigationManager()
{
    delete ui;
}

void navigationManager::changeWindow(int id)
{
    // Сохраняем текущий индекс в историю только если это не возврат назад
    if (!isGoingBack && ui->pages->currentIndex() != id) {
        navigationHistory.push(ui->pages->currentIndex());
    }
    isGoingBack = false; // Сбрасываем флаг после использования

    if (id == 0)
    {
        lastFunc = [this]() { this->MainPage->openWidnow(); };
    }
    if (id == 5)
    {
        lastFunc = [this]() { this->EditReport->openReport(eSetting::Repairs); };
    }
    lastFunc();
    ui->pages->setCurrentIndex(id);
}

void navigationManager::goBack()
{
    if (!navigationHistory.isEmpty()) {
        isGoingBack = true; // Устанавливаем флаг перед возвратом назад
        int previousIndex = navigationHistory.pop();
        changeWindow(previousIndex);
    } else {
        // Если история пуста, возвращаемся на главную
        changeWindow(0);
    }
}

bool navigationManager::openSettings(int id)
{
    switch (id)
    {
    case 0:
        lastFunc = [this]() { this->SettingPage->setSettings(Setting::Drivers); };
        break;
    case 1:
        lastFunc = [this]() { this->SettingPage->setSettings(Setting::Cars); };
        break;
    case 2:
        lastFunc = [this]() { this->SettingPage->setSettings(Setting::Investors); };
        break;
    case 3:
        lastFunc = [this]() { this->SettingPage->setSettings(Setting::Types); };
        break;
    case 4:
        lastFunc = [this]() { this->SettingPage->setSettings(Setting::Locations); };
        break;
    case 5:
        lastFunc = [this]() { this->SettingPage->setSettings(Setting::Users); };
        break;
    case 6:
        lastFunc = [this]() { this->SettingPage->setSettings(Setting::Parameters); };
        break;
    default:
        return false;
    }
    lastFunc();
    changeWindow(1);
    return true;
}

bool navigationManager::openEvents(int id, QDate date)
{
    switch (id)
    {
    case 0:
        this->EventPage->setEvents(Events::Events, date);
        lastFunc = [this](QDate date=QDate()) { this->EventPage->setEvents(Events::Events, date); };
        break;
    case 1:
        this->EventPage->setEvents(Events::Charges, date);
        lastFunc = [this](QDate date=QDate()) { this->EventPage->setEvents(Events::Charges, date); };
        break;
    default:
        return false;
    }
    changeWindow(2);
    return true;
}

bool navigationManager::userOpenReport(int index, int id, QDate from, QDate to) {
    switch (index) {
    case 0:
        this->GReportPage->setReport(Report::Cars, from, to);
        lastFunc = [this](QDate from=QDate(), QDate to=QDate()) { this->GReportPage->setReport(Report::Cars, from, to); };
        changeWindow(3);
        break;
    case 1:
        this->GReportPage->setReport(Report::Charges, from, to);
        lastFunc = [this](QDate from=QDate(), QDate to=QDate()) { this->GReportPage->setReport(Report::Charges, from, to); };
        changeWindow(3);
        break;
    case 2:
        this->GReportPage->setReport(Report::Debts, from, to);
        lastFunc = [this](QDate from=QDate(), QDate to=QDate()) { this->GReportPage->setReport(Report::Debts, from, to); };
        changeWindow(3);
        break;
    default:
        return false;
        break;
    }
    return true;
}

bool navigationManager::openReport(int index, int id, QDate from, QDate to)
{
    switch (index)
    {
    case 0:
        this->GReportPage->setReport(Report::Investors, from, to);
        lastFunc = [this](QDate from=QDate(), QDate to=QDate()) { this->GReportPage->setReport(Report::Investors, from, to); };
        changeWindow(3);
        break;
    case 1:
        this->GReportPage->setReport(Report::Types, from, to);
        lastFunc = [this](QDate from=QDate(), QDate to=QDate()) { this->GReportPage->setReport(Report::Types, from, to); };
        changeWindow(3);
        break;
    case 2:
        this->GReportPage->setReport(Report::Cars, from, to);
        lastFunc = [this](QDate from=QDate(), QDate to=QDate()) { this->GReportPage->setReport(Report::Cars, from, to); };
        changeWindow(3);
        break;
    case 3:
        this->GReportPage->setReport(Report::Drivers, from, to);
        lastFunc = [this](QDate from=QDate(), QDate to=QDate()) { this->GReportPage->setReport(Report::Drivers, from, to); };
        changeWindow(3);
        break;
    case 4:
        this->GReportPage->setReport(Report::Locations, from, to);
        lastFunc = [this](QDate from=QDate(), QDate to=QDate()) { this->GReportPage->setReport(Report::Locations, from, to); };
        changeWindow(3);
        break;
    case 5:
        this->GReportPage->setReport(Report::Users2, from, to);
        lastFunc = [this](QDate from=QDate(), QDate to=QDate()) { this->GReportPage->setReport(Report::Users2, from, to); };
        changeWindow(3);
        break;
    case 6:
        this->GReportPage->setReport(Report::Charges, from, to);
        lastFunc = [this](QDate from=QDate(), QDate to=QDate()) { this->GReportPage->setReport(Report::Charges, from, to); };
        changeWindow(3);
        break;
    case 7:
        this->ReportsPage->setReport(Report::Types, id, from, to);
        lastFunc = [this](int id=-2, QDate from=QDate(), QDate to=QDate()) { this->ReportsPage->setReport(Report::Types, id, from, to); };
        changeWindow(4);
        break;
    case 8:
        this->ReportsPage->setReport(Report::Investors, id, from, to);
        lastFunc = [this](int id=-2, QDate from=QDate(), QDate to=QDate()) { this->ReportsPage->setReport(Report::Investors, id, from, to); };
        changeWindow(4);
        break;
    case 9:
        this->ReportsPage->setReport(Report::Cars, id, from, to);
        lastFunc = [this](int id=-2, QDate from=QDate(), QDate to=QDate()) { this->ReportsPage->setReport(Report::Cars, id, from, to); };
        changeWindow(4);
        break;
    case 10:
        this->ReportsPage->setReport(Report::Drivers, id, from, to);
        lastFunc = [this](int id=-2, QDate from=QDate(), QDate to=QDate()) { this->ReportsPage->setReport(Report::Drivers, id, from, to); };
        changeWindow(4);
        break;
    case 11:
        this->ReportsPage->setReport(Report::Users, id, from, to);
        lastFunc = [this](int id=-2, QDate from=QDate(), QDate to=QDate()) { this->ReportsPage->setReport(Report::Users, id, from, to); };
        changeWindow(4);
        break;
    case 12:
        this->ReportsPage->setReport(Report::Locations, id, from, to);
        lastFunc = [this](int id=-2, QDate from=QDate(), QDate to=QDate()) { this->ReportsPage->setReport(Report::Locations, id, from, to); };
        changeWindow(4);
        break;
    case 13:
        this->ReportsPage->setReport(Report::Charges, id, from, to);
        lastFunc = [this](int id=-2, QDate from=QDate(), QDate to=QDate()) { this->ReportsPage->setReport(Report::Charges, id, from, to); };
        changeWindow(4);
        break;
    case 14:
        this->GReportPage->setReport(Report::Users, from, to);
        lastFunc = [this](QDate from=QDate(), QDate to=QDate()) {this->GReportPage->setReport(Report::Users, from, to);};
        break;
    case 15:
        this->GReportPage->setReport(Report::Debts, from, to);
        lastFunc = [this](QDate from=QDate(), QDate to=QDate()) { this->GReportPage->setReport(Report::Debts, from, to); };
        changeWindow(3);
        break;
    case 16:
        this->GReportPage->setReport(Report::Debts, from, to);
        lastFunc = [this](QDate from=QDate(), QDate to=QDate()) { this->GReportPage->setReport(Report::Debts, from, to); };
        changeWindow(3);
        break;
    default:
        return false;
        break;
    }
    return true;
}

void navigationManager::reload()
{
    lastFunc();
}

bool navigationManager::openFines(int index, int id, QDate from, QDate to)
{
    if (index  == 0)
    {
        lastFunc = [this]() { this->EditReport->openReport(eSetting::Fines); };
        lastFunc();
        ui->pages->setCurrentIndex(5);
        return true;
    }
    if (index == 1)
    {
        this->GReportPage->setReport(Report::FinesByCars, from, to);
        lastFunc = [this](QDate from=QDate(), QDate to=QDate()) { this->GReportPage->setReport(Report::FinesByCars, from, to); };
        changeWindow(3);
        return true;
    }
    if (index == 2)
    {
        this->GReportPage->setReport(Report::FinesByDrivers, from, to);
        lastFunc = [this](QDate from=QDate(), QDate to=QDate()) { this->GReportPage->setReport(Report::FinesByDrivers, from, to); };
        changeWindow(3);
        return true;
    }
    if (index == 3)
    {
        this->ReportsPage->setReport(Report::FinesByCars, id, from, to);
        lastFunc = [this](int id=-2, QDate from=QDate(), QDate to=QDate()) { this->ReportsPage->setReport(Report::FinesByCars, id, from, to); };
        changeWindow(4);
        return true;
    }
    if (index == 4)
    {
        this->ReportsPage->setReport(Report::FinesByDrivers, id, from, to);
        lastFunc = [this](int id=-2, QDate from=QDate(), QDate to=QDate()) { this->ReportsPage->setReport(Report::FinesByDrivers, id, from, to); };
        changeWindow(4);
        return true;
    }
    return false;
}

bool navigationManager::openPaymentHistory()
{
    if (!paymentHistoryPage) {
        paymentHistoryPage = new PaymentHistory(this);
        ui->pages->insertWidget(6, paymentHistoryPage);
        connect(paymentHistoryPage, &PaymentHistory::backClicked, this, [this]() {
            this->changeWindow(0); // Возврат на главную страницу
        });
    }
    changeWindow(6);
    return true;
}
