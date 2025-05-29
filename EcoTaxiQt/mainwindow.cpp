#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(nm *nav, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    userSession &u = userSession::getInstance();

    this->nav = nav;
    ui->setupUi(this);
    this->setWindowTitle("Главное окно | ECO TAXI");

    ui->SettingsButton->setCurrentIndex(-1);
    ui->ReportsButton->setCurrentIndex(-1);
    ui->FinesButton->setCurrentIndex(-1);
    ui->timeEdit->setDisabled(true);

    // hide settings and reports button if user is not admin
    // if (!u.checkIsAdmin())
    // {
    //     // ui->SettingsButton->setDisabled(true);
    //     ui->SettingsButton->removeItem(6);
    //     ui->SettingsButton->removeItem(5);
    //     ui->SettingsButton->removeItem(2);
    //     ui->ReportsButton->removeItem(7);
    //     ui->ReportsButton->removeItem(5);
    //     ui->ReportsButton->removeItem(4);
    //     ui->ReportsButton->removeItem(3);
    //     ui->ReportsButton->removeItem(1);
    //     ui->ReportsButton->removeItem(0);
    // }
    // else if (u.getId() != -1)
    // {
    //     ui->SettingsButton->removeItem(6);
    //     ui->SettingsButton->removeItem(5);
    // }

    // date & time
    date = QDate::currentDate();
    time = QTime::currentTime();

    ui->KWTEdit->setValidator(new QIntValidator());
    ui->SumEdit->setValidator(new QDoubleValidator());
    ui->DolgEdit->setValidator(new QDoubleValidator());
    ui->TimeEdit->setValidator(new QIntValidator());

    ui->dateButton->setText(date.toString("dd.MM.yyyy"));
    ui->timeEdit->setTime(time);

    //
    ui->dateButton->setProperty("color", "gray");

    ui->eventFrame->setProperty("color", "green");
    ui->chargeFrame->setProperty("color", "green");
    ui->SettingsButton->setProperty("type", "nav");
    ui->ReportsButton->setProperty("type", "nav");
    ui->FinesButton->setProperty("type", "nav");

    // event components
    ui->typeLabel->setProperty("color", "white");
    ui->carsLabel->setProperty("color", "white");
    ui->driversLabel->setProperty("color", "white");
    ui->sumLabel->setProperty("color", "white");
    ui->descLabel->setProperty("color", "white");
    ui->dolgLabel->setProperty("color", "white");

    // charge components
    ui->carsLabel_2->setProperty("color", "white");
    ui->driversLabel_2->setProperty("color", "white");
    ui->locsLabel->setProperty("color", "white");
    ui->kwtLabel->setProperty("color", "white");
    ui->timeLabel->setProperty("color", "white");

    db = &dbManager::getInstance();

    openWidnow();
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::checkEventFill()
{
    bool result = true;

    if (ui->SumEdit->text().isEmpty())
    {
        ui->SumEdit->setStyleSheet("background-color: red;");
        ui->sumLabel->setStyleSheet("color: red;");
        result = false;
    }

    if (ui->DolgEdit->text().isEmpty()) {
        ui->DolgEdit->setStyleSheet("background-color: red;");
        ui->dolgLabel->setStyleSheet("color: red;");
        result = false;
    }

    return result;
}

bool MainWindow::checkChargeFill()
{
    bool result = true;

    if (ui->KWTEdit->text().isEmpty())
    {
        ui->KWTEdit->setStyleSheet("background-color: red;");
        ui->kwtLabel->setStyleSheet("color: red;");
        result = false;
    }

    if (ui->CarsBox_2->currentIndex() == 0)
    {
        ui->CarsBox_2->setStyleSheet("background-color: red;");
        ui->carsLabel_2->setStyleSheet("color: red;");
        result = false;
    }

    if (ui->DriversBox_2->currentIndex() == 0)
    {
        ui->DriversBox_2->setStyleSheet("background-color: red;");
        ui->driversLabel_2->setStyleSheet("color: red;");
        result = false;
    }

    if (ui->LocsBox->currentIndex() == 0)
    {
        ui->LocsBox->setStyleSheet("background-color: red;");
        ui->locsLabel->setStyleSheet("color: red;");
        result = false;
    }

    return result;
}

void MainWindow::setSettingIndex()
{
    if (ui->SettingsButton->currentIndex() == -1)
    {
        return;
    }
    ui->SettingsButton->setCurrentIndex(-1);
}

void MainWindow::setReportIndex()
{
    if (ui->ReportsButton->currentIndex() == -1)
    {
        return;
    }
    ui->ReportsButton->setCurrentIndex(-1);
}

void MainWindow::setFinesIndex()
{
    if (ui->FinesButton->currentIndex() == -1)
    {
        return;
    }
    ui->FinesButton->setCurrentIndex(-1);
}

void MainWindow::on_SettingsButton_currentIndexChanged(int index)
{
    if (userSession::getInstance().checkIsAdmin()) {
        if (index == 6)
        {
            RemoveDB *rdb = new RemoveDB();
            connect(rdb, &RemoveDB::yesClicked, this, &MainWindow::setComboBoxesData);
            rdb->show();
        }
        else
        {
            nav->openSettings(index);
        }
    }
    else {
        if (index > 1) {
            index += 1;
        }
        nav->openSettings(index);
    }
    setSettingIndex();
}
void MainWindow::on_ReportsButton_currentIndexChanged(int index)
{
    setReportIndex();
    if (index > 7)
        index += 7;
    nav->openReport(index);
}

void MainWindow::on_FinesButton_currentIndexChanged(int index)
{
    nav->openFines(index);
    setFinesIndex();
}

void MainWindow::openWidnow()
{
    setComboBoxesData();
}

void MainWindow::setComboBoxesData()
{
    // clear
    this->cars.clear();
    this->drivers.clear();
    this->types.clear();
    this->locations.clear();

    ui->CarsBox->clear();
    ui->CarsBox_2->clear();
    ui->CarsBox->clear();
    ui->DriversBox->clear();
    ui->DriversBox_2->clear();
    ui->TypesBox->clear();
    ui->LocsBox->clear();

    // set
    ui->CarsBox->addItem("-");
    ui->CarsBox_2->addItem("-");
    QList<Car> cars_list = Operations::selectAllCars();
    foreach (Car car, cars_list)
    {
        QString show = car.getSid();
        this->cars.insert(show, car.getId());
        ui->CarsBox->addItem(show);
        ui->CarsBox_2->addItem(show);
    }

    ui->DriversBox->addItem("-");
    ui->DriversBox_2->addItem("-");
    QList<Driver> driver_list = Operations::selectAllDrivers();
    foreach (Driver driver, driver_list)
    {
        QString show = driver.getName();
        this->drivers.insert(show, driver.getId());
        ui->DriversBox->addItem(show);
        ui->DriversBox_2->addItem(show);
    }

    QList<Type> types_list = Operations::selectAllTypes();
    foreach (Type type, types_list)
    {
        QString show = type.getName();
        this->types.insert(show, type.getId());
        ui->TypesBox->addItem(show);
    }

    QList<Location> locations_list = Operations::selectAllLocations();
    ui->LocsBox->addItem("-");
    foreach (Location location, locations_list)
    {
        QString show = location.getName();
        this->locations.insert(show, location.getId());
        ui->LocsBox->addItem(show);
    }
}

void MainWindow::on_addEventButton_clicked()
{
    if (checkEventFill())
    {
        // summa == int
        if (true)
        {
            QString datetime;
            if (useSystemTime)
                datetime = this->date.toString("yyyy-MM-dd") + " " + QTime::currentTime().toString("hh:mm:ss");
            else
                datetime = this->date.toString("yyyy-MM-dd") + " " + ui->timeEdit->time().toString("hh:mm:ss");
            Operations::addEvent(Event(QVariantList::fromList({
                0,
                this->cars.value(ui->CarsBox->currentText()),
                this->drivers.value(ui->DriversBox->currentText()),
                this->types.value(ui->TypesBox->currentText()),
                ui->DolgEdit->text(),
                ui->SumEdit->text(),
                ui->descEdit->toPlainText(),
                datetime
            })));
            clearAllInputsEvent();
            ui->TypesBox->setStyleSheet("background-color: #32CD32;");
            ui->typeLabel->setStyleSheet("color: #32CD32;");
            ui->DriversBox->setStyleSheet("background-color: #32CD32;");
            ui->driversLabel->setStyleSheet("color: #32CD32;");
            ui->CarsBox->setStyleSheet("background-color: #32CD32;");
            ui->carsLabel->setStyleSheet("color: #32CD32;");
            ui->SumEdit->setStyleSheet("background-color: #32CD32;");
            ui->sumLabel->setStyleSheet("color: #32CD32;");
            ui->DolgEdit->setStyleSheet("background-color: #32CD32;");
            ui->dolgLabel->setStyleSheet("color: #32CD32;");
            ui->descEdit->setStyleSheet("background-color: #32CD32;");
            ui->descLabel->setStyleSheet("color: #32CD32;");
        }
    }
    QTimer::singleShot(200, this, &MainWindow::resetInputColor);
}

void MainWindow::on_addChargeButton_clicked()
{
    if (checkChargeFill())
    {
        QString datetime;
        if (useSystemTime)
            datetime = this->date.toString("yyyy-MM-dd") + " " + QTime::currentTime().toString("hh:mm:ss");
        else
            datetime = this->date.toString("yyyy-MM-dd") + " " + ui->timeEdit->time().toString("hh:mm:ss");
        Operations::addCharge(Charge(QVariantList::fromList({0, this->cars.value(ui->CarsBox_2->currentText()), this->drivers.value(ui->DriversBox_2->currentText()), this->locations.value(ui->LocsBox->currentText()), ui->KWTEdit->text(), ui->TimeEdit->text(), datetime})));
        clearAllInputsCharge();
        ui->LocsBox->setStyleSheet("background-color: #32CD32;");
        ui->locsLabel->setStyleSheet("color: #32CD32;");
        ui->CarsBox_2->setStyleSheet("background-color: #32CD32;");
        ui->carsLabel_2->setStyleSheet("color: #32CD32;");
        ui->DriversBox_2->setStyleSheet("background-color: #32CD32;");
        ui->driversLabel_2->setStyleSheet("color: #32CD32;");
        ui->KWTEdit->setStyleSheet("background-color: #32CD32;");
        ui->kwtLabel->setStyleSheet("color: #32CD32;");
        ui->TimeEdit->setStyleSheet("background-color: #32CD32;");
        ui->timeLabel->setStyleSheet("color: #32CD32;");
    }
    QTimer::singleShot(200, this, &MainWindow::resetInputColor);
}

void MainWindow::on_eventsButton_clicked()
{
    nav->openEvents(0, date);
}

void MainWindow::on_chargesButton_clicked()
{
    nav->openEvents(1, date);
}

void MainWindow::resetInputColor()
{
    ui->CarsBox->setStyleSheet("");
    ui->carsLabel->setStyleSheet("color: white;");
    ui->CarsBox_2->setStyleSheet("");
    ui->carsLabel_2->setStyleSheet("color: white;");
    ui->DriversBox->setStyleSheet("");
    ui->driversLabel->setStyleSheet("color: white;");
    ui->DriversBox_2->setStyleSheet("");
    ui->driversLabel_2->setStyleSheet("color: white;");
    ui->TypesBox->setStyleSheet("");
    ui->typeLabel->setStyleSheet("color: white;");
    ui->LocsBox->setStyleSheet("");
    ui->locsLabel->setStyleSheet("color: white;");
    ui->SumEdit->setStyleSheet("");
    ui->sumLabel->setStyleSheet("color: white;");
    ui->KWTEdit->setStyleSheet("");
    ui->kwtLabel->setStyleSheet("color: white;");
    ui->TimeEdit->setStyleSheet("");
    ui->timeLabel->setStyleSheet("color: white;");
    ui->descEdit->setStyleSheet("");
    ui->descLabel->setStyleSheet("color: white;");
    ui->DolgEdit->setStyleSheet("");
    ui->dolgLabel->setStyleSheet("color: white;");
}

void MainWindow::on_dateButton_clicked()
{
    CalendarPage *calendar = new CalendarPage(date);

    connect(calendar, &CalendarPage::changeDate, this, &MainWindow::onChangeDate);

    calendar->show();
}

void MainWindow::onChangeDate(QDate date)
{
    this->date = date;
    ui->dateButton->setText(this->date.toString("dd.MM.yyyy"));
}

void MainWindow::clearAllInputsEvent()
{
    ui->SumEdit->clear();
    ui->descEdit->clear();
    ui->DolgEdit->clear();
}

void MainWindow::clearAllInputsCharge()
{
    ui->CarsBox_2->setCurrentIndex(0);
    ui->DriversBox_2->setCurrentIndex(0);
    ui->LocsBox->setCurrentIndex(0);
    ui->KWTEdit->clear();
    ui->TimeEdit->clear();
}

void MainWindow::on_systemTimeCheck_checkStateChanged(Qt::CheckState state)
{
    switch (state)
    {
    case Qt::CheckState::Checked:
        time = QTime::currentTime();
        ui->timeEdit->setTime(time);

        useSystemTime = true;
        ui->timeEdit->setDisabled(true);
        break;
    case Qt::CheckState::Unchecked:
        time = QTime::currentTime();
        ui->timeEdit->setTime(time);

        useSystemTime = false;
        ui->timeEdit->setDisabled(false);
        break;
    default:
        break;
    }
}

void MainWindow::on_repairsButton_clicked()
{
    nav->changeWindow(5);
}

// void MainWindow::on_addImagesButton_clicked()
// {
//     // AwsS3Uploader uploader;
//     QStringList filesToUpload = QFileDialog::getOpenFileNames(this, tr("Выберите изображения"), "", tr("Изображения (*.png *.jpg *.jpeg *.ico)"));
//     // std::vector<QString> uploadedUrls = uploader.uploadFiles("ecotaxi-bucket", filesToUpload);
// }
