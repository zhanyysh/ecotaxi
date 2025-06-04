#ifndef PAYMENTHISTORY_H
#define PAYMENTHISTORY_H

#include <QWidget>
#include <QDate>
#include <QStandardItemModel>
#include "../managers/excelmanager.h"
#include "../managers/pdfmanager.h"

namespace Ui {
class PaymentHistory;
}

class PaymentHistory : public QWidget
{
    Q_OBJECT

public:
    explicit PaymentHistory(QWidget *parent = nullptr);
    ~PaymentHistory();

signals:
    void backClicked();

private slots:
    void on_AddPaymentButton_clicked();
    void on_SettingsButton_clicked();
    void on_BackButton_clicked();

private:
    Ui::PaymentHistory *ui;
    QStandardItemModel *model;
    void setupTable();
    void loadPayments();
};

#endif // PAYMENTHISTORY_H 