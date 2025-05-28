#include "addupdaterepair.h"
#include "ui_addupdaterepair.h"

AddUpdateRepair::AddUpdateRepair(eSetting mode, int id, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AddUpdateRepair)
{
    ui->setupUi(this);

    this->mode = mode;

    setAttribute(Qt::WA_DeleteOnClose);

    ui->AmountEdit->setValidator(new QIntValidator());

    ui->frame->setProperty("color", "green");

    this->id = id;

    if (id > 0)
    {
        ui->pushButton->setText("Сохранить");
    }
    else
    {
        ui->pushButton->setText("Добавить");
    }

    ui->CarIdBox->addItem("-");
    carsId_list = Operations::selectAllCars();
    foreach (Car car, carsId_list) {
        QString show = car.getSid();
        carsId.insert(show, car.getId());
        carsSid.insert(car.getId(), show);
        ui->CarIdBox->addItem(show);
    }

    ui->CarIdLabel->setProperty("color", "white");

    ui->FromDateLabel->setProperty("color", "white");
    ui->fromButton->setProperty("color", "gray");

    ui->DescLabel->setProperty("color", "white");

    switch (this->mode) {
    case eSetting::Repairs:
        ui->ToDateLabel->setProperty("color", "white");
        ui->toButton->setProperty("color", "gray");
        ui->AmountFrame->hide();
        ui->DriversBox->hide();
        ui->isPaidFrame->hide();

        if (id > 0)
        {
            QVariantList repair = Operations::getRepair(id);

            ui->CarIdBox->setCurrentText(carsSid.value(repair[1].toInt()));
            this->from = repair[2].toDate();
            ui->fromButton->setText(this->from.toString("dd.MM.yyyy"));
            this->to = repair[3].toDate();
            ui->toButton->setText(this->to.isValid() ? this->to.toString("dd.MM.yyyy") : "-");
            ui->DescEdit->setText(repair[4].toString());
        }
        else
        {
            this->from = QDate::currentDate();
            ui->fromButton->setText(this->from.toString("dd.MM.yyyy"));
        }
        break;
    case eSetting::Fines:
        ui->AmountLabel->setProperty("color", "white");
        ui->DriversLabel->setProperty("color", "white");
        ui->isPaidCheckBox->setProperty("color", "white");

        ui->DriversBox->addItem("-");
        driversId_list = Operations::selectAllDrivers();
        foreach (Driver driver, driversId_list) {
            QString show = driver.getName();
            driversId.insert(show, driver.getId());
            driversName.insert(driver.getId(), show);
            ui->DriversBox->addItem(show);
        }

        if (id > 0)
        {
            QVariantList fine = Operations::getFine(id);

            this->from = fine[1].toDate();
            ui->fromButton->setText(this->from.toString("dd.MM.yyyy"));
            ui->CarIdBox->setCurrentText(carsSid.value(fine[2].toInt()));
            ui->DriversBox->setCurrentText(driversName.value(fine[3].toInt()));
            ui->AmountEdit->setText(fine[4].toString());
            ui->isPaidCheckBox->setChecked(fine[5].toBool());
            ui->DescEdit->setText(fine[6].toString());
        }
        else
        {
            this->from = QDate::currentDate();
            ui->fromButton->setText(this->from.toString("dd.MM.yyyy"));
        }

        ui->ToDateFrame->hide();
        break;
    default:
        break;
    }



}

AddUpdateRepair::~AddUpdateRepair()
{
    delete ui;
}

void AddUpdateRepair::on_pushButton_clicked()
{
    addRecord();
}

void AddUpdateRepair::addRecord()
{
    if (checkFill()) {
        if (id > 0)
        {
            switch (this->mode) {
            case eSetting::Repairs:
                Operations::updateRepair(id, carsId.value(ui->CarIdBox->currentText()), this->from, this->to, ui->DescEdit->toPlainText());
                break;
            case eSetting::Fines:
                Operations::updateFine(id, this->from, carsId.value(ui->CarIdBox->currentText()), driversId.value(ui->DriversBox->currentText()), ui->AmountEdit->text().toLongLong(), ui->isPaidCheckBox->isChecked(), ui->DescEdit->toPlainText());
                break;
            default:
                break;
            }
        }
        else
        {
            switch (this->mode) {
            case eSetting::Repairs:
                Operations::addRepair(carsId.value(ui->CarIdBox->currentText()), this->from, this->to, ui->DescEdit->toPlainText());
                break;
            case eSetting::Fines:
                Operations::addFine(this->from, carsId.value(ui->CarIdBox->currentText()), driversId.value(ui->DriversBox->currentText()), ui->AmountEdit->text().toLongLong(), ui->isPaidCheckBox->isChecked(), ui->DescEdit->toPlainText());
                break;
            default:
                break;
            }
        }
        emit close();
        this->closed();
    }
    else
    {
        QTimer::singleShot(200, this, &AddUpdateRepair::resetInputColor);
    }
}

bool AddUpdateRepair::checkFill()
{
    bool result = true;
    switch (this->mode) {
    case eSetting::Repairs:
        if (ui->CarIdBox->currentText() == "-")
        {
            ui->CarIdLabel->setProperty("color", "red");
            ui->CarIdBox->setStyleSheet("background-color: red;");
            result = false;
        }
        break;
    case eSetting::Fines:
        if (ui->CarIdBox->currentText() == "-")
        {
            ui->CarIdLabel->setProperty("color", "red");
            ui->CarIdBox->setStyleSheet("background-color: red;");
            result = false;
        }
        if (ui->AmountEdit->text().isEmpty())
        {
            ui->AmountLabel->setProperty("color", "red");
            ui->AmountEdit->setStyleSheet("background-color: red;");
        }
    default:
        break;
    }
    return result;
}

void AddUpdateRepair::resetInputColor()
{
    ui->CarIdLabel->setProperty("color", "white");
    ui->DescLabel->setProperty("color", "white");
    ui->AmountLabel->setProperty("color", "white");
    ui->DriversLabel->setProperty("color", "white");

    ui->CarIdBox->setStyleSheet("background-color: white;");
    ui->DescEdit->setStyleSheet("background-color: white;");
    ui->AmountEdit->setStyleSheet("background-color: white;");
    ui->DriversBox->setStyleSheet("background-color: white;");

}

void AddUpdateRepair::on_fromButton_clicked()
{
    CalendarPage *c = new CalendarPage(this->from);

    connect(c, &CalendarPage::changeDate, this, &AddUpdateRepair::setFromDate);

    c->show();
}

void AddUpdateRepair::on_toButton_clicked()
{
    CalendarPage *c = new CalendarPage(this->to.isValid() ? this->to : QDate::currentDate());

    connect(c, &CalendarPage::changeDate, this, &AddUpdateRepair::setToDate);

    c->show();
}

void AddUpdateRepair::setFromDate(QDate date)
{
    this->from = date;
    ui->fromButton->setText(date.toString("dd.MM.yyyy"));
}

void AddUpdateRepair::setToDate(QDate date)
{
    this->to = date;
    ui->toButton->setText(date.toString("dd.MM.yyyy"));
}
