#ifndef CALENDARPAGE_H
#define CALENDARPAGE_H

#include <QWidget>
#include <QDate>

namespace Ui {
class CalendarPage;
}

class CalendarPage : public QWidget
{
    Q_OBJECT

public:
    explicit CalendarPage(QDate date, QWidget *parent = nullptr);
    ~CalendarPage();

signals:
    void changeDate(QDate date);

private slots:
    void on_confirmButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::CalendarPage *ui;
};

#endif // CALENDARPAGE_H
