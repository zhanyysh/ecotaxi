#ifndef EVENTPAGE_H
#define EVENTPAGE_H

#include <QWidget>
#include <QStandardItemModel>

#include "../managers/nm.h"
#include "../managers/operations.h"
#include "../managers/usersession.h"

#include "../classes/event.h"
#include "../classes/charge.h"

#include "../pages/calendarpage.h"
#include "../pages/addupdatewindowevents.h"
#include "../pages/customdialog.h"

#include "../scripts/greenbackgrounddelegate.h"

#include "../enums/Events.h"

namespace Ui
{
    class eventPage;
}

class eventPage : public QWidget
{
    Q_OBJECT

public:
    explicit eventPage(nm *nav, QWidget *parent = nullptr);
    ~eventPage();
    void setEvents(Events events, QDate date = QDate());

private slots:
    void on_BackButton_clicked();
    void on_EditButton_clicked();
    void on_DeleteButton_clicked();

    void on_DateButton_clicked();

    void on_ReportButton_clicked();

    void onDateChange(QDate date);

    void on_PrevButton_clicked();
    void on_NextButton_clicked();

    void onSectionResized(int logicalIndex, int oldSize, int newSize);

    void handleDoubleClick(const QModelIndex &index);

    void onSortIndicatorChanged(int logicalIndex, Qt::SortOrder order);

    void onYes();
    void onNo();

private:
    Ui::eventPage *ui;
    nm *nav;
    Events table;

    void setHeader();
    bool setTable();
    void setBottomTable();

    void adjustColumnWidths();

    const int tableWidth = 1130;
    const int minColumnWidth = 100;

    int selectedColumn;
    Qt::SortOrder sortOrder;

    QDate date;
};

#endif // EVENTPAGE_H
