#include "settingspage.h"
#include "ui_settingspage.h"

SettingsPage::SettingsPage(nm *nav, QWidget *parent)
    : QWidget(parent), ui(new Ui::SettingsPage)
{
    this->nav = nav;
    ui->setupUi(this);

    connect(ui->tableView, &QTableView::doubleClicked, this, &SettingsPage::handleDoubleClick);
}

SettingsPage::~SettingsPage()
{
    delete ui;
}

void SettingsPage::setSettings(Setting table)
{
    this->table = table;

    setHeader();
    setTable();
}

void SettingsPage::setHeader()
{
    QString header = "meow";
    switch (this->table)
    {
    case Setting::Drivers:
        header = "ВОДИТЕЛИ";
        break;

    case Setting::Cars:
        header = "МАШИНЫ";
        break;

    case Setting::Investors:
        header = "ИНВЕСТОРЫ";
        break;

    case Setting::Types:
        header = "ТИПЫ";
        break;

    case Setting::Locations:
        header = "ЛОКАЦИИ";
        break;

    case Setting::Users:
        header = "Пользователи";
        break;
    }

    ui->EcoTaxi->setText(header);
}

bool SettingsPage::setTable()
{
    QStandardItemModel *model = new QStandardItemModel();

    switch (this->table)
    {
    case Setting::Drivers:
        model->setHorizontalHeaderLabels({"ID", "Имя", "Описание"});
        for (Driver driver : Operations::selectAllDrivers())
            model->appendRow({new QStandardItem(QString::number(driver.getId())),
                              new QStandardItem(driver.getName()),
                              new QStandardItem(driver.getDescription())});
        break;
    case Setting::Cars:
        if (true) {
            userSession &us = userSession::getInstance();
            if (us.checkIsAdmin()) {
                model->setHorizontalHeaderLabels({"uid", "ID", "Марка", "Модель", "Госномер", "Год", "Инвестор", "Пробег", "Процент" , "Описание"});
                for (const QVariant &car : Operations::selectAllCarsQuick())
                {
                    QVariantList values = car.toList();
                    model->appendRow({new QStandardItem(values[0].toString()),
                                      new QStandardItem(values[1].toString()),
                                      new QStandardItem(values[2].toString()),
                                      new QStandardItem(values[3].toString()),
                                      new QStandardItem(values[4].toString()),
                                      new QStandardItem(values[5].toString()),
                                      new QStandardItem(values[6].toString()),
                                      new QStandardItem(values[7].toString()),
                                      new QStandardItem(values[8].toString() + "%"),
                                      new QStandardItem(values[9].toString())});
                }
            }
            else
            {
                model->setHorizontalHeaderLabels({"uid", "ID", "Марка", "Модель", "Госномер", "Год", "Инвестор", "Пробег", "Описание"});
                for (const QVariant &car : Operations::selectAllCarsQuick())
                {
                    QVariantList values = car.toList();
                    model->appendRow({new QStandardItem(values[0].toString()),
                                      new QStandardItem(values[1].toString()),
                                      new QStandardItem(values[2].toString()),
                                      new QStandardItem(values[3].toString()),
                                      new QStandardItem(values[4].toString()),
                                      new QStandardItem(values[5].toString()),
                                      new QStandardItem(values[6].toString()),
                                      new QStandardItem(values[7].toString()),
                                      new QStandardItem(values[9].toString())});
                }
            }
        }
        break;
    case Setting::Investors:
        model->setHorizontalHeaderLabels({"ID", "Имя", "Описание"});
        for (Investor investor : Operations::selectAllInvestors())
            model->appendRow({new QStandardItem(QString::number(investor.getId())),
                              new QStandardItem(investor.getName()),
                              new QStandardItem(investor.getDescription())});
        break;
    case Setting::Types:
        model->setHorizontalHeaderLabels({"ID", "Название", "Описание"});
        for (Type type : Operations::selectAllTypes())
            model->appendRow({new QStandardItem(QString::number(type.getId())),
                              new QStandardItem(type.getName()),
                              new QStandardItem(type.getDescription())});
        break;
    case Setting::Locations:
        model->setHorizontalHeaderLabels({"ID", "Название", "Описание"});
        for (Location location : Operations::selectAllLocations())
            model->appendRow({new QStandardItem(QString::number(location.getId())),
                              new QStandardItem(location.getName()),
                              new QStandardItem(location.getDescription())});
        break;
    case Setting::Users:
        model->setHorizontalHeaderLabels({"ID", "Имя", "Описание"});
        for (User user : Operations::selectAllUsers())
            model->appendRow({new QStandardItem(QString::number(user.getId())),
                              new QStandardItem(user.getName()),
                              new QStandardItem(user.getDescription())});
        break;
    }

    ui->tableView->setModel(model);

    ui->tableView->setColumnHidden(0, true);

    ui->tableView->resizeColumnsToContents();

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);

    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    ui->tableView->horizontalHeader()->setCascadingSectionResizes(true);

    connect(ui->tableView->horizontalHeader(), &QHeaderView::sectionResized, this, &SettingsPage::onSectionResized);

    switch (this->table)
    {
    case Setting::Drivers:
        ui->tableView->setColumnWidth(0, 200);
        ui->tableView->setColumnWidth(1, 400);
        ui->tableView->setColumnWidth(2, 530);
        break;
    case Setting::Cars:
        ui->tableView->setColumnWidth(0, 100);
        ui->tableView->setColumnWidth(1, 130);
        ui->tableView->setColumnWidth(2, 130);
        ui->tableView->setColumnWidth(3, 170);
        ui->tableView->setColumnWidth(4, 130);
        ui->tableView->setColumnWidth(5, 130);
        ui->tableView->setColumnWidth(6, 130);
        ui->tableView->setColumnWidth(7, 130);
        ui->tableView->setColumnWidth(8, 140);
        break;
    case Setting::Investors:
        ui->tableView->setColumnWidth(0, 200);
        ui->tableView->setColumnWidth(1, 400);
        ui->tableView->setColumnWidth(2, 530);
        break;
    case Setting::Types:
        ui->tableView->setColumnWidth(0, 200);
        ui->tableView->setColumnWidth(1, 400);
        ui->tableView->setColumnWidth(2, 530);
        break;
    case Setting::Locations:
        ui->tableView->setColumnWidth(0, 200);
        ui->tableView->setColumnWidth(1, 400);
        ui->tableView->setColumnWidth(2, 530);
        break;
    case Setting::Users:
        ui->tableView->setColumnWidth(0, 200);
        ui->tableView->setColumnWidth(1, 400);
        ui->tableView->setColumnWidth(2, 530);
        break;
    }

    return true;
}

void SettingsPage::on_BackButton_clicked()
{
    nav->changeWindow(0);
}

void SettingsPage::on_AddButton_clicked()
{
    addUpdateWindow *w = new addUpdateWindow(this->table, -1);
    w->resize(w->minimumSizeHint());
    w->show();

    QEventLoop loop;
    connect(w, SIGNAL(closed()), &loop, SLOT(quit()));
    loop.exec();

    setTable();
}

void SettingsPage::on_EditButton_clicked()
{
    QItemSelectionModel *select = ui->tableView->selectionModel();
    if (select->hasSelection())
    {
        int row = select->selectedRows().at(0).row();

        int id = ui->tableView->model()->index(row, 0).data().toLongLong();

        addUpdateWindow *w = new addUpdateWindow(this->table, id);
        w->resize(w->minimumSizeHint());
        w->show();

        QEventLoop loop;
        connect(w, SIGNAL(closed()), &loop, SLOT(quit()));
        loop.exec();

        setTable();
    }
}

void SettingsPage::on_DeleteButton_clicked()
{
    QItemSelectionModel *select = ui->tableView->selectionModel();
    if (select->hasSelection())
    {
        int row = select->selectedRows().at(0).row();
        int id = ui->tableView->model()->index(row, 0).data().toLongLong();
        CustomDialog *d = nullptr;

        switch (this->table)
        {
        case Setting::Drivers:
        {
            Driver driver = Operations::getDriver(id);
            d = new CustomDialog("Вы уверены что хотите удалить водителя " + driver.getName() + "?");
            break;
        }
        case Setting::Cars:
        {
            Car car = Operations::getCar(id);
            d = new CustomDialog("Вы уверены что хотите удалить машину " + car.getBrand() + " " + car.getModel() + " " + car.getLicensePlate() + " " + QString::number(car.getYear()) + " " + "?");
            break;
        }
        case Setting::Investors:
        {
            Investor investor = Operations::getInvestor(id);
            d = new CustomDialog("Вы уверены что хотите удалить инвестора " + investor.getName() + "?");
            break;
        }
        case Setting::Types:
        {
            Type type = Operations::getType(id);
            d = new CustomDialog("Вы уверены что хотите удалить тип " + type.getName() + "?");
            break;
        }
        case Setting::Locations:
        {
            Location location = Operations::getLocation(id);
            d = new CustomDialog("Вы уверены что хотите удалить локацию " + location.getName() + "?");
            break;
        }
        case Setting::Users:
        {
            User user = Operations::getUser(id);
            d = new CustomDialog("Вы уверены что хотите удалить пользователя " + user.getName() + "?");
            break;
        }
        default:
            // Handle unexpected case
            break;
        }

        if (d)
        {
            connect(d, &CustomDialog::yesClicked, this, &SettingsPage::onYes);
            connect(d, &CustomDialog::noClicked, this, &SettingsPage::onNo);
            d->show();
        }
    }
}


void SettingsPage::onYes()
{
    QItemSelectionModel *select = ui->tableView->selectionModel();
    if (select->hasSelection())
    {
        int row = select->selectedRows().at(0).row();

        int id = ui->tableView->model()->index(row, 0).data().toLongLong();

        switch (this->table)
        {
        case Setting::Drivers:
            Operations::deleteDriver(id);
            break;
        case Setting::Cars:
            Operations::deleteCar(id);
            break;
        case Setting::Investors:
            Operations::deleteInvestor(id);
            break;
        case Setting::Types:
            Operations::deleteType(id);
            break;
        case Setting::Locations:
            Operations::deleteLocation(id);
            break;
        case Setting::Users:
            Operations::deleteUser(id);
            break;
        }

        setTable();
    }
}

void SettingsPage::onNo()
{
}

void SettingsPage::on_ReportButton_clicked()
{
    QItemSelectionModel *select = ui->tableView->selectionModel();
    if (select->hasSelection())
    {
        int row = select->selectedRows().at(0).row();

        int id = ui->tableView->model()->index(row, 0).data().toLongLong();

        switch (this->table)
        {
        case Setting::Drivers:
            nav->openReport(9, id);
            break;
        case Setting::Cars:
            nav->openReport(8, id);
            break;
        case Setting::Investors:
            nav->openReport(7, id);
            break;
        case Setting::Types:
            nav->openReport(6, id);
            break;
        case Setting::Locations:
            nav->openReport(10, id);
            break;
        case Setting::Users:
            nav->openReport(11, id);
            break;
        }
    }
    else
    {
        switch (this->table)
        {
        case Setting::Drivers:
            nav->openReport(3);
            break;
        case Setting::Cars:
            nav->openReport(2);
            break;
        case Setting::Investors:
            nav->openReport(0);
            break;
        case Setting::Types:
            nav->openReport(1);
            break;
        case Setting::Locations:
            nav->openReport(4);
            break;
        case Setting::Users:
            nav->openReport(5);
            break;
        }
    }
}

void SettingsPage::onSectionResized(int logicalIndex, int oldSize, int newSize) {
    if (logicalIndex != ui->tableView->model()->columnCount() - 1) {
        adjustColumnWidths();
    }
}

void SettingsPage::adjustColumnWidths() {
    int totalWidth = 0;
    for (int i = 0; i < ui->tableView->model()->columnCount() - 1; ++i) {
        totalWidth += ui->tableView->columnWidth(i);
    }

    int lastColumnWidth = 1130 - totalWidth;
    if (lastColumnWidth > 150) {
        ui->tableView->setColumnWidth(ui->tableView->model()->columnCount() - 1, lastColumnWidth);
    } else {
        ui->tableView->setColumnWidth(ui->tableView->model()->columnCount() - 1, 150);
    }
}

void SettingsPage::handleDoubleClick(const QModelIndex &index)
{
    if (index.isValid()) {
        QModelIndex firstColumnIndex = index.sibling(index.row(), 0);

        int id = ui->tableView->model()->data(firstColumnIndex).toLongLong();

        addUpdateWindow *w = new addUpdateWindow(this->table, id);
        w->resize(w->minimumSizeHint());
        w->show();

        QEventLoop loop;
        connect(w, SIGNAL(closed()), &loop, SLOT(quit()));
        loop.exec();

        setTable();
    }
}
