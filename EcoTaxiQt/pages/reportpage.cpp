#include "reportpage.h"
#include "ui_reportpage.h"

ReportPage::ReportPage(nm *nav, QWidget *parent)
    : QWidget(parent), ui(new Ui::ReportPage)
{
    ui->setupUi(this);

    this->nav = nav;

    this->fromDate = QDate::currentDate().addDays(-1);
    this->toDate = QDate::currentDate();

    ui->FromDateButton->setProperty("color", "gray");
    ui->ToDateButton->setProperty("color", "gray");

    ui->FromDateButton->setText(this->fromDate.toString("dd.MM.yyyy"));
    ui->ToDateButton->setText(this->toDate.toString("dd.MM.yyyy"));

    connect(ui->tableView->horizontalHeader(), &QHeaderView::sectionResized, this, &ReportPage::onSectionResized);

    connect(ui->tableView->horizontalHeader(), &QHeaderView::sortIndicatorChanged, this, &ReportPage::onSortIndicatorChanged);
}

ReportPage::~ReportPage()
{
    delete ui;
}

void ReportPage::setReport(Report mode, int id, QDate from, QDate to)
{
    this->selectedColumn = -1;

    this->mode = mode;

    if (!from.isNull())
    {
        this->fromDate = from;
        this->toDate = to;

        ui->FromDateButton->setText(this->fromDate.toString("dd.MM.yyyy"));
        ui->ToDateButton->setText(this->toDate.toString("dd.MM.yyyy"));
    }

    if (id > -2)
    {
        this->id = id;
    }

    setHeader();
    setSideTable();

    if (id != 0)
    {
        setTable();
        setBottomTable();
        setTableSizes();
    }
    else
    {
        QStandardItemModel *model = new QStandardItemModel();
        ui->tableView->setModel(model);
    }
}

void ReportPage::setHeader()
{
    switch (this->mode)
    {
    case Report::Cars:
        ui->Header->setText("ПО МАШИНЕ");
        ui->ReportButton->setText("ОТЧЕТ ПО МАШИНАМ");
        break;

    case Report::Drivers:
        ui->Header->setText("ПО ВОДИТЕЛЮ");
        ui->ReportButton->setText("ОТЧЕТ ПО ВОДИТЕЛЯМ");
        break;

    case Report::Investors:
        ui->Header->setText("ПО ИНВЕСТОРУ");
        ui->ReportButton->setText("ОТЧЕТ ПО ИНВЕСТОРАМ");
        break;

    case Report::Types:
        ui->Header->setText("ПО ТИПУ");
        ui->ReportButton->setText("ОТЧЕТ ПО ТИПАМ");
        break;

    case Report::Locations:
        ui->Header->setText("ПО ЛОКАЦИИ");
        ui->ReportButton->setText("ОТЧЕТ ПО ЛОКАЦИЯМ");
        break;

    case Report::Charges:
        ui->Header->setText("ПО ЗАРЯДКЕ");
        ui->ReportButton->setText("ОТЧЕТ ПО ЗАРЯДКАМ");
        break;

    case Report::Users:
    case Report::Users2:
        ui->Header->setText("ПО ПОЛЬЗОВАТЕЛЮ");
        ui->ReportButton->setText("ОТЧЕТ ПО ПОЛЬЗОВАТЕЛЯМ");
        break;
    
    case Report::FinesByCars:
        ui->Header->setText("ПО ШТРАФАМ ПО МАШИНЕ");
        ui->ReportButton->setText("ОТЧЕТ ПО МАШИНАМ");
        break;

    case Report::FinesByDrivers:
        ui->Header->setText("ПО ШТРАФАМ ПО ВОДИТЕЛЮ");
        ui->ReportButton->setText("ОТЧЕТ ПО ВОДИТЕЛЯМ");
        break;
    
    default:
        break;
    }
}

void ReportPage::setTable()
{
    QStandardItemModel *model = new QStandardItemModel();
    switch (this->mode)
    {
    case Report::Cars:
        model->setHorizontalHeaderLabels({"Дата", "Тип", "Водитель", "Сумма", "Описание"});
        for (const QVariant &carData : ReportOperations::getCarReport(this->id, this->fromDate, this->toDate))
        {
            QVariantList car = carData.toList();
            QList<QStandardItem *> row;

            QStandardItem *dateItem = new QStandardItem();
            dateItem->setData(car[0].toDateTime(), Qt::DisplayRole);
            row.append(dateItem);

            // Other columns as string
            row.append(new QStandardItem(car[1].toString())); // Тип
            row.append(new QStandardItem(car[2].toString())); // Водитель

            // Ensure numerical data (Сумма) is set correctly for sorting as integers
            QStandardItem *amountItem = new QStandardItem();
            amountItem->setData(car[3].toInt(), Qt::DisplayRole); // Сумма
            row.append(amountItem);

            row.append(new QStandardItem(car[4].toString())); // Описание

            model->appendRow(row);
        }
        break;
    case Report::Drivers:
        model->setHorizontalHeaderLabels({"Дата", "Тип", "Машина", "Сумма", "Описание"});
        for (const QVariant &driverData : ReportOperations::getDriverReport(this->id, this->fromDate, this->toDate))
        {
            QVariantList driver = driverData.toList();
            QList<QStandardItem *> row;

            QStandardItem *dateItem = new QStandardItem();
            dateItem->setData(driver[0].toDateTime(), Qt::DisplayRole);
            row.append(dateItem);

            // Create QStandardItem for Тип and Машина as strings
            row.append(new QStandardItem(driver[1].toString())); // Тип
            row.append(new QStandardItem(driver[2].toString())); // Машина

            // Ensure numerical data (Сумма) is set correctly for sorting as integers
            QStandardItem *amountItem = new QStandardItem();
            amountItem->setData(driver[3].toInt(), Qt::DisplayRole); // Сумма
            row.append(amountItem);

            // Create QStandardItem for Описание as string
            row.append(new QStandardItem(driver[4].toString())); // Описание

            model->appendRow(row);
        }
        break;
    case Report::Investors:
        model->setHorizontalHeaderLabels({"id", "ID", "Доход", "Налог 5%", "KWH * 10", "Расход", "Общий", "%", "Комиссия", "Инвестору"});
        for (const QVariant &investorData : ReportOperations::getInvestorReport(this->id, this->fromDate, this->toDate))
        {
            QVariantList investor = investorData.toList();
            QList<QStandardItem *> row;
            row.append(new QStandardItem(investor[0].toString()));
            row.append(new QStandardItem(investor[1].toString()));
            row.append(new QStandardItem());
            row[2]->setData(investor[2].toInt(), Qt::DisplayRole);
            row.append(new QStandardItem());
            row[3]->setData(investor[3].toInt(), Qt::DisplayRole);
            row.append(new QStandardItem());
            row[4]->setData(investor[4].toInt(), Qt::DisplayRole);
            row.append(new QStandardItem());
            row[5]->setData(investor[5].toInt(), Qt::DisplayRole);
            row.append(new QStandardItem());
            row[6]->setData(investor[6].toInt(), Qt::DisplayRole);
            row.append(new QStandardItem());
            row[7]->setData(investor[7].toInt(), Qt::DisplayRole);
            row.append(new QStandardItem());
            row[8]->setData(investor[8].toInt(), Qt::DisplayRole);
            row.append(new QStandardItem());
            row[9]->setData(investor[9].toInt(), Qt::DisplayRole);
            model->appendRow(row);
        }
        break;
    case Report::Types:
        model->setHorizontalHeaderLabels({"Дата", "Водитель", "Машина", "Сумма", "Описание"});
        for (const QVariant &typeData : ReportOperations::getTypeReport(this->id, this->fromDate, this->toDate))
        {
            QVariantList type = typeData.toList();
            QList<QStandardItem *> row;

            QStandardItem *dateItem = new QStandardItem();
            dateItem->setData(type[0].toDateTime(), Qt::DisplayRole);
            row.append(dateItem);

            // Create QStandardItem for Водитель, Машина, and Описание as strings
            row.append(new QStandardItem(type[1].toString())); // Водитель
            row.append(new QStandardItem(type[2].toString())); // Машина

            // Ensure numerical data (Сумма) is set correctly for sorting as integers
            QStandardItem *amountItem = new QStandardItem();
            amountItem->setData(type[3].toInt(), Qt::DisplayRole); // Сумма
            row.append(amountItem);

            // Create QStandardItem for Описание as string
            row.append(new QStandardItem(type[4].toString())); // Описание

            model->appendRow(row);
        }
        break;

    case Report::Locations:
        model->setHorizontalHeaderLabels({"Дата", "Машина", "Водитель", "KWH", "Время"});
        for (const QVariant &locationData : ReportOperations::getLocationReport(this->id, this->fromDate, this->toDate))
        {
            QVariantList location = locationData.toList();
            QList<QStandardItem *> row;

            QStandardItem *dateItem = new QStandardItem();
            dateItem->setData(location[0].toDateTime(), Qt::DisplayRole);
            row.append(dateItem);

            // Create QStandardItem for Машина and Водитель as strings
            row.append(new QStandardItem(location[1].toString())); // Машина
            row.append(new QStandardItem(location[2].toString())); // Водитель

            // Ensure numerical data (KWH and Время) is set correctly for sorting as integers
            QStandardItem *kwhItem = new QStandardItem();
            kwhItem->setData(location[3].toInt(), Qt::DisplayRole); // KWH
            row.append(kwhItem);

            QStandardItem *timeItem = new QStandardItem();
            timeItem->setData(location[4].toInt(), Qt::DisplayRole); // Время
            row.append(timeItem);

            model->appendRow(row);
        }
        break;

    case Report::Charges:
        model->setHorizontalHeaderLabels({"Дата", "Водитель", "Локация", "KWH", "Время"});
        for (const QVariant &chargeData : ReportOperations::getChargesByCarReport(this->id, this->fromDate, this->toDate))
        {
            QVariantList charge = chargeData.toList();
            QList<QStandardItem *> row;

            QStandardItem *dateItem = new QStandardItem();
            dateItem->setData(charge[0].toDateTime(), Qt::DisplayRole);
            row.append(dateItem);

            // Create QStandardItem for Водитель and Локация as strings
            row.append(new QStandardItem(charge[1].toString())); // Водитель
            row.append(new QStandardItem(charge[2].toString())); // Локация

            // Ensure numerical data (KWH and Время) is set correctly for sorting as integers
            QStandardItem *kwhItem = new QStandardItem();
            kwhItem->setData(charge[3].toInt(), Qt::DisplayRole); // KWH
            row.append(kwhItem);

            QStandardItem *timeItem = new QStandardItem();
            timeItem->setData(charge[4].toInt(), Qt::DisplayRole); // Время
            row.append(timeItem);

            model->appendRow(row);
        }

        break;

    case Report::Users:
    case Report::Users2:
        model->setHorizontalHeaderLabels({"ID", "Дата", "Тип", "Водитель", "Машина", "Сумма"});
        for (const QVariant &userData : ReportOperations::getUserReport(this->id, this->fromDate, this->toDate))
        {
            QVariantList user = userData.toList();
            QList<QStandardItem *> row;

            row.append(new QStandardItem(user[0].toString())); // ID

            QStandardItem *dateItem = new QStandardItem();
            dateItem->setData(user[1].toDateTime(), Qt::DisplayRole);
            row.append(dateItem); // Дата

            row.append(new QStandardItem(user[2].toString())); // Тип
            row.append(new QStandardItem(user[3].toString())); // Водитель
            row.append(new QStandardItem(user[4].toString())); // Машина

            QStandardItem *amountItem = new QStandardItem();
            amountItem->setData(user[5].toInt(), Qt::DisplayRole);
            row.append(amountItem); // Сумма

            model->appendRow(row);
        }
        break;
    
    case Report::FinesByCars:
        model->setHorizontalHeaderLabels({"Дата", "Водитель", "Сумма", "Оплачен", "Описание"});
        for (const QVariant &fineData : ReportOperations::getFinesByCarReport(this->id, this->fromDate, this->toDate))
        {
            QVariantList fine = fineData.toList();
            QList<QStandardItem *> row;

            QStandardItem *dateItem = new QStandardItem();
            dateItem->setData(fine[1].toDate(), Qt::DisplayRole);
            row.append(dateItem); // Дата

            row.append(new QStandardItem(fine[2].toString())); // Водитель

            QStandardItem *amountItem = new QStandardItem();
            amountItem->setData(fine[3].toInt(), Qt::DisplayRole);
            row.append(amountItem); // Сумма

            row.append(new QStandardItem(fine[4].toBool() ? "Да" : "Нет")); // Оплачен
            row.append(new QStandardItem(fine[5].toString())); // Описание

            model->appendRow(row);
        }
        break;

    case Report::FinesByDrivers:
        model->setHorizontalHeaderLabels({"Дата", "Машина", "Сумма", "Оплачен", "Описание"});
        for (const QVariant &fineData : ReportOperations::getFinesByDriverReport(this->id, this->fromDate, this->toDate))
        {
            QVariantList fine = fineData.toList();
            QList<QStandardItem *> row;

            QStandardItem *dateItem = new QStandardItem();
            dateItem->setData(fine[1].toDate(), Qt::DisplayRole);
            row.append(dateItem); // Дата

            row.append(new QStandardItem(fine[2].toString())); // Машина

            QStandardItem *amountItem = new QStandardItem();
            amountItem->setData(fine[3].toInt(), Qt::DisplayRole);
            row.append(amountItem); // Сумма

            row.append(new QStandardItem(fine[4].toBool() ? "Да" : "Нет")); // Оплачен
            row.append(new QStandardItem(fine[5].toString())); // Описание

            model->appendRow(row);
        }
        break;
    
    default:
        break;
    }

    ui->tableView->setModel(model);

    if (this->mode == Report::Investors)
    {
        ui->tableView->setColumnHidden(0, true);
        ui->ItemButton->setDisabled(false);
    }
    else
    {
        ui->tableView->setColumnHidden(0, false);
        ui->ItemButton->setDisabled(true);
    }
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);

    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    ui->tableView->horizontalHeader()->setCascadingSectionResizes(true);

    if (this->selectedColumn != -1)
        ui->tableView->sortByColumn(this->selectedColumn, this->sortOrder);
}

void ReportPage::setBottomTable()
{
    if (this->id > 0) {
    QStandardItemModel *model = new QStandardItemModel();
    switch (this->mode)
    {
    case Report::Cars:
        if (true)
        {
            QVariantList report = ReportOperations::getAllCarReport(this->id, this->fromDate, this->toDate);
            model->setHorizontalHeaderLabels({
                "Итого",
                "Доход",
                "Налог 5%",
                "KWH * 10",
                "Расход",
                "Общая",
                "%",
                "Комиссия",
                "Инвестору",
            });

            QList<QStandardItem *> row;
            row.append(new QStandardItem("Итого"));
            row.append(new QStandardItem(report[0].toString()));
            row.append(new QStandardItem(report[1].toString()));
            row.append(new QStandardItem(report[2].toString()));
            row.append(new QStandardItem(report[3].toString()));
            row.append(new QStandardItem(report[4].toString()));
            row.append(new QStandardItem(report[5].toString() + "%"));
            row.append(new QStandardItem(report[6].toString()));
            row.append(new QStandardItem(report[7].toString()));
            model->appendRow(row);
        }
        break;
    case Report::Drivers:
        if (true)
        {
            QVariantList report = ReportOperations::getAllDriverReport(this->id, this->fromDate, this->toDate);
            model->setHorizontalHeaderLabels({
                "Итого",
                "Доход",
                "KWH * 10",
                "Расход",
                "Оборот"
            });

            QList<QStandardItem *> row;
            row.append(new QStandardItem("Итого"));
            row.append(new QStandardItem(report[0].toString()));
            row.append(new QStandardItem(report[1].toString()));
            row.append(new QStandardItem(report[2].toString()));
            row.append(new QStandardItem(report[3].toString()));
            model->appendRow(row);
        }
        break;
    case Report::Investors:
        if (true)
        {
            QVariantList report = ReportOperations::getAllInvestorReport(this->id, this->fromDate, this->toDate);
            model->setHorizontalHeaderLabels({
                "Итого",
                "Доход",
                "Налог 5%",
                "KWH * 10",
                "Расход",
                "Общая",
                "Машин",
                "Комиссия",
                "Инвестору",
            });

            QList<QStandardItem *> row;
            row.append(new QStandardItem("Итого"));
            row.append(new QStandardItem(report[1].toString()));
            row.append(new QStandardItem(report[2].toString()));
            row.append(new QStandardItem(report[3].toString()));
            row.append(new QStandardItem(report[4].toString()));
            row.append(new QStandardItem(report[5].toString()));
            row.append(new QStandardItem(report[6].toString()));
            row.append(new QStandardItem(report[7].toString()));
            row.append(new QStandardItem(report[8].toString()));
            model->appendRow(row);
        }
        break;
    case Report::Types:
        if (true)
        {
            QVariantList report = ReportOperations::getAllTypeReport(this->id, this->fromDate, this->toDate);
            model->setHorizontalHeaderLabels({
                "Итого",
                "Доход",
                "Расход",
                "Прибыль"
            });

            QList<QStandardItem *> row;
            row.append(new QStandardItem("Итого"));
            row.append(new QStandardItem(report[0].toString()));
            row.append(new QStandardItem(report[1].toString()));
            row.append(new QStandardItem(report[2].toString()));
            model->appendRow(row);
        }
        break;
    case Report::Locations:
        if (true)
        {
            QVariantList report = ReportOperations::getAllLocationReport(this->id, this->fromDate, this->toDate);
            model->setHorizontalHeaderLabels({
                "Итого",
                "KWH",
            });

            QList<QStandardItem *> row;
            row.append(new QStandardItem("Итого"));
            row.append(new QStandardItem(report[0].toString()));
            model->appendRow(row);
        }
        break;
    case Report::Charges:
        if (true)
        {
            QVariantList report = ReportOperations::getAllChargesByCarReport(this->id, this->fromDate, this->toDate);
            model->setHorizontalHeaderLabels({
                "Итого",
                "KWH",
            });

            QList<QStandardItem *> row;
            row.append(new QStandardItem("Итого"));
            row.append(new QStandardItem(report[0].toString()));
            model->appendRow(row);
        }
        break;

    case Report::Users:
    case Report::Users2:
        if (true)
        {
            QVariantList report = ReportOperations::getAllUserReport(this->id, this->fromDate, this->toDate);
            model->setHorizontalHeaderLabels({
                "Итого",
                "Входов",
                "Событий",
            });

            QList<QStandardItem *> row;
            QVariantList data = report[0].toList();
            row.append(new QStandardItem("Итого"));
            row.append(new QStandardItem(data[0].toString()));
            row.append(new QStandardItem(data[1].toString()));
            model->appendRow(row);
        }
        break;
    
    case Report::FinesByCars:
        if (true)
        {
            QVariantList report = ReportOperations::getAllFinesByCarReport(this->id, this->fromDate, this->toDate)[0].toList();
            model->setHorizontalHeaderLabels({
                "Итого",
                "Сумма",
                "Количество",
                "Оплачено",
                "Количество",
                "Не оплачено",
                "Количество",
            });

            QList<QStandardItem *> row;
            row.append(new QStandardItem("Итого"));
            row.append(new QStandardItem(report[0].toString()));
            row.append(new QStandardItem(report[1].toString()));
            row.append(new QStandardItem(report[2].toString()));
            row.append(new QStandardItem(report[3].toString()));
            row.append(new QStandardItem(report[4].toString()));
            row.append(new QStandardItem(report[5].toString()));
            model->appendRow(row);
        }
        break;
    
    case Report::FinesByDrivers:
        if (true)
        {
            QVariantList report = ReportOperations::getAllFinesByDriverReport(this->id, this->fromDate, this->toDate)[0].toList();
            model->setHorizontalHeaderLabels({
                "Итого",
                "Сумма",
                "Количество",
                "Оплачено",
                "Количество",
                "Не оплачено",
                "Количество",
            });

            QList<QStandardItem *> row;
            row.append(new QStandardItem("Итого"));
            row.append(new QStandardItem(report[0].toString()));
            row.append(new QStandardItem(report[1].toString()));
            row.append(new QStandardItem(report[2].toString()));
            row.append(new QStandardItem(report[3].toString()));
            row.append(new QStandardItem(report[4].toString()));
            row.append(new QStandardItem(report[5].toString()));
            model->appendRow(row);
        }
        break;

    default:
        break;
    }

    ui->bottomTable->setModel(model);

    ui->bottomTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    }
    else
    {
        ui->bottomTable->setModel(new QStandardItemModel());
    }
}

void ReportPage::setSideTable()
{
    QStandardItemModel *model = new QStandardItemModel();
    int row = 0;
    switch (this->mode)
    {
    case Report::Cars:
    case Report::Charges:
    case Report::FinesByCars:
        model->setHorizontalHeaderLabels({"id", "Машины"});
        for (Car car : Operations::selectAllCars())
        {
            if (this->id != 0 && car.getId() == this->id)
                row = model->rowCount();
            model->appendRow({new QStandardItem(QString::number(car.getId())), new QStandardItem(car.getSid())});
        }
        break;

    case Report::Drivers:
    case Report::FinesByDrivers:
        model->setHorizontalHeaderLabels({"id", "Водители"});
        for (Driver driver : Operations::selectAllDrivers())
        {
            if (this->id != 0 && driver.getId() == this->id)
                row = model->rowCount();
            model->appendRow({new QStandardItem(QString::number(driver.getId())), new QStandardItem(driver.getName())});
        }
        break;

    case Report::Investors:
        model->setHorizontalHeaderLabels({"id", "Инвесторы"});
        for (Investor investor : Operations::selectAllInvestors())
        {
            if (this->id != 0 && investor.getId() == this->id)
                row = model->rowCount();
            model->appendRow({new QStandardItem(QString::number(investor.getId())), new QStandardItem(investor.getName())});
        }
        break;

    case Report::Types:
        model->setHorizontalHeaderLabels({"id", "Типы"});
        for (Type type : Operations::selectAllTypes())
        {
            if (this->id != 0 && type.getId() == this->id)
                row = model->rowCount();
            model->appendRow({new QStandardItem(QString::number(type.getId())), new QStandardItem(type.getName())});
        }
        break;

    case Report::Locations:
        model->setHorizontalHeaderLabels({"id", "Локации"});
        for (Location location : Operations::selectAllLocations())
        {
            if (this->id != 0 && location.getId() == this->id)
                row = model->rowCount();
            model->appendRow({new QStandardItem(QString::number(location.getId())), new QStandardItem(location.getName())});
        }
        break;

    case Report::Users:
    case Report::Users2:
        model->setHorizontalHeaderLabels({"id", "Пользователи"});
        model->appendRow({new QStandardItem("-1"), new QStandardItem("admin")});
        for (User user : Operations::selectAllUsers())
        {
            if (this->id != 0 && user.getId() == this->id)
                row = model->rowCount();
            model->appendRow({new QStandardItem(QString::number(user.getId())), new QStandardItem(user.getName())});
        }
        break;
    }

    ui->sideTable->setModel(model);

    if (this->id != 0)
        ui->sideTable->selectRow(row);

    ui->sideTable->setColumnHidden(0, true);

    ui->sideTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    connect(ui->sideTable->selectionModel(), &QItemSelectionModel::selectionChanged, this, &ReportPage::selectionChanged);
}

void ReportPage::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    int row = selected.indexes()[0].row();

    this->id = ui->sideTable->model()->index(row, 0).data().toLongLong();

    setTable();

    setBottomTable();

    setTableSizes();
}

void ReportPage::setTableSizes()
{
    switch (this->mode)
    {

    case Report::Cars:
        ui->tableView->setColumnWidth(0, 172);
        ui->tableView->setColumnWidth(1, 172);
        ui->tableView->setColumnWidth(2, 172);
        ui->tableView->setColumnWidth(3, 172);
        ui->tableView->setColumnWidth(4, 172);
        break;

    case Report::Drivers:
        ui->tableView->setColumnWidth(0, 172);
        ui->tableView->setColumnWidth(1, 172);
        ui->tableView->setColumnWidth(2, 172);
        ui->tableView->setColumnWidth(3, 172);
        ui->tableView->setColumnWidth(4, 172);
        break;

    case Report::Investors:
        ui->tableView->setColumnWidth(1, 100);
        ui->tableView->setColumnWidth(2, 130);
        ui->tableView->setColumnWidth(3, 110);
        ui->tableView->setColumnWidth(4, 130);
        ui->tableView->setColumnWidth(5, 130);
        ui->tableView->setColumnWidth(6, 130);
        ui->tableView->setColumnWidth(7, 60);
        ui->tableView->setColumnWidth(8, 130);
        ui->tableView->setColumnWidth(9, 130);
        break;

    case Report::Types:
        ui->tableView->setColumnWidth(0, 172);
        ui->tableView->setColumnWidth(1, 172);
        ui->tableView->setColumnWidth(2, 172);
        ui->tableView->setColumnWidth(3, 172);
        ui->tableView->setColumnWidth(4, 172);
        break;

    case Report::Locations:
        ui->tableView->setColumnWidth(0, 172);
        ui->tableView->setColumnWidth(1, 172);
        ui->tableView->setColumnWidth(2, 172);
        ui->tableView->setColumnWidth(3, 172);
        ui->tableView->setColumnWidth(4, 172);
        break;

    case Report::Charges:
        ui->tableView->setColumnWidth(0, 172);
        ui->tableView->setColumnWidth(1, 172);
        ui->tableView->setColumnWidth(2, 172);
        ui->tableView->setColumnWidth(3, 172);
        ui->tableView->setColumnWidth(4, 172);
        break;

    case Report::Users:
    case Report::Users2:
        ui->tableView->setColumnWidth(0, 172);
        ui->tableView->setColumnWidth(1, 172);
        ui->tableView->setColumnWidth(2, 172);
        ui->tableView->setColumnWidth(3, 172);
        ui->tableView->setColumnWidth(4, 172);
        break;
    
    case Report::FinesByCars:
    case Report::FinesByDrivers:
        ui->tableView->setColumnWidth(0, 160);
        ui->tableView->setColumnWidth(1, 160);
        ui->tableView->setColumnWidth(2, 160);
        ui->tableView->setColumnWidth(3, 160);
        ui->tableView->setColumnWidth(4, 160);
        ui->tableView->setColumnWidth(5, 160);
        break;

    default:
        break;
    }
}

void ReportPage::on_SettingsButton_clicked()
{
    switch (this->mode)
    {
    case Report::Drivers:
        nav->openSettings(0);
        break;
    case Report::Cars:
        nav->openSettings(1);
        break;
    case Report::Investors:
        nav->openSettings(2);
        break;
    case Report::Types:
        nav->openSettings(3);
        break;
    case Report::Locations:
        nav->openSettings(4);
        break;
    case Report::Charges:
        nav->openEvents(1);
        break;
    case Report::Users:
    case Report::Users2:
        nav->openSettings(5);
        break;
    case Report::FinesByCars:
    case Report::FinesByDrivers:
        nav->openFines(0);
        break;
    default:
        break;
    }
}

void ReportPage::on_BackButton_clicked()
{
    nav->changeWindow(0);
}

void ReportPage::on_ReportButton_clicked()
{
    switch (this->mode)
    {
    case Report::Cars:
        nav->openReport(2, 0, fromDate, toDate);
        break;
    case Report::Types:
        nav->openReport(1, 0, fromDate, toDate);
        break;
    case Report::Drivers:
        nav->openReport(3, 0, fromDate, toDate);
        break;
    case Report::Investors:
        nav->openReport(0, 0, fromDate, toDate);
        break;
    case Report::Locations:
        nav->openReport(4, 0, fromDate, toDate);
        break;
    case Report::Charges:
        nav->openReport(6, 0, fromDate, toDate);
        break;
    case Report::Users:
    case Report::Users2:
        nav->openReport(5, 0, fromDate, toDate);
        break;
    case Report::FinesByCars:
        nav->openFines(1, 0, fromDate, toDate);
        break;
    case Report::FinesByDrivers:
        nav->openFines(2, 0, fromDate, toDate);
        break;
    default:
        break;
    }
}

void ReportPage::on_FromDateButton_clicked()
{
    CalendarPage *c = new CalendarPage(this->fromDate);

    connect(c, &CalendarPage::changeDate, this, &ReportPage::setFromDate);

    c->show();
}

void ReportPage::on_ToDateButton_clicked()
{
    CalendarPage *c = new CalendarPage(this->toDate);

    connect(c, &CalendarPage::changeDate, this, &ReportPage::setToDate);

    c->show();
}

void ReportPage::on_ToPDFButton_clicked()
{
    QString title;
    int start = 1;
    switch (this->mode)
    {
    case Report::Cars:
        title = "Отчет по машине " + Operations::getCar(this->id).getSid();
        start = 0;
        break;

    case Report::Drivers:
        title = "Отчет по водителю " + Operations::getDriver(this->id).getName();
        start = 0;
        break;

    case Report::Investors:
        title = "Отчет по инвестору " + Operations::getInvestor(this->id).getName();
        break;

    case Report::Types:
        title = "Отчет по типу " + Operations::getType(this->id).getName();
        start = 0;
        break;

    case Report::Locations:
        title = "Отчет по локации " + Operations::getLocation(this->id).getName();
        start = 0;
        break;

    case Report::Charges:
        title = "Отчет по зарядке для машины " + Operations::getCar(this->id).getSid();
        start = 0;
        break;

    case Report::Users:
    case Report::Users2:
        title = "Отчет по пользователю " + Operations::getUser(this->id).getName();
        break;
    
    case Report::FinesByCars:
        title = "Отчет по штрафам по машине " + Operations::getCar(this->id).getSid();
        start = 0;
        break;
    
    case Report::FinesByDrivers:
        title = "Отчет по штрафам по водителю " + Operations::getDriver(this->id).getName();
        start = 0;
        break;

    case Report::Debts:
        break;
    }

    PDFmanager::exportToPDF(title, this->fromDate.toString("dd.MM.yyyy") + " - " + this->toDate.toString("dd.MM.yyyy"), { ui->tableView->model(), ui->bottomTable->model() }, start);
}

void ReportPage::setFromDate(QDate date)
{
    this->fromDate = date;
    ui->FromDateButton->setText(date.toString("dd.MM.yyyy"));
}

void ReportPage::setToDate(QDate date)
{
    this->toDate = date;
    ui->ToDateButton->setText(date.toString("dd.MM.yyyy"));
}

void ReportPage::on_FilterButton_clicked()
{
    setTable();
    setBottomTable();

    setTableSizes();
}

void ReportPage::onSectionResized(int logicalIndex, int oldSize, int newSize)
{
    if (logicalIndex != ui->tableView->model()->columnCount() - 1)
    {
        adjustColumnWidths();
    }
}

void ReportPage::adjustColumnWidths()
{
    int totalWidth = 0;
    for (int i = 0; i < ui->tableView->model()->columnCount() - 1; ++i)
    {
        totalWidth += ui->tableView->columnWidth(i);
    }

    int lastColumnWidth = 860 - totalWidth;
    if (lastColumnWidth > 150)
    {
        ui->tableView->setColumnWidth(ui->tableView->model()->columnCount() - 1, lastColumnWidth);
    }
    else
    {
        ui->tableView->setColumnWidth(ui->tableView->model()->columnCount() - 1, 150);
    }
}

void ReportPage::on_ItemButton_clicked()
{
    QItemSelectionModel *select = ui->tableView->selectionModel();

    if (select->hasSelection())
    {
        int row = select->selectedRows().at(0).row();

        int id = ui->tableView->model()->index(row, 0).data().toLongLong();

        switch (this->mode)
        {
        case Report::Investors:
            nav->openReport(9, id, fromDate, toDate);
            break;
        default:
            break;
        }
    }
}

void ReportPage::onSortIndicatorChanged(int logicalIndex, Qt::SortOrder order) {
    this->selectedColumn = logicalIndex;
    this->sortOrder = order;
}
