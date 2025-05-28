#ifndef SETTINGSPAGE_H
#define SETTINGSPAGE_H

#include <QWidget>
#include "QString"
#include "QStandardItemModel"

#include "../managers/nm.h"
#include "../managers/operations.h"

#include "../managers/usersession.h"

#include "../enums/setting.h"

#include "addupdatewindow.h"
#include "customdialog.h"

namespace Ui {
class SettingsPage;
}

class SettingsPage : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsPage(nm *nav, QWidget *parent = nullptr);
    ~SettingsPage();

    void setSettings(Setting table);

private slots:
    void on_BackButton_clicked();
    void on_AddButton_clicked();
    void on_EditButton_clicked();
    void on_DeleteButton_clicked();

    void onYes();
    void onNo();

    void on_ReportButton_clicked();

    void handleDoubleClick(const QModelIndex &index);

    void onSectionResized(int logicalIndex, int oldSize, int newSize);

private:
    Ui::SettingsPage *ui;

    Setting table;

    void setHeader();

    bool setTable();

    nm *nav;

    void adjustColumnWidths();
};

#endif // SETTINGSPAGE_H
