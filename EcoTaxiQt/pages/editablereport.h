#ifndef EDITABLEREPORT_H
#define EDITABLEREPORT_H

#include "../enums/eSetting.h"

#include <QWidget>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QItemSelectionModel>
#include <QLineEdit>
#include <QComboBox>
#include <QStringList>
#include "../managers/operations.h"
#include "../managers/reportoperations.h"
#include "../managers/pdfmanager.h"
#include "../managers/excelmanager.h"
#include "../managers/nm.h"
#include "customdialog.h"
#include "addupdaterepair.h"

namespace Ui {
class EditableReport;
}

class EditableReport : public QWidget
{
    Q_OBJECT

public:
    explicit EditableReport(nm *nav, QWidget *parent = nullptr);
    ~EditableReport();

    void openReport(eSetting mode);

    void setTable();
    void setTableSize();

private slots:
    void on_BackButton_clicked();
    void on_AddButton_clicked();
    void on_EditButton_clicked();
    void on_DeleteButton_clicked();
    void on_ToPDFButton_clicked();
    void on_ToExcelButton_clicked();

    void on_checkBox_checkStateChanged(Qt::CheckState state);

    void onYes();
    void onNo();

private:
    Ui::EditableReport *ui;

    nm *nav;

    eSetting mode;

    void populateComboBoxes();
    void updateFilterComboBoxes();
};

#endif // EDITABLEREPORT_H
