#ifndef ADDUPDATEWINDOW_H
#define ADDUPDATEWINDOW_H

#include <QWidget>
#include "QVariantList"
#include "QTimer"

#include "../managers/operations.h"
#include "../managers/usersession.h"

#include "../enums/setting.h"

#include "../classes/car.h"
#include "../classes/charge.h"
#include "../classes/driver.h"
#include "../classes/event.h"
#include "../classes/investor.h"
#include "../classes/location.h"
#include "../classes/type.h"
#include "../classes/user.h"

#include "../scripts/hash.h"

namespace Ui
{
    class addUpdateWindow;
}

class addUpdateWindow : public QWidget
{
    Q_OBJECT

public:
    explicit addUpdateWindow(Setting mode, int id = -1, QWidget *parent = nullptr);
    ~addUpdateWindow();

signals:
    void closed();

private slots:
    void on_pushButton_clicked();

private:
    Ui::addUpdateWindow *ui;

    void addRecord();

    bool checkFill();

    void resetInputColor();

    QList<Investor> investors_list;

    QMap<QString, int> investors;

    Setting mode;
    int id;
};

#endif // ADDUPDATEWINDOW_H
