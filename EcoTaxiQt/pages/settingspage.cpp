#include "settingspage.h"
#include "ui_settingspage.h"
#include <QInputDialog>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDateEdit>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QTextEdit>
#include "calendarpage.h"

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

    case Setting::Parameters:
        header = "ПАРАМЕТРЫ";
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
        for (Driver driver : Operations::selectAllDrivers()) {
            QList<QStandardItem*> row;
            row << new QStandardItem(QString::number(driver.getId()))
                << new QStandardItem(driver.getName())
                << new QStandardItem(driver.getDescription());
            model->appendRow(row);
        }
        ui->tableView->setModel(model);
        ui->tableView->setColumnHidden(0, true);
        ui->tableView->resizeColumnsToContents();
        break;
    case Setting::Cars:
        model->setHorizontalHeaderLabels({
            "ID", "SID", "Марка", "Модель", "Госномер", "Год", "Инвестор", "Пробег", "% инвестора", "Описание"
        });
        for (Car car : Operations::selectAllCars()) {
            QList<QStandardItem*> row;
            row << new QStandardItem(QString::number(car.getId()))
                << new QStandardItem(car.getSid())
                << new QStandardItem(car.getBrand())
                << new QStandardItem(car.getModel())
                << new QStandardItem(car.getLicensePlate())
                << new QStandardItem(QString::number(car.getYear()))
                << new QStandardItem(Operations::getInvestor(car.getInvestorId()).getName())
                << new QStandardItem(QString::number(car.getMilleage()))
                << new QStandardItem(QString::number(car.getPercentage()))
                << new QStandardItem(car.getDescription());
            model->appendRow(row);
        }
        ui->tableView->setModel(model);
        ui->tableView->setColumnHidden(0, true);
        ui->tableView->resizeColumnsToContents();
        break;
    case Setting::Investors:
        model->setHorizontalHeaderLabels({"ID", "Имя", "Описание"});
        for (Investor investor : Operations::selectAllInvestors()) {
            QList<QStandardItem*> row;
            row << new QStandardItem(QString::number(investor.getId()))
                << new QStandardItem(investor.getName())
                << new QStandardItem(investor.getDescription());
            model->appendRow(row);
        }
        ui->tableView->setModel(model);
        ui->tableView->setColumnHidden(0, true);
        ui->tableView->resizeColumnsToContents();
        break;
    case Setting::Types:
        model->setHorizontalHeaderLabels({"ID", "Название", "Описание"});
        for (Type type : Operations::selectAllTypes()) {
            QList<QStandardItem*> row;
            row << new QStandardItem(QString::number(type.getId()))
                << new QStandardItem(type.getName())
                << new QStandardItem(type.getDescription());
            model->appendRow(row);
        }
        ui->tableView->setModel(model);
        ui->tableView->setColumnHidden(0, true);
        ui->tableView->resizeColumnsToContents();
        break;
    case Setting::Locations:
        model->setHorizontalHeaderLabels({"ID", "Название", "Описание"});
        for (Location location : Operations::selectAllLocations()) {
            QList<QStandardItem*> row;
            row << new QStandardItem(QString::number(location.getId()))
                << new QStandardItem(location.getName())
                << new QStandardItem(location.getDescription());
            model->appendRow(row);
        }
        ui->tableView->setModel(model);
        ui->tableView->setColumnHidden(0, true);
        ui->tableView->resizeColumnsToContents();
        break;
    case Setting::Users:
        model->setHorizontalHeaderLabels({"ID", "Логин", "Описание"});
        for (User user : Operations::selectAllUsers()) {
            QList<QStandardItem*> row;
            row << new QStandardItem(QString::number(user.getId()))
                << new QStandardItem(user.getName())
                << new QStandardItem(user.getDescription());
            model->appendRow(row);
        }
        ui->tableView->setModel(model);
        ui->tableView->setColumnHidden(0, true);
        ui->tableView->resizeColumnsToContents();
        break;
    case Setting::Parameters:
        model->setHorizontalHeaderLabels({"ID", "С", "По", "Множитель", "Описание"});
        for (KwhPeriod period : Operations::selectAllKwhPeriods()) {
            QList<QStandardItem*> row;
            row << new QStandardItem(QString::number(period.getId()))
                << new QStandardItem(period.getFromDate().toString("dd.MM.yyyy"))
                << new QStandardItem(period.getToDate().toString("dd.MM.yyyy"))
                << new QStandardItem(QString::number(period.getMultiplier()))
                << new QStandardItem(period.getDescription());
            model->appendRow(row);
        }
        ui->tableView->setModel(model);
        ui->tableView->setColumnHidden(0, true);
        ui->tableView->resizeColumnsToContents();
        break;
    }

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
    case Setting::Parameters:
        ui->tableView->setColumnWidth(0, 80);   // ID
        ui->tableView->setColumnWidth(1, 120);  // С
        ui->tableView->setColumnWidth(2, 120);  // По
        ui->tableView->setColumnWidth(3, 140);  // Множитель
        ui->tableView->setColumnWidth(4, 150);  // Описание
        break;
    }

    return true;
}

void SettingsPage::on_BackButton_clicked()
{
    nav->goBack();
}

void SettingsPage::on_AddButton_clicked()
{
    if (this->table == Setting::Parameters) {
        // Создаем диалог для добавления периода kwh
        QDialog dialog(this);
        dialog.setWindowTitle("Добавить период kwh");
        dialog.setModal(true);
        
        QVBoxLayout *layout = new QVBoxLayout(&dialog);
        
        // Переменные для хранения дат
        QDate fromDate = QDate::currentDate();
        QDate toDate = QDate::currentDate().addYears(1);
        // Поле "С"
        QHBoxLayout *fromLayout = new QHBoxLayout();
        QLabel *fromLabel = new QLabel("С:", &dialog);
        QPushButton *fromDateButton = new QPushButton(fromDate.toString("dd.MM.yyyy"), &dialog);
        fromDateButton->setStyleSheet("font-size: 22px; font-weight: bold; background: white; color: #008000; border: 2px solid #008000; border-radius: 8px; padding: 6px 10px;");
        fromLayout->addWidget(fromLabel);
        fromLayout->addWidget(fromDateButton);
        layout->addLayout(fromLayout);
        // Поле "По"
        QHBoxLayout *toLayout = new QHBoxLayout();
        QLabel *toLabel = new QLabel("По:", &dialog);
        QPushButton *toDateButton = new QPushButton(toDate.toString("dd.MM.yyyy"), &dialog);
        toDateButton->setStyleSheet("font-size: 22px; font-weight: bold; background: white; color: #008000; border: 2px solid #008000; border-radius: 8px; padding: 6px 10px;");
        toLayout->addWidget(toLabel);
        toLayout->addWidget(toDateButton);
        layout->addLayout(toLayout);
        // Слоты для открытия календаря
        QObject::connect(fromDateButton, &QPushButton::clicked, [&]() {
            CalendarPage *c = new CalendarPage(fromDate);
            QObject::connect(c, &CalendarPage::changeDate, [&](QDate date){
                fromDate = date;
                fromDateButton->setText(date.toString("dd.MM.yyyy"));
            });
            c->show();
        });
        QObject::connect(toDateButton, &QPushButton::clicked, [&]() {
            CalendarPage *c = new CalendarPage(toDate);
            QObject::connect(c, &CalendarPage::changeDate, [&](QDate date){
                toDate = date;
                toDateButton->setText(date.toString("dd.MM.yyyy"));
            });
            c->show();
        });
        
        // Поле "Множитель"
        QHBoxLayout *multiplierLayout = new QHBoxLayout();
        QLabel *multiplierLabel = new QLabel("Множитель:", &dialog);
        QDoubleSpinBox *multiplierSpinBox = new QDoubleSpinBox(&dialog);
        multiplierSpinBox->setRange(0.1, 1000.0);
        multiplierSpinBox->setValue(10.0);
        multiplierSpinBox->setDecimals(2);
        multiplierLayout->addWidget(multiplierLabel);
        multiplierLayout->addWidget(multiplierSpinBox);
        layout->addLayout(multiplierLayout);
        
        // Поле "Описание"
        QHBoxLayout *descLayout = new QHBoxLayout();
        QLabel *descLabel = new QLabel("Описание:", &dialog);
        QTextEdit *descEdit = new QTextEdit(&dialog);
        descEdit->setMinimumHeight(40);
        descLayout->addWidget(descLabel);
        descLayout->addWidget(descEdit);
        layout->addLayout(descLayout);
        
        // Кнопки
        QHBoxLayout *buttonLayout = new QHBoxLayout();
        QPushButton *okButton = new QPushButton("OK", &dialog);
        QPushButton *cancelButton = new QPushButton("Отмена", &dialog);
        buttonLayout->addWidget(okButton);
        buttonLayout->addWidget(cancelButton);
        layout->addLayout(buttonLayout);
        
        connect(okButton, &QPushButton::clicked, &dialog, &QDialog::accept);
        connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);
        
        dialog.setStyleSheet("QDialog { background: #008000; border-radius: 16px; } QLabel { color: white; font-size: 22px; font-weight: bold; } QDateEdit, QDoubleSpinBox, QTextEdit { background: white; color: #008000; font-size: 22px; font-weight: bold; border: 2px solid #008000; border-radius: 8px; padding: 4px 8px; } QPushButton { background-color: #008000; color: white; font-size: 28px; font-weight: bold; border-radius: 10px; padding: 10px 30px; } QPushButton:hover { background-color: #005500; }");
        fromLabel->setStyleSheet("color: white; font-size: 22px; font-weight: bold;");
        toLabel->setStyleSheet("color: white; font-size: 22px; font-weight: bold;");
        multiplierLabel->setStyleSheet("color: white; font-size: 22px; font-weight: bold;");
        descLabel->setStyleSheet("color: white; font-size: 22px; font-weight: bold;");
        okButton->setMinimumHeight(52);
        cancelButton->setMinimumHeight(52);
        
        if (dialog.exec() == QDialog::Accepted) {
            KwhPeriod period;
            period.setFromDate(fromDate);
            period.setToDate(toDate);
            period.setMultiplier(multiplierSpinBox->value());
            period.setDescription(descEdit->toPlainText());
            
            if (Operations::addKwhPeriod(period)) {
                setTable();
            }
        }
        return;
    }
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
    if (this->table == Setting::Parameters) {
        QItemSelectionModel *select = ui->tableView->selectionModel();
        if (!select->hasSelection()) {
            return;
        }
        
        int row = select->selectedRows().at(0).row();
        int id = ui->tableView->model()->index(row, 0).data().toInt();
        
        KwhPeriod period = Operations::getKwhPeriod(id);
        if (period.getId() == 0) {
            return;
        }
        
        // Создаем диалог для редактирования периода kwh
        QDialog dialog(this);
        dialog.setWindowTitle("Редактировать период kwh");
        dialog.setModal(true);
        
        QVBoxLayout *layout = new QVBoxLayout(&dialog);
        
        // Переменные для хранения дат
        QDate fromDate = period.getFromDate();
        QDate toDate = period.getToDate();
        // Поле "С"
        QHBoxLayout *fromLayout = new QHBoxLayout();
        QLabel *fromLabel = new QLabel("С:", &dialog);
        QPushButton *fromDateButton = new QPushButton(fromDate.toString("dd.MM.yyyy"), &dialog);
        fromDateButton->setStyleSheet("font-size: 22px; font-weight: bold; background: white; color: #008000; border: 2px solid #008000; border-radius: 8px; padding: 6px 10px;");
        fromLayout->addWidget(fromLabel);
        fromLayout->addWidget(fromDateButton);
        layout->addLayout(fromLayout);
        // Поле "По"
        QHBoxLayout *toLayout = new QHBoxLayout();
        QLabel *toLabel = new QLabel("По:", &dialog);
        QPushButton *toDateButton = new QPushButton(toDate.toString("dd.MM.yyyy"), &dialog);
        toDateButton->setStyleSheet("font-size: 22px; font-weight: bold; background: white; color: #008000; border: 2px solid #008000; border-radius: 8px; padding: 6px 10px;");
        toLayout->addWidget(toLabel);
        toLayout->addWidget(toDateButton);
        layout->addLayout(toLayout);
        // Слоты для открытия календаря
        QObject::connect(fromDateButton, &QPushButton::clicked, [&]() {
            CalendarPage *c = new CalendarPage(fromDate);
            QObject::connect(c, &CalendarPage::changeDate, [&](QDate date){
                fromDate = date;
                fromDateButton->setText(date.toString("dd.MM.yyyy"));
            });
            c->show();
        });
        QObject::connect(toDateButton, &QPushButton::clicked, [&]() {
            CalendarPage *c = new CalendarPage(toDate);
            QObject::connect(c, &CalendarPage::changeDate, [&](QDate date){
                toDate = date;
                toDateButton->setText(date.toString("dd.MM.yyyy"));
            });
            c->show();
        });
        
        // Поле "Множитель"
        QHBoxLayout *multiplierLayout = new QHBoxLayout();
        QLabel *multiplierLabel = new QLabel("Множитель:", &dialog);
        QDoubleSpinBox *multiplierSpinBox = new QDoubleSpinBox(&dialog);
        multiplierSpinBox->setRange(0.1, 1000.0);
        multiplierSpinBox->setValue(period.getMultiplier());
        multiplierSpinBox->setDecimals(2);
        multiplierLayout->addWidget(multiplierLabel);
        multiplierLayout->addWidget(multiplierSpinBox);
        layout->addLayout(multiplierLayout);
        
        // Поле "Описание"
        QHBoxLayout *descLayout = new QHBoxLayout();
        QLabel *descLabel = new QLabel("Описание:", &dialog);
        QTextEdit *descEdit = new QTextEdit(&dialog);
        descEdit->setMinimumHeight(40);
        descEdit->setText(period.getDescription());
        descLayout->addWidget(descLabel);
        descLayout->addWidget(descEdit);
        layout->addLayout(descLayout);
        
        // Кнопки
        QHBoxLayout *buttonLayout = new QHBoxLayout();
        QPushButton *okButton = new QPushButton("OK", &dialog);
        QPushButton *cancelButton = new QPushButton("Отмена", &dialog);
        buttonLayout->addWidget(okButton);
        buttonLayout->addWidget(cancelButton);
        layout->addLayout(buttonLayout);
        
        connect(okButton, &QPushButton::clicked, &dialog, &QDialog::accept);
        connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);
        
        dialog.setStyleSheet("QDialog { background: #008000; border-radius: 16px; } QLabel { color: white; font-size: 22px; font-weight: bold; } QDateEdit, QDoubleSpinBox, QTextEdit { background: white; color: #008000; font-size: 22px; font-weight: bold; border: 2px solid #008000; border-radius: 8px; padding: 4px 8px; } QPushButton { background-color: #008000; color: white; font-size: 28px; font-weight: bold; border-radius: 10px; padding: 10px 30px; } QPushButton:hover { background-color: #005500; }");
        fromLabel->setStyleSheet("color: white; font-size: 22px; font-weight: bold;");
        toLabel->setStyleSheet("color: white; font-size: 22px; font-weight: bold;");
        multiplierLabel->setStyleSheet("color: white; font-size: 22px; font-weight: bold;");
        descLabel->setStyleSheet("color: white; font-size: 22px; font-weight: bold;");
        okButton->setMinimumHeight(52);
        cancelButton->setMinimumHeight(52);
        
        if (dialog.exec() == QDialog::Accepted) {
            period.setFromDate(fromDate);
            period.setToDate(toDate);
            period.setMultiplier(multiplierSpinBox->value());
            period.setDescription(descEdit->toPlainText());
            
            if (Operations::updateKwhPeriod(period)) {
                setTable();
            }
        }
        return;
    }
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
        case Setting::Parameters:
        {
            KwhPeriod period = Operations::getKwhPeriod(id);
            d = new CustomDialog("Вы уверены что хотите удалить период kwh с " + 
                               period.getFromDate().toString("dd.MM.yyyy") + " по " + 
                               period.getToDate().toString("dd.MM.yyyy") + " (множитель: " + 
                               QString::number(period.getMultiplier()) + ")?");
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
        case Setting::Parameters:
            Operations::deleteKwhPeriod(id);
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
    if (!index.isValid()) return;
    QModelIndex firstColumnIndex = index.sibling(index.row(), 0);
    int id = ui->tableView->model()->data(firstColumnIndex).toInt();

    if (this->table == Setting::Parameters) {
        KwhPeriod period = Operations::getKwhPeriod(id);
        if (period.getId() == 0) return;
        // --- диалог редактирования параметра (копия из on_EditButton_clicked) ---
        QDialog dialog(this);
        dialog.setWindowTitle("Редактировать период kwh");
        dialog.setModal(true);
        QVBoxLayout *layout = new QVBoxLayout(&dialog);
        QDate fromDate = period.getFromDate();
        QDate toDate = period.getToDate();
        QHBoxLayout *fromLayout = new QHBoxLayout();
        QLabel *fromLabel = new QLabel("С:", &dialog);
        QPushButton *fromDateButton = new QPushButton(fromDate.toString("dd.MM.yyyy"), &dialog);
        fromDateButton->setStyleSheet("font-size: 22px; font-weight: bold; background: white; color: #008000; border: 2px solid #008000; border-radius: 8px; padding: 6px 10px;");
        fromLayout->addWidget(fromLabel);
        fromLayout->addWidget(fromDateButton);
        layout->addLayout(fromLayout);
        QHBoxLayout *toLayout = new QHBoxLayout();
        QLabel *toLabel = new QLabel("По:", &dialog);
        QPushButton *toDateButton = new QPushButton(toDate.toString("dd.MM.yyyy"), &dialog);
        toDateButton->setStyleSheet("font-size: 22px; font-weight: bold; background: white; color: #008000; border: 2px solid #008000; border-radius: 8px; padding: 6px 10px;");
        toLayout->addWidget(toLabel);
        toLayout->addWidget(toDateButton);
        layout->addLayout(toLayout);
        QObject::connect(fromDateButton, &QPushButton::clicked, [&]() {
            CalendarPage *c = new CalendarPage(fromDate);
            QObject::connect(c, &CalendarPage::changeDate, [&](QDate date){
                fromDate = date;
                fromDateButton->setText(date.toString("dd.MM.yyyy"));
            });
            c->show();
        });
        QObject::connect(toDateButton, &QPushButton::clicked, [&]() {
            CalendarPage *c = new CalendarPage(toDate);
            QObject::connect(c, &CalendarPage::changeDate, [&](QDate date){
                toDate = date;
                toDateButton->setText(date.toString("dd.MM.yyyy"));
            });
            c->show();
        });
        QHBoxLayout *multiplierLayout = new QHBoxLayout();
        QLabel *multiplierLabel = new QLabel("Множитель:", &dialog);
        QDoubleSpinBox *multiplierSpinBox = new QDoubleSpinBox(&dialog);
        multiplierSpinBox->setRange(0.1, 1000.0);
        multiplierSpinBox->setValue(period.getMultiplier());
        multiplierSpinBox->setDecimals(2);
        multiplierLayout->addWidget(multiplierLabel);
        multiplierLayout->addWidget(multiplierSpinBox);
        layout->addLayout(multiplierLayout);
        QHBoxLayout *descLayout = new QHBoxLayout();
        QLabel *descLabel = new QLabel("Описание:", &dialog);
        QTextEdit *descEdit = new QTextEdit(&dialog);
        descEdit->setMinimumHeight(40);
        descEdit->setText(period.getDescription());
        descLayout->addWidget(descLabel);
        descLayout->addWidget(descEdit);
        layout->addLayout(descLayout);
        QHBoxLayout *buttonLayout = new QHBoxLayout();
        QPushButton *okButton = new QPushButton("OK", &dialog);
        QPushButton *cancelButton = new QPushButton("Отмена", &dialog);
        buttonLayout->addWidget(okButton);
        buttonLayout->addWidget(cancelButton);
        layout->addLayout(buttonLayout);
        connect(okButton, &QPushButton::clicked, &dialog, &QDialog::accept);
        connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);
        dialog.setStyleSheet("QDialog { background: #008000; border-radius: 16px; } QLabel { color: white; font-size: 22px; font-weight: bold; } QDateEdit, QDoubleSpinBox, QTextEdit { background: white; color: #008000; font-size: 22px; font-weight: bold; border: 2px solid #008000; border-radius: 8px; padding: 4px 8px; } QPushButton { background-color: #008000; color: white; font-size: 28px; font-weight: bold; border-radius: 10px; padding: 10px 30px; } QPushButton:hover { background-color: #005500; }");
        fromLabel->setStyleSheet("color: white; font-size: 22px; font-weight: bold;");
        toLabel->setStyleSheet("color: white; font-size: 22px; font-weight: bold;");
        multiplierLabel->setStyleSheet("color: white; font-size: 22px; font-weight: bold;");
        descLabel->setStyleSheet("color: white; font-size: 22px; font-weight: bold;");
        okButton->setMinimumHeight(52);
        cancelButton->setMinimumHeight(52);
        if (dialog.exec() == QDialog::Accepted) {
            period.setFromDate(fromDate);
            period.setToDate(toDate);
            period.setMultiplier(multiplierSpinBox->value());
            period.setDescription(descEdit->toPlainText());
            Operations::updateKwhPeriod(period);
            setTable();
        }
        return;
    }
    // Остальные режимы — стандартное поведение (например, addUpdateWindow)
    addUpdateWindow *w = new addUpdateWindow(this->table, id);
    w->resize(w->minimumSizeHint());
    w->show();
    QEventLoop loop;
    connect(w, SIGNAL(closed()), &loop, SLOT(quit()));
    loop.exec();
    setTable();
}
