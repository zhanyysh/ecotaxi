#ifndef GENERALREPORT_H
#define GENERALREPORT_H

#include <QWidget>
#include "QDate"
#include <QStandardItem>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <QMessageBox>

#include "../managers/reportoperations.h"
#include "../managers/operations.h"

#include "../enums/Report.h"
#include "../managers/nm.h"

#include "../managers/pdfmanager.h"

#include "calendarpage.h"

namespace Ui
{
    class GeneralReport;
}

class GeneralReport : public QWidget
{
    Q_OBJECT

public:
    explicit GeneralReport(nm *nav, QWidget *parent = nullptr);
    ~GeneralReport();

    void setReport(Report mode, QDate from = QDate(), QDate to = QDate());

private slots:
    void on_BackButton_clicked();

    void on_SettingsButton_clicked();

    void on_ReportButton_clicked();

    void on_SecondReportButton_clicked();

    void on_FilterButton_clicked();

    void on_FromDateButton_clicked();
    void on_ToDateButton_clicked();

    void on_ToPDFButton_clicked();

    void handleDoubleClick(const QModelIndex &index);

    void onSectionResized(int logicalIndex, int oldSize, int newSize);

    void onSortIndicatorChanged(int logicalIndex, Qt::SortOrder order);

private:
    Ui::GeneralReport *ui;

    nm *nav;

    void setHeader();

    void setTable();

    void setBottomTable();

    void setTableSizes();

    void setFromDate(QDate date);
    void setToDate(QDate date);

    void adjustColumnWidths();

    int selectedColumn;
    Qt::SortOrder sortOrder;

    Report mode;

    QDate fromDate;
    QDate toDate;
};

#endif // GENERALREPORT_H
