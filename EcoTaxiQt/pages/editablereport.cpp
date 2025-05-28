#include "editablereport.h"
#include "ui_editablereport.h"
#include <QDate>
#include <QDebug>

EditableReport::EditableReport(nm *nav, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::EditableReport)
{
    ui->setupUi(this);
    this->nav = nav;

    QDate currentDate = QDate::currentDate();
    QDate startDate = QDate(currentDate.year(), 1, 1); // January 1st of the current year

    ui->startDateEdit->setDate(startDate);
    ui->endDateEdit->setDate(currentDate);

    populateComboBoxes();

    connect(ui->startDateEdit, &QDateEdit::dateChanged, this, &EditableReport::setTable);
    connect(ui->endDateEdit, &QDateEdit::dateChanged, this, &EditableReport::setTable);
    connect(ui->carFilterComboBox, &QComboBox::currentTextChanged, this, &EditableReport::setTable);
    connect(ui->driverFilterComboBox, &QComboBox::currentTextChanged, this, &EditableReport::setTable);
    connect(ui->descriptionFilterLineEdit, &QLineEdit::textChanged, this, &EditableReport::setTable);
    connect(ui->searchLineEdit, &QLineEdit::textChanged, this, &EditableReport::setTable);
    // Add this for days filter (for repairs)
    connect(ui->daysFilterLineEdit, &QLineEdit::textChanged, this, &EditableReport::setTable);
}

void EditableReport::populateComboBoxes()
{
    // Get unique values for car and driver (fines)
    QStringList cars, drivers;
    QVariantList fines = ReportOperations::getFinesReport();
    for (const QVariant &fine : fines) {
        QVariantList rp = fine.toList();
        QString car = rp[2].toString(); // carLicensePlate
        QString driver = rp[3].toString();
        if (!cars.contains(car)) cars << car;
        if (!drivers.contains(driver)) drivers << driver;
    }
    // Store for use in openReport
    ui->carFilterComboBox->setProperty("finesCars", QVariant::fromValue(cars));
    ui->driverFilterComboBox->setProperty("finesDrivers", QVariant::fromValue(drivers));

    // Get unique values for car and investor (repairs)
    QStringList repairCars, repairInvestors;
    QVariantList repairs = ReportOperations::getRepairsReport();
    for (const QVariant &repair : repairs) {
        QVariantList rp = repair.toList();
        QString car = rp[1].toString(); // carSid
        QString investor = rp[2].toString();
        if (!repairCars.contains(car)) repairCars << car;
        if (!repairInvestors.contains(investor)) repairInvestors << investor;
    }
    ui->carFilterComboBox->setProperty("repairCars", QVariant::fromValue(repairCars));
    ui->driverFilterComboBox->setProperty("repairInvestors", QVariant::fromValue(repairInvestors));
}

void EditableReport::openReport(eSetting mode)
{
    this->mode = mode;
    switch (this->mode) {
    case eSetting::Repairs: {
        ui->EcoTaxi->setText("РЕМОНТ");
        ui->startDateEdit->show();
        ui->endDateEdit->show();
        ui->carFilterComboBox->show();
        ui->driverFilterComboBox->show(); // used for investor in repairs
        ui->daysFilterLineEdit->show();
        ui->descriptionFilterLineEdit->show();
        ui->searchLineEdit->show(); // <-- enable search bar for repairs
        if (QLabel *label = this->findChild<QLabel*>("label_4")) label->show();
        if (QLabel *label = this->findChild<QLabel*>("label")) label->show();
        if (QLabel *label = this->findChild<QLabel*>("label_2")) { label->show(); label->setText("Инвестор:"); }
        if (QLabel *label = this->findChild<QLabel*>("label_3")) label->show();
        if (QLabel *label = this->findChild<QLabel*>("label_days")) label->show();

        // Set car/investor combos for repairs
        ui->carFilterComboBox->blockSignals(true);
        ui->carFilterComboBox->clear();
        ui->carFilterComboBox->addItem("");
        QStringList repairCars = ui->carFilterComboBox->property("repairCars").toStringList();
        ui->carFilterComboBox->addItems(repairCars);
        ui->carFilterComboBox->blockSignals(false);

        ui->driverFilterComboBox->blockSignals(true);
        ui->driverFilterComboBox->clear();
        ui->driverFilterComboBox->addItem("");
        QStringList repairInvestors = ui->driverFilterComboBox->property("repairInvestors").toStringList();
        ui->driverFilterComboBox->addItems(repairInvestors);
        ui->driverFilterComboBox->blockSignals(false);
        break;
    }
    case eSetting::Fines: {
        ui->EcoTaxi->setText("ШТРАФЫ");
        ui->startDateEdit->show();
        ui->endDateEdit->show();
        ui->carFilterComboBox->show();
        ui->driverFilterComboBox->show();
        ui->daysFilterLineEdit->hide();
        ui->descriptionFilterLineEdit->show();
        ui->searchLineEdit->show();
        if (QLabel *label = this->findChild<QLabel*>("label_4")) label->show();
        if (QLabel *label = this->findChild<QLabel*>("label")) label->show();
        if (QLabel *label = this->findChild<QLabel*>("label_2")) { label->show(); label->setText("Водитель:"); }
        if (QLabel *label = this->findChild<QLabel*>("label_3")) label->show();
        if (QLabel *label = this->findChild<QLabel*>("label_days")) label->hide();

        // Set car/driver combos for fines
        ui->carFilterComboBox->blockSignals(true);
        ui->carFilterComboBox->clear();
        ui->carFilterComboBox->addItem("");
        QStringList finesCars = ui->carFilterComboBox->property("finesCars").toStringList();
        ui->carFilterComboBox->addItems(finesCars);
        ui->carFilterComboBox->blockSignals(false);

        ui->driverFilterComboBox->blockSignals(true);
        ui->driverFilterComboBox->clear();
        ui->driverFilterComboBox->addItem("");
        QStringList finesDrivers = ui->driverFilterComboBox->property("finesDrivers").toStringList();
        ui->driverFilterComboBox->addItems(finesDrivers);
        ui->driverFilterComboBox->blockSignals(false);
        break;
    }
    default:
        break;
    }
    setTable();
    setTableSize();
}

EditableReport::~EditableReport()
{
    delete ui;
}

void EditableReport::setTable()
{
    QStandardItemModel *model = new QStandardItemModel();
    QDate startDate = ui->startDateEdit->date();
    QDate endDate = ui->endDateEdit->date();

    switch (this->mode) {
    case eSetting::Repairs: {
        QString carFilter = ui->carFilterComboBox->currentText();
        QString investorFilter = ui->driverFilterComboBox->currentText();
        QString daysFilter = ui->daysFilterLineEdit->text();
        QString descriptionFilter = ui->descriptionFilterLineEdit->text();
        QString searchText = ui->searchLineEdit->text(); // Search bar text
        model->setHorizontalHeaderLabels({ "ID", "Машина", "Инвестор", "Дней", "От", "До", "Описание" });

        QVariantList report = ui->checkBox->isChecked()
            ? ReportOperations::getNotDoneRepairsReport()
            : ReportOperations::getRepairsReport();

        for (const QVariant &repair : report) {
            QVariantList rp = repair.toList();
            QString carSid = rp[1].toString();
            QString investor = rp[2].toString();
            int days = rp[3].toInt();
            QDate fromDate = QDate::fromString(rp[4].toString(), "yyyy-MM-dd");
            QDate toDate = QDate::fromString(rp[5].toString(), "yyyy-MM-dd");
            QString desc = rp[6].toString();

            // Apply filters
            if (!carFilter.isEmpty() && carSid != carFilter)
                continue;
            if (!investorFilter.isEmpty() && investor != investorFilter)
                continue;
            if (!daysFilter.isEmpty() && days != daysFilter.toInt())
                continue;
            if (!descriptionFilter.isEmpty() && !desc.contains(descriptionFilter, Qt::CaseInsensitive))
                continue;

            // Apply search bar logic: match any column
            if (!searchText.isEmpty() &&
                !rp[0].toString().contains(searchText, Qt::CaseInsensitive) && // ID
                !carSid.contains(searchText, Qt::CaseInsensitive) &&          // Машина
                !investor.contains(searchText, Qt::CaseInsensitive) &&        // Инвестор
                !QString::number(days).contains(searchText, Qt::CaseInsensitive) && // Дней
                !rp[4].toString().contains(searchText, Qt::CaseInsensitive) && // От
                !rp[5].toString().contains(searchText, Qt::CaseInsensitive) && // До
                !desc.contains(searchText, Qt::CaseInsensitive))              // Описание
                continue;

            // Apply date filter
            if (fromDate.isValid() && (fromDate < startDate || fromDate > endDate))
                continue;

            QList<QStandardItem *> row;
            row << new QStandardItem(rp[0].toString());
            row << new QStandardItem(carSid);
            row << new QStandardItem(investor);
            QStandardItem *item = new QStandardItem();
            item->setData(days, Qt::DisplayRole);
            row << item;
            row << new QStandardItem(rp[4].toString());
            row << new QStandardItem(rp[5].toString());
            row << new QStandardItem(desc);
            model->appendRow(row);
        }
        break;
    }
    case eSetting::Fines: {
        QString carFilter = ui->carFilterComboBox->currentText();
        QString driverFilter = ui->driverFilterComboBox->currentText();
        QString descriptionFilter = ui->descriptionFilterLineEdit->text();
        QString searchText = ui->searchLineEdit->text();
        model->setHorizontalHeaderLabels({ "ID", "Дата", "Машина", "Водитель", "Сумма", "Оплачен", "Описание" });

        QVariantList report = ui->checkBox->isChecked()
            ? ReportOperations::getNotPaidFinesReport()
            : ReportOperations::getFinesReport();

        for (const QVariant &fine : report) {
            QVariantList rp = fine.toList();
            QDate fineDate = rp[1].toDate();
            QString carLicensePlate = rp[2].toString();
            QString driverName = rp[3].toString();
            QString fineDescription = rp[6].toString();

            if (fineDate < startDate || fineDate > endDate)
                continue;
            if (!carFilter.isEmpty() && carLicensePlate != carFilter)
                continue;
            if (!driverFilter.isEmpty() && driverName != driverFilter)
                continue;
            if (!descriptionFilter.isEmpty() && !fineDescription.contains(descriptionFilter, Qt::CaseInsensitive))
                continue;

            // Apply search bar logic: match any column
            if (!searchText.isEmpty() &&
                !rp[0].toString().contains(searchText, Qt::CaseInsensitive) && // ID
                !fineDate.toString("yyyy-MM-dd").contains(searchText, Qt::CaseInsensitive) && // Дата
                !carLicensePlate.contains(searchText, Qt::CaseInsensitive) && // Машина
                !driverName.contains(searchText, Qt::CaseInsensitive) &&      // Водитель
                !rp[4].toString().contains(searchText, Qt::CaseInsensitive) && // Сумма
                !rp[5].toString().contains(searchText, Qt::CaseInsensitive) && // Оплачен
                !fineDescription.contains(searchText, Qt::CaseInsensitive))    // Описание
                continue;

            QList<QStandardItem *> row;
            row << new QStandardItem(rp[0].toString());
            row << new QStandardItem();
            row[1]->setData(rp[1].toDate(), Qt::DisplayRole);
            row << new QStandardItem(carLicensePlate);
            row << new QStandardItem(driverName);
            row << new QStandardItem();
            row[4]->setData(rp[4].toInt(), Qt::DisplayRole);
            row << new QStandardItem(rp[5].toBool() ? "Да" : "Нет");
            row << new QStandardItem(fineDescription);
            model->appendRow(row);
        }
        break;
    }
    default:
        break;
    }

    ui->tableView->setModel(model);
}

void EditableReport::setTableSize()
{
    ui->tableView->setColumnHidden(0, true);

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);

    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    ui->tableView->horizontalHeader()->setCascadingSectionResizes(true);

    switch (this->mode) {
    case eSetting::Repairs:
    case eSetting::Fines:
        ui->tableView->setColumnWidth(1, 200);
        ui->tableView->setColumnWidth(2, 200);
        ui->tableView->setColumnWidth(3, 200);
        ui->tableView->setColumnWidth(4, 200);
        ui->tableView->setColumnWidth(5, 200);
        ui->tableView->setColumnWidth(6, 200);
        break;
    default:
        break;
    }
}

void EditableReport::on_AddButton_clicked()
{
    AddUpdateRepair *w = new AddUpdateRepair(this->mode, -1);
    w->resize(w->minimumSizeHint());
    w->show();

    QEventLoop loop;
    connect(w, SIGNAL(closed()), &loop, SLOT(quit()));
    loop.exec();

    setTable();
}

void EditableReport::on_EditButton_clicked()
{
    QItemSelectionModel *select = ui->tableView->selectionModel();
    if (select->hasSelection())
    {
        int row = select->selectedRows().at(0).row();

        int id = ui->tableView->model()->index(row, 0).data().toLongLong();

        AddUpdateRepair *w = new AddUpdateRepair(this->mode, id);
        w->resize(w->minimumSizeHint());
        w->show();

        QEventLoop loop;
        connect(w, SIGNAL(closed()), &loop, SLOT(quit()));
        loop.exec();

        setTable();
    }
}

void EditableReport::on_BackButton_clicked()
{
    nav->changeWindow(0);
}

void EditableReport::on_DeleteButton_clicked()
{
    QItemSelectionModel *select = ui->tableView->selectionModel();
    if (select->hasSelection())
    {
        int row = select->selectedRows().at(0).row();
        int id = ui->tableView->model()->index(row, 0).data().toLongLong();
        QString text;
        switch (this->mode) {
        case eSetting::Repairs:
            text = "Вы уверены что хотите удалить ремонт машины " + ui->tableView->model()->index(row, 1).data().toString() + "?";
            break;
        case eSetting::Fines:
            text = "Вы уверены что хотите удалить штраф машины " + ui->tableView->model()->index(row, 2).data().toString() + ", водителя " + ui->tableView->model()->index(row, 3).data().toString() + ", на сумму " + ui->tableView->model()->index(row, 4).data().toString() + "?";
            break;
        default:
            break;
        }
        CustomDialog *d = new CustomDialog(text);
        connect(d, &CustomDialog::yesClicked, this, &EditableReport::onYes);
        connect(d, &CustomDialog::noClicked, this, &EditableReport::onNo);
        d->show();
    }
}

void EditableReport::onYes()
{
    QItemSelectionModel *select = ui->tableView->selectionModel();
    if (select->hasSelection())
    {
        int row = select->selectedRows().at(0).row();

        int id = ui->tableView->model()->index(row, 0).data().toLongLong();

        switch (this->mode)
        {
        case eSetting::Repairs:
            Operations::deleteRepair(id);
            break;
        case eSetting::Fines:
            Operations::deleteFine(id);
            break;
        default:
            break;
        }

        setTable();
    }
}

void EditableReport::onNo()
{
}

void EditableReport::on_checkBox_checkStateChanged(Qt::CheckState state)
{
    setTable();
}

void EditableReport::on_ToPDFButton_clicked()
{
    QString title;
    QString type;
    switch (this->mode)
    {
        case eSetting::Repairs:
            title = "Ремонты";
            type = ui->checkBox->isChecked() ? "Не завершенные" : "Все";
            break;
        case eSetting::Fines:
            title = "Штрафы";
            type = ui->checkBox->isChecked() ? "Не оплаченные" : "Все";
            break;
        default:
            break;
    }
    PDFmanager::exportToPDF(title, type, { ui->tableView->model() });
}
