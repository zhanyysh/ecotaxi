#include "eventpage.h"
#include "ui_eventpage.h"

eventPage::eventPage(nm *nav, QWidget *parent)
    : QWidget(parent), ui(new Ui::eventPage)
{
    ui->setupUi(this);
    this->nav = nav;

    this->date = QDate::currentDate();

    userSession &u = userSession::getInstance();

    if (!u.checkIsAdmin())
    {
        ui->ReportButton->setDisabled(true);
    }

    ui->DateButton->setText(this->date.toString("dd.MM.yyyy"));

    ui->DateButton->setProperty("color", "gray");

    connect(ui->tableView, &QTableView::doubleClicked, this, &eventPage::handleDoubleClick);

    connect(ui->tableView->horizontalHeader(), &QHeaderView::sectionResized, this, &eventPage::onSectionResized);

    connect(ui->tableView->horizontalHeader(), &QHeaderView::sortIndicatorChanged, this, &eventPage::onSortIndicatorChanged);
}

eventPage::~eventPage()
{
    delete ui;
}

void eventPage::setEvents(Events table, QDate date)
{
    this->selectedColumn = -1;

    this->table = table;

    if (!date.isNull())
    {
        this->date = date;

        ui->DateButton->setText(this->date.toString("dd.MM.yyyy"));
    }

    setHeader();
    setTable();
    setBottomTable();
}

void eventPage::setHeader()
{
    QString header = "bark";
    switch (this->table)
    {
    case Events::Events:
        header = "СОБЫТИЯ";
        break;

    case Events::Charges:
        header = "ЗАРЯДКИ";
        break;
    }

    ui->EcoTaxi->setText(header);
}

bool eventPage::setTable()
{
    QStandardItemModel *model = new QStandardItemModel();

    switch (this->table)
    {
    case Events::Events:
        model->setHorizontalHeaderLabels({"id", "Время", "Тип", "ID Водителя", "ID Машины", "Сумма", "Описание"});
        for (const QVariant &event : Operations::selectEventsByDate(this->date))
        {
            QList<QStandardItem *> row;

            QVariantList data = event.toList();

            row.append(new QStandardItem(data[0].toString()));

            QStandardItem *dateItem = new QStandardItem();
            dateItem->setData(data[1].toDateTime(), Qt::DisplayRole);
            row.append(dateItem);

            row.append(new QStandardItem(data[2].toString()));
            row.append(new QStandardItem(data[3].toString()));
            row.append(new QStandardItem(data[4].toString()));

            QStandardItem *amountItem = new QStandardItem();
            amountItem->setData(data[5].toInt(), Qt::DisplayRole); // Сумма
            row.append(amountItem);
            
            row.append(new QStandardItem(data[6].toString()));
            model->appendRow(row);
        }
        break;
    case Events::Charges:
        model->setHorizontalHeaderLabels({"id", "Время", "ID Машины", "ID Водителя", "Локация", "КВТ", "Время"});
        for (const QVariant &charge : Operations::selectChargesByDate(this->date))
        {
            QList<QStandardItem *> row;

            QVariantList data = charge.toList();

            row.append(new QStandardItem(data[0].toString()));

            QStandardItem *dateItem = new QStandardItem();
            dateItem->setData(data[1].toDateTime(), Qt::DisplayRole);
            row.append(dateItem);

            row.append(new QStandardItem(data[2].toString()));
            row.append(new QStandardItem(data[3].toString()));
            row.append(new QStandardItem(data[4].toString()));

            QStandardItem *kwhItem = new QStandardItem();
            kwhItem->setData(data[5].toInt(), Qt::DisplayRole);
            row.append(kwhItem);
            
            QStandardItem *timeItem = new QStandardItem();
            timeItem->setData(data[6].toInt(), Qt::DisplayRole);
            row.append(timeItem);

            model->appendRow(row);
        }
    }
    ui->tableView->setModel(model);

    if (this->table == Events::Events)
    {
        GreenBackgroundDelegate *delegate = new GreenBackgroundDelegate(ui->tableView);
        ui->tableView->setItemDelegateForColumn(5, delegate);
    }

    ui->tableView->setColumnHidden(0, true);

    ui->tableView->resizeColumnsToContents();

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);

    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    ui->tableView->horizontalHeader()->setCascadingSectionResizes(true);

    if (this->selectedColumn != -1)
        ui->tableView->sortByColumn(this->selectedColumn, this->sortOrder);

    switch (this->table)
    {
    case Events::Events:
        ui->tableView->setColumnWidth(1, 120);
        ui->tableView->setColumnWidth(2, 199);
        ui->tableView->setColumnWidth(3, 199);
        ui->tableView->setColumnWidth(4, 199);
        ui->tableView->setColumnWidth(5, 199);
        ui->tableView->setColumnWidth(6, 215);
        break;

    case Events::Charges:
        ui->tableView->setColumnWidth(1, 120);
        ui->tableView->setColumnWidth(2, 200);
        ui->tableView->setColumnWidth(3, 200);
        ui->tableView->setColumnWidth(4, 200);
        ui->tableView->setColumnWidth(5, 200);
        ui->tableView->setColumnWidth(6, 215);
        break;
    }

    return true;
}


void eventPage::setBottomTable() {

    QStandardItemModel *model = new QStandardItemModel();

    switch (this->table)
    {
    case Events::Events:
        for (const QVariant &event : Operations::getAllEventsReport(this->date)) {
            QVariantList events = event.toList();
            model->setHorizontalHeaderLabels({"Итого",
                                              events[0].toString(),
                                              events[1].toString(),
                                              events[2].toString(),
                                              });
        }
        break;

    case Events::Charges:
        for (const QVariant &charge : Operations::getAllChargesReport(this->date)) {
            QVariantList charges = charge.toList();
            model->setHorizontalHeaderLabels({"Итого",
                                              charges[0].toString()});
        }
        break;

    default:
        break;
    }

    ui->bottomTable->setModel(model);

    ui->bottomTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}

void eventPage::on_BackButton_clicked()
{
    nav->changeWindow(0);
}

void eventPage::on_DeleteButton_clicked()
{
    QItemSelectionModel *select = ui->tableView->selectionModel();
    if (select->hasSelection())
    {
        int row = select->selectedRows().at(0).row();

        int id = ui->tableView->model()->index(row, 0).data().toLongLong();

        QString question;

        switch (this->table)
        {
        case Events::Events:
            if (true)
            {
                Event event = Operations::getEvent(id);
                question = "Вы уверены что хотите удалить эвент " + QString::number(event.getId()) + ", " + event.getDate().toString("HH:mm:ss") + ", " + Operations::getType(event.getTypeId()).getName() + ", " + QString::number(event.getAmount()) + ", " + event.getDescription() + "?";
            }
            break;
        case Events::Charges:
            if (true)
            {
                Charge charge = Operations::getCharge(id);
                question = "Вы уверены что хотите удалить зарядку " + QString::number(charge.getId()) + ", " + charge.getDate().toString("HH:mm:ss") + ", " + Operations::getCar(charge.getCarId()).getSid() + ", " + Operations::getDriver(charge.getDriverId()).getName() + ", " + Operations::getLocation(charge.getLocationId()).getName() + ", " + QString::number(charge.getKwh()) + ", " + QString::number(charge.getDuration()) + "?";
            }
        default:
            break;
        }
        CustomDialog *d = new CustomDialog(question);

        connect(d, &CustomDialog::yesClicked, this, &eventPage::onYes);
        connect(d, &CustomDialog::noClicked, this, &eventPage::onNo);

        d->show();
    }
}

void eventPage::onYes()
{
    QItemSelectionModel *select = ui->tableView->selectionModel();
    if (select->hasSelection())
    {
        int row = select->selectedRows().at(0).row();

        int id = ui->tableView->model()->index(row, 0).data().toLongLong();

        switch (this->table)
        {
        case Events::Events:
            Operations::deleteEvent(id);
            break;
        case Events::Charges:
            Operations::deleteCharge(id);
            break;
        }
        setTable();
    }
}

void eventPage::onNo()
{
    // nav->changeWindow(0);
}

void eventPage::on_EditButton_clicked()
{
    QItemSelectionModel *select = ui->tableView->selectionModel();
    if (select->hasSelection())
    {
        int row = select->selectedRows().at(0).row();

        int id = ui->tableView->model()->index(row, 0).data().toLongLong();

        addupdatewindowEvents *w = new addupdatewindowEvents(this->table, id);
        w->resize(w->minimumSizeHint());
        w->show();

        QEventLoop loop;
        connect(w, SIGNAL(closed()), &loop, SLOT(quit()));
        loop.exec();

        setTable();
    }
}

void eventPage::on_DateButton_clicked()
{
    CalendarPage *c = new CalendarPage(this->date);

    connect(c, &CalendarPage::changeDate, this, &eventPage::onDateChange);

    c->show();
}

void eventPage::onDateChange(QDate date)
{
    this->date = date;
    ui->DateButton->setText(this->date.toString("dd.MM.yyyy"));
    setTable();
    setBottomTable();
}

void eventPage::onSectionResized(int logicalIndex, int oldSize, int newSize)
{
    if (logicalIndex != ui->tableView->model()->columnCount() - 1)
    {
        adjustColumnWidths();
    }
}

void eventPage::adjustColumnWidths()
{
    int totalWidth = 0;
    for (int i = 0; i < ui->tableView->model()->columnCount() - 1; ++i)
    {
        totalWidth += ui->tableView->columnWidth(i);
    }

    int lastColumnWidth = 1130 - totalWidth;
    if (lastColumnWidth > 150)
    {
        ui->tableView->setColumnWidth(ui->tableView->model()->columnCount() - 1, lastColumnWidth);
    }
    else
    {
        ui->tableView->setColumnWidth(ui->tableView->model()->columnCount() - 1, 150);
    }
}

void eventPage::on_ReportButton_clicked()
{
    switch (this->table)
    {
    case Events::Events:
        nav->openReport(1);
        break;
    case Events::Charges:
        nav->openReport(6);
        break;
    }
}

void eventPage::on_PrevButton_clicked()
{
    this->date = this->date.addDays(-1);
    ui->DateButton->setText(this->date.toString("dd.MM.yyyy"));

    setTable();
    setBottomTable();
}

void eventPage::on_NextButton_clicked()
{
    this->date = this->date.addDays(1);
    ui->DateButton->setText(this->date.toString("dd.MM.yyyy"));
    setTable();
    setBottomTable();
}

void eventPage::handleDoubleClick(const QModelIndex &index)
{
    if (index.isValid()) {
        QModelIndex firstColumnIndex = index.sibling(index.row(), 0);

        int id = ui->tableView->model()->data(firstColumnIndex).toLongLong();

        addupdatewindowEvents *w = new addupdatewindowEvents(this->table, id);
        w->resize(w->minimumSizeHint());
        w->show();

        QEventLoop loop;
        connect(w, SIGNAL(closed()), &loop, SLOT(quit()));
        loop.exec();

        setTable();
    }
}

void eventPage::onSortIndicatorChanged(int logicalIndex, Qt::SortOrder order) {
    this->selectedColumn = logicalIndex;
    this->sortOrder = order;
}
