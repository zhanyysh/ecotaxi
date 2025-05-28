#include "calendarpage.h"
#include "ui_calendarpage.h"

CalendarPage::CalendarPage(QDate date, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CalendarPage)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose);

    ui->calendarWidget->setSelectedDate(date);

    this->setWindowModality(Qt::ApplicationModal);
}

CalendarPage::~CalendarPage()
{
    delete ui;
}

void CalendarPage::on_confirmButton_clicked()
{
    emit changeDate(ui->calendarWidget->selectedDate());
    this->close();
}

void CalendarPage::on_cancelButton_clicked()
{
    this->close();
}
