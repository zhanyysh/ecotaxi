#include "addupdatewindowevents.h"
#include "ui_addupdatewindowevents.h"

addupdatewindowEvents::addupdatewindowEvents(Events mode, int id, QWidget *parent)
    : QWidget(parent), ui(new Ui::addupdatewindowEvents)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose);

    ui->frame->setProperty("color", "green");

    this->mode = mode;
    this->id = id;

    ui->AmountEdit->setValidator(new QDoubleValidator());
    ui->KWTEdit->setValidator(new QIntValidator());
    ui->DurationEdit->setValidator(new QIntValidator());
    ui->DolgEdit->setValidator(new QDoubleValidator());

    this->setWindowModality(Qt::ApplicationModal);

    if (id > 0)
    {
        ui->pushButton->setText("Сохранить");
    }
    else
    {
        ui->pushButton->setText("Добавить");
    }

    ui->DriverIdBox->addItem("-");
    driversId_list = Operations::selectAllDrivers();
    foreach (Driver driver, driversId_list) {
        QString show = driver.getName();
        driversId.insert(show, driver.getId());
        ui->DriverIdBox->addItem(show);
    }

    ui->CarIdBox->addItem("-");
    carsId_list = Operations::selectAllCars();
    foreach (Car car, carsId_list) {
        QString show = car.getSid();
        carsId.insert(show, car.getId());
        ui->CarIdBox->addItem(show);
    }

    switch (this->mode) {
    case Events::Events:
        ui->LocationFrame->hide();
        ui->KWTFrame->hide();
        ui->DurationFrame->hide();
        ui->DateTimeLabel->setProperty("color", "white");
        ui->TypeLabel->setProperty("color", "white");
        ui->CarIdLabel->setProperty("color", "white");
        ui->DriverIdLabel->setProperty("color", "white");
        ui->AmountLabel->setProperty("color", "white");
        ui->DescLabel->setProperty("color", "white");
        ui->DolgLabel->setProperty("color", "white");

        types_list = Operations::selectAllTypes();
        foreach (Type type, types_list) {
            QString show = type.getName();
            types.insert(show, type.getId());
            ui->TypeBox->addItem(show);
        }

        if (id > 0) {
            ui->Header->setText("Редактировать событие");
            Event event = Operations::getEvent(id);
            this->date = event.getDate();
            ui->DateTimeEdit->setDateTime(event.getDate());
            ui->TypeBox->setCurrentText(Operations::getType(event.getTypeId()).getName());
            ui->CarIdBox->setCurrentText(Operations::getCar(event.getCarId()).getSid());
            ui->DriverIdBox->setCurrentText(Operations::getDriver(event.getDriverId()).getName());
            ui->AmountEdit->setText(QString::number(event.getAmount()));
            ui->DescEdit->setText(event.getDescription());
            ui->DolgEdit->setText(QString::number(event.getDolg()));

            // Проверяем долг и устанавливаем состояние кнопки "Оплатить"
            if (event.getDolg() == 0) {
                ui->paidButton->setDisabled(true);
                ui->paidButton->setText("Оплачено");
            }
        }
        break;
    case Events::Charges:
        ui->TypeFrame->hide();
        ui->AmountFrame->hide();
        ui->DescEdit->hide();
        ui->DescLabel->hide();
        ui->DateTimeLabel->setProperty("color", "white");
        ui->KWTLabel->setProperty("color", "white");
        ui->DurationLabel->setProperty("color", "white");
        ui->LocationLabel->setProperty("color", "white");
        ui->CarIdLabel->setProperty("color", "white");
        ui->DriverIdLabel->setProperty("color", "white");

        locations_list = Operations::selectAllLocations();
        foreach (Location location, locations_list) {
            QString show = location.getName();
            locations.insert(show, location.getId());
            ui->LocationBox->addItem(show);
        }

        if (id > 0) {
            ui->Header->setText("Редактировать зарядку");
            Charge charge = Operations::getCharge(id);
            this->date = charge.getDate();
            ui->DateTimeEdit->setDateTime(charge.getDate());
            ui->KWTEdit->setText(QString::number(charge.getKwh()));
            ui->DurationEdit->setText(QString::number(charge.getDuration()));
            ui->LocationBox->setCurrentText(Operations::getLocation(charge.getLocationId()).getName());
            ui->CarIdBox->setCurrentText(Operations::getCar(charge.getCarId()).getSid());
            ui->DriverIdBox->setCurrentText(Operations::getDriver(charge.getDriverId()).getName());
        }
    }
}

addupdatewindowEvents::~addupdatewindowEvents()
{
    delete ui;
}

void addupdatewindowEvents::on_pushButton_clicked()
{
    addRecord();
}

void addupdatewindowEvents::addRecord() {
    if (checkFill()) {
        if (id > 0) {
            switch (this->mode) {
            case Events::Events:
                Operations::updateEvent(Event(QVariantList::fromList({
                    id,
                    carsId.value(ui->CarIdBox->currentText()),
                    driversId.value(ui->DriverIdBox->currentText()),
                    types.value(ui->TypeBox->currentText()),
                    ui->DolgEdit->text().toFloat(),
                    ui->AmountEdit->text().toFloat(),
                    ui->DescEdit->toPlainText(),
                    ui->DateTimeEdit->dateTime()
                })));
                break;
            case Events::Charges:
                Operations::updateCharge(Charge(QVariantList::fromList({id, carsId.value(ui->CarIdBox->currentText()), driversId.value(ui->DriverIdBox->currentText()), locations.value(ui->LocationBox->currentText()), ui->KWTEdit->text().toLongLong(), ui->DurationEdit->text().toLongLong(), ui->DateTimeEdit->dateTime()})));
            }
        }
        else {
            switch (this->mode) {
            case Events::Events:
                Operations::addEvent(Event(QVariantList::fromList({
                    0,
                    carsId.value(ui->CarIdBox->currentText()),
                    driversId.value(ui->DriverIdBox->currentText()),
                    types.value(ui->TypeBox->currentText()),
                    ui->DolgEdit->text().toFloat(),
                    ui->AmountEdit->text().toFloat(),
                    ui->DescEdit->toPlainText(),
                    ui->DateTimeEdit->dateTime()
                })));
                break;
            case Events::Charges:
                // Implementation for adding a charge
                break;
            }
        }
        emit close();
        this->closed();
    }
    else {
        QTimer::singleShot(200, this, &addupdatewindowEvents::resetInputColor);
    }
}

bool addupdatewindowEvents::checkFill() {
    bool result = true;
    switch (this->mode) {
    case Events::Events:
        if (ui->AmountEdit->text().isEmpty()) {
            ui->AmountEdit->setStyleSheet("background-color: red;");
            ui->AmountLabel->setStyleSheet("color: red;");
            result = false;
        }
        if (ui->DolgEdit->text().isEmpty()) {
            ui->DolgEdit->setStyleSheet("background-color: red;");
            ui->DolgLabel->setStyleSheet("color: red;");
            result = false;
        }
        break;
    case Events::Charges:
        if (ui->KWTEdit->text().isEmpty()) {
            ui->KWTEdit->setStyleSheet("background-color: red;");
            ui->KWTLabel->setStyleSheet("color: red;");
            result = false;
        }
        if (ui->DurationEdit->text().isEmpty()) {
            ui->DurationEdit->setStyleSheet("background-color: red;");
            ui->DurationLabel->setStyleSheet("color: red;");
            result = false;
        }
        break;
    }
    return result;
}

void addupdatewindowEvents::resetInputColor() {
    ui->AmountEdit->setStyleSheet("");
    ui->AmountLabel->setStyleSheet("color: white;");
    ui->KWTEdit->setStyleSheet("");
    ui->KWTLabel->setStyleSheet("color: white;");
    ui->DurationEdit->setStyleSheet("");
    ui->DurationLabel->setStyleSheet("color: white;");
    ui->LocationBox->setStyleSheet("");
    ui->LocationLabel->setStyleSheet("color: white;");
    ui->CarIdBox->setStyleSheet("");
    ui->CarIdLabel->setStyleSheet("color: white;");
    ui->DriverIdBox->setStyleSheet("");
    ui->DriverIdLabel->setStyleSheet("color: white;");
    ui->TypeBox->setStyleSheet("");
    ui->TypeLabel->setStyleSheet("color: white;");
    ui->DescEdit->setStyleSheet("");
    ui->DescLabel->setStyleSheet("color: white;");
    ui->DolgEdit->setStyleSheet("");
    ui->DolgLabel->setStyleSheet("color: white;");
}

void addupdatewindowEvents::on_paidButton_clicked() {
    // 1. Получаем данные исходного события
    Event originalEvent = Operations::getEvent(this->id); // Получаем событие по ID окна

    float currentDolg = originalEvent.getDolg(); // Получаем значение долга из исходного события

    // 2. Обновляем исходное событие в базе данных
    originalEvent.setDolg(0);
    originalEvent.setAmount(0);
    // Описание для исходного события: "оплачено: [сегодняшняя дата]"
    originalEvent.setDescription(QString("оплачено: %1").arg(QDateTime::currentDateTime().toString("dd.MM.yyyy")));
    Operations::updateEvent(originalEvent);

    // 3. Создаем новое событие для оплаты на текущую дату
    // Описание для нового события оплаты, используем дату исходного события
    QString paymentDescForNewEvent = QString("долг оплачен: %1").arg(originalEvent.getDate().toString("dd.MM.yyyy"));

    Operations::addEvent(Event(QVariantList::fromList({
        0, // ID для нового события (0 означает новое событие)
        originalEvent.getCarId(),
        originalEvent.getDriverId(),
        originalEvent.getTypeId(),
        0, // Долг для события оплаты равен 0
        currentDolg, // Сумма для события оплаты равна исходному долгу
        paymentDescForNewEvent,
        QDateTime::currentDateTime() // Дата события оплаты - текущая дата
    })));

    // 4. Обновляем UI текущего окна (необязательно, так как окно закроется)
    ui->DolgEdit->setText("0");
    ui->AmountEdit->setText("0"); // Также очищаем сумму
    ui->paidButton->setDisabled(true); // Отключаем кнопку после действия

    // 5. Закрываем окно
    emit closed();
    this->close();
}





