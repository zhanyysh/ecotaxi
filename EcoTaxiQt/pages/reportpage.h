#ifndef REPORTPAGE_H
#define REPORTPAGE_H

#include <QWidget>
#include <QDate>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QItemSelection>

#include "../managers/nm.h"
#include "../managers/reportoperations.h"
#include "../managers/operations.h"
#include "../managers/pdfmanager.h"

#include "../classes/car.h"
#include "../classes/driver.h"
#include "../classes/type.h"
#include "../classes/investor.h"

#include "../enums/Report.h"

#include "calendarpage.h"

namespace Ui
{
    class ReportPage;
}

class ReportPage : public QWidget
{
    Q_OBJECT

public:
    explicit ReportPage(nm *nav, QWidget *parent = nullptr);
    ~ReportPage();

    void setReport(Report mode, int id, QDate from = QDate(), QDate to = QDate());

private slots:
    void on_BackButton_clicked();

    void on_SettingsButton_clicked();

    void on_ReportButton_clicked();

    void on_ItemButton_clicked();

    void on_FilterButton_clicked();

    void on_FromDateButton_clicked();
    void on_ToDateButton_clicked();

    void on_ToPDFButton_clicked();

    void onSectionResized(int logicalIndex, int oldSize, int newSize);

    void onSortIndicatorChanged(int logicalIndex, Qt::SortOrder order);

private:
    Ui::ReportPage *ui;

    void setHeader();

    void setTable();

    void setBottomTable();

    void setSideTable();

    void setTableSizes();

    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

    Report mode;

    void setFromDate(QDate date);
    void setToDate(QDate date);

    void adjustColumnWidths();

    int selectedColumn;
    Qt::SortOrder sortOrder;

    QDate fromDate;
    QDate toDate;

    int id;

    nm *nav;
};

#endif // REPORTPAGE_H
