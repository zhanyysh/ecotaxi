#include "addupdatewindow.h"
#include "ui_addupdatewindow.h"

addUpdateWindow::addUpdateWindow(Setting mode, int id, QWidget *parent)
    : QWidget(parent), ui(new Ui::addUpdateWindow)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose);

    ui->frame->setProperty("color", "green");

    this->mode = mode;
    this->id = id;

    this->setWindowModality(Qt::ApplicationModal);

    ui->YearEdit->setValidator(new QIntValidator());
    ui->MilleageEdit->setValidator(new QIntValidator());
    ui->PercentEdit->setValidator(new QIntValidator());

    ui->DescLabel->setProperty("color", "white");

    if (id > 0)
    {
        ui->pushButton->setText("Сохранить");
    }
    else
    {
        ui->pushButton->setText("Добавить");
    }

    switch (this->mode)
    {
    case Setting::Drivers:
        ui->TitleFrame->hide();
        ui->IdFrame->hide();
        ui->BrandFrame->hide();
        ui->ModelFrame->hide();
        ui->LicenseFrame->hide();
        ui->YearFrame->hide();
        ui->InvestorFrame->hide();
        ui->MilleageFrame->hide();
        ui->PasswordFrame->hide();
        ui->PercentFrame->hide();
        ui->isAdminFrame->hide();
        ui->NameLabel->setProperty("color", "white");
        if (id > 0)
        {
            ui->Header->setText("Редактировать водителя");
            Driver driver = Operations::getDriver(id);
            ui->NameEdit->setText(driver.getName());
            ui->DescEdit->setText(driver.getDescription());
        }
        else
        {
            ui->Header->setText("Добавить водителя");
        }
        break;
    case Setting::Cars:
        ui->TitleFrame->hide();
        ui->NameFrame->hide();
        ui->PasswordFrame->hide();
        ui->isAdminFrame->hide();
        ui->IdLabel->setProperty("color", "white");
        ui->BrandLabel->setProperty("color", "white");
        ui->ModelLabel->setProperty("color", "white");
        ui->LicenseLabel->setProperty("color", "white");
        ui->YearLabel->setProperty("color", "white");
        ui->InvestorLabel->setProperty("color", "white");
        ui->MilleageLabel->setProperty("color", "white");

        if (true){
            userSession &us = userSession::getInstance();
            if (us.checkIsAdmin())
                ui->PercentLabel->setProperty("color", "white");
            else
                ui->PercentFrame->hide();
        }

        investors_list = Operations::selectAllInvestors();

        foreach (Investor investor, investors_list)
        {
            QString show = investor.getName();
            investors.insert(show, investor.getId());
            ui->investorBox->addItem(show);
        }

        if (id > 0)
        {
            ui->Header->setText("Редактировать машину");
            Car car = Operations::getCar(id);
            ui->IdEdit->setText(car.getSid());
            ui->BrandEdit->setText(car.getBrand());
            ui->ModelEdit->setText(car.getModel());
            ui->LicenseEdit->setText(car.getLicensePlate());
            ui->investorBox->setCurrentText(Operations::getInvestor(car.getInvestorId()).getName());
            ui->MilleageEdit->setText(QString::number(car.getMilleage()));
            ui->YearEdit->setText(QString::number(car.getYear()));
            ui->PercentEdit->setText(QString::number(car.getPercentage()));
            ui->DescEdit->setText(car.getDescription());
        }
        else
        {
            ui->Header->setText("Добавить машину");
        }
        break;
    case Setting::Investors:
        ui->TitleFrame->hide();
        ui->BrandFrame->hide();
        ui->IdFrame->hide();
        ui->ModelFrame->hide();
        ui->LicenseFrame->hide();
        ui->YearFrame->hide();
        ui->InvestorFrame->hide();
        ui->MilleageFrame->hide();
        ui->PercentFrame->hide();
        ui->isAdminFrame->hide();
        ui->NameLabel->setProperty("color", "white");
        ui->PasswordLabel->setProperty("color", "white");
        if (id > 0)
        {
            ui->Header->setText("Редактировать инвестора");
            Investor investor = Operations::getInvestor(id);
            ui->NameEdit->setText(investor.getName());
            ui->DescEdit->setText(investor.getDescription());
        }
        else
        {
            ui->Header->setText("Добавить инвестора");
        }
        break;
    case Setting::Types:
        ui->NameFrame->hide();
        ui->IdFrame->hide();
        ui->BrandFrame->hide();
        ui->ModelFrame->hide();
        ui->LicenseFrame->hide();
        ui->YearFrame->hide();
        ui->InvestorFrame->hide();
        ui->MilleageFrame->hide();
        ui->PasswordFrame->hide();
        ui->PercentFrame->hide();
        ui->TitleLabel->setProperty("color", "white");
        ui->isAdminCheckBox->setProperty("color", "white");
        ui->isAdminCheckBox->setText("Для админов");
        if (id > 0)
        {
            ui->Header->setText("Редактировать тип");
            Type type = Operations::getType(id);
            ui->TitleEdit->setText(type.getName());
            ui->DescEdit->setText(type.getDescription());
            ui->isAdminCheckBox->setChecked(type.getForAdmin());
        }
        else
        {
            ui->Header->setText("Добавить тип");
        }
        break;
    case Setting::Locations:
        ui->NameFrame->hide();
        ui->IdFrame->hide();
        ui->BrandFrame->hide();
        ui->ModelFrame->hide();
        ui->LicenseFrame->hide();
        ui->YearFrame->hide();
        ui->InvestorFrame->hide();
        ui->MilleageFrame->hide();
        ui->PasswordFrame->hide();
        ui->PercentFrame->hide();
        ui->isAdminFrame->hide();
        ui->TitleLabel->setProperty("color", "white");
        if (id > 0)
        {
            ui->Header->setText("Редактировать локацию");
            Location location = Operations::getLocation(id);
            ui->TitleEdit->setText(location.getName());
            ui->DescEdit->setText(location.getDescription());
        }
        else
        {
            ui->Header->setText("Добавить локацию");
        }
        break;
    case Setting::Users:
        ui->IdFrame->hide();
        ui->BrandFrame->hide();
        ui->ModelFrame->hide();
        ui->LicenseFrame->hide();
        ui->YearFrame->hide();
        ui->InvestorFrame->hide();
        ui->MilleageFrame->hide();
        ui->TitleFrame->hide();
        ui->PercentFrame->hide();
        ui->NameLabel->setProperty("color", "white");
        ui->PasswordLabel->setProperty("color", "white");
        ui->isAdminCheckBox->setProperty("color", "white");
        if (id > 0)
        {
            ui->Header->setText("Редактировать пользователя");
            User user = Operations::getUser(id);
            ui->NameEdit->setText(user.getName());
            userSession &u = userSession::getInstance();
            ui->DescEdit->setText(user.getDescription());
            ui->isAdminCheckBox->setChecked(user.getIsAdmin());
        }
        else
        {
            ui->Header->setText("Добавить пользователя");
        }
        break;
    }
}

addUpdateWindow::~addUpdateWindow()
{
    delete ui;
}

void addUpdateWindow::on_pushButton_clicked()
{
    addRecord();
}

void addUpdateWindow::addRecord()
{
    if (checkFill())
    {
        if (id > 0)
        {
            switch (this->mode)
            {
            case Setting::Drivers:
                Operations::updateDriver(Driver(QVariantList::fromList({id, ui->NameEdit->text(), ui->DescEdit->toPlainText()})));
                break;
            case Setting::Cars:
                Operations::updateCar(id, Car(QVariantList::fromList({id, ui->IdEdit->text(), ui->BrandEdit->text(), ui->ModelEdit->text(), ui->LicenseEdit->text(), ui->YearEdit->text().toLongLong(), investors.value(ui->investorBox->currentText()), ui->MilleageEdit->text().toLongLong(), ui->DescEdit->toPlainText(), ui->PercentEdit->text().toLongLong()})));
                break;
            case Setting::Investors:
                Operations::updateInvestor(Investor(QVariantList::fromList({id, ui->NameEdit->text(), ui->DescEdit->toPlainText(), HASH::generateHMAC(ui->PasswordEdit->text())})));
                break;
            case Setting::Types:
                Operations::updateType(Type(QVariantList::fromList({id, ui->TitleEdit->text(), ui->DescEdit->toPlainText(), ui->isAdminCheckBox->isChecked()})));
                break;
            case Setting::Locations:
                Operations::updateLocation(Location(QVariantList::fromList({id, ui->TitleEdit->text(), ui->DescEdit->toPlainText()})));
                break;
            case Setting::Users:
                Operations::updateUser(User(QVariantList::fromList({id, ui->NameEdit->text(), HASH::generateHMAC(ui->PasswordEdit->text()), ui->DescEdit->toPlainText(), ui->isAdminCheckBox->isChecked()})));
                break;
            }
        }
        else
        {
            switch (this->mode)
            {
            case Setting::Drivers:
                Operations::addDriver(Driver(QVariantList::fromList({id, ui->NameEdit->text(), ui->DescEdit->toPlainText()})));
                break;
            case Setting::Cars:
                Operations::addCar(Car(QVariantList::fromList({0, ui->IdEdit->text(), ui->BrandEdit->text(), ui->ModelEdit->text(), ui->LicenseEdit->text(), ui->YearEdit->text(), investors.value(ui->investorBox->currentText()), ui->MilleageEdit->text(), ui->DescEdit->toPlainText(), ui->PercentEdit->text().toLongLong()})));
                break;
            case Setting::Investors:
                Operations::addInvestor(Investor(QVariantList::fromList({id, ui->NameEdit->text(), ui->DescEdit->toPlainText(), HASH::generateHMAC(ui->PasswordEdit->text())})));
                break;
            case Setting::Types:
                Operations::addType(Type(QVariantList::fromList({id, ui->TitleEdit->text(), ui->DescEdit->toPlainText(), ui->isAdminCheckBox->isChecked()})));
                break;
            case Setting::Locations:
                Operations::addLocation(Location(QVariantList::fromList({id, ui->TitleEdit->text(), ui->DescEdit->toPlainText()})));
                break;
            case Setting::Users:
                Operations::addUser(User(QVariantList::fromList({id, ui->NameEdit->text(), HASH::generateHMAC(ui->PasswordEdit->text()), ui->DescEdit->toPlainText(), ui->isAdminCheckBox->isChecked()})));
                break;
            }
        }
        emit close();
        this->closed();
    }
    else
    {
        QTimer::singleShot(200, this, &addUpdateWindow::resetInputColor);
    }
}

bool addUpdateWindow::checkFill()
{
    bool result = true;
    switch (this->mode)
    {
    case Setting::Drivers:
        if (ui->NameEdit->text().isEmpty())
        {
            ui->NameEdit->setStyleSheet("background-color: red;");
            ui->NameLabel->setStyleSheet("color: red;");
            result = false;
        }
        break;

    case Setting::Cars:
        if (ui->investorBox->currentIndex() == -1)
        {
            ui->investorBox->setStyleSheet("background-color: red;");
            ui->InvestorLabel->setStyleSheet("color: red;");
            result = false;
        }
        if (ui->IdEdit->text().isEmpty())
        {
            ui->IdEdit->setStyleSheet("background-color: red;");
            ui->IdLabel->setStyleSheet("color: red;");
            result = false;
        }
        if (ui->BrandEdit->text().isEmpty())
        {
            ui->BrandEdit->setStyleSheet("background-color: red;");
            ui->BrandLabel->setStyleSheet("color: red;");
            result = false;
        }
        if (ui->ModelEdit->text().isEmpty())
        {
            ui->ModelEdit->setStyleSheet("background-color: red;");
            ui->ModelLabel->setStyleSheet("color: red;");
            result = false;
        }
        if (ui->LicenseEdit->text().isEmpty())
        {
            ui->LicenseEdit->setStyleSheet("background-color: red;");
            ui->LicenseLabel->setStyleSheet("color: red;");
            result = false;
        }
        if (ui->YearEdit->text().isEmpty())
        {
            ui->YearEdit->setStyleSheet("background-color: red;");
            ui->YearLabel->setStyleSheet("color: red;");
            result = false;
        }
        if (ui->MilleageEdit->text().isEmpty())
        {
            ui->MilleageEdit->setStyleSheet("background-color: red;");
            ui->MilleageLabel->setStyleSheet("color: red;");
            result = false;
        }
        try
        {
            ui->MilleageEdit->text().toLongLong();
        }
        catch (const std::exception &e)
        {
            ui->MilleageEdit->setStyleSheet("background-color: red;");
            ui->MilleageLabel->setStyleSheet("color: red;");
            result = false;
        }
        try
        {
            ui->YearEdit->text().toLongLong();
        }
        catch (const std::exception &e)
        {
            ui->YearEdit->setStyleSheet("background-color: red;");
            ui->YearLabel->setStyleSheet("color: red;");
            result = false;
        }
        try
        {
            ui->IdEdit->text().toLongLong();
        }
        catch (const std::exception &e)
        {
            ui->IdEdit->setStyleSheet("background-color: red;");
            ui->IdLabel->setStyleSheet("color: red;");
            result = false;
        }
        break;

    case Setting::Investors:
        if (ui->NameEdit->text().isEmpty())
        {
            ui->NameEdit->setStyleSheet("background-color: red;");
            ui->NameLabel->setStyleSheet("color: red;");
            result = false;
        }
        break;

    case Setting::Types:
        if (ui->TitleEdit->text().isEmpty())
        {
            ui->TitleEdit->setStyleSheet("background-color: red;");
            ui->TitleLabel->setStyleSheet("color: red;");
            result = false;
        }
        break;

    case Setting::Locations:
        if (ui->TitleEdit->text().isEmpty())
        {
            ui->TitleEdit->setStyleSheet("background-color: red;");
            ui->TitleLabel->setStyleSheet("color: red;");
            result = false;
        }
        break;

    case Setting::Users:
        if (ui->NameEdit->text().isEmpty())
        {
            ui->NameEdit->setStyleSheet("background-color: red;");
            ui->NameLabel->setStyleSheet("color: red;");
            result = false;
        }
    }

    return result;
}

void addUpdateWindow::resetInputColor()
{
    ui->NameEdit->setStyleSheet("");
    ui->DescEdit->setStyleSheet("");
    ui->IdEdit->setStyleSheet("");
    ui->BrandEdit->setStyleSheet("");
    ui->ModelEdit->setStyleSheet("");
    ui->LicenseEdit->setStyleSheet("");
    ui->YearEdit->setStyleSheet("");
    ui->MilleageEdit->setStyleSheet("");
    ui->TitleEdit->setStyleSheet("");
    ui->PasswordEdit->setStyleSheet("");
    ui->investorBox->setStyleSheet("");

    ui->NameLabel->setStyleSheet("color: white;");
    ui->DescLabel->setStyleSheet("color: white;");
    ui->IdLabel->setStyleSheet("color: white;");
    ui->BrandLabel->setStyleSheet("color: white;");
    ui->ModelLabel->setStyleSheet("color: white;");
    ui->LicenseLabel->setStyleSheet("color: white;");
    ui->YearLabel->setStyleSheet("color: white;");
    ui->MilleageLabel->setStyleSheet("color: white;");
    ui->TitleLabel->setStyleSheet("color: white;");
    ui->PasswordLabel->setStyleSheet("color: white;");
    ui->InvestorLabel->setStyleSheet("color: white;");
}
