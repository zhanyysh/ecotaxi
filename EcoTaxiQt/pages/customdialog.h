#ifndef CUSTOMDIALOG_H
#define CUSTOMDIALOG_H

#include <QWidget>

namespace Ui {
class CustomDialog;
}

class CustomDialog : public QWidget
{
    Q_OBJECT

public:
    explicit CustomDialog(QString text = "Вы уверены?", QWidget *parent = nullptr);
    ~CustomDialog();

private:
    Ui::CustomDialog *ui;

private slots:
    void on_YesButton_clicked();
    void on_NoButton_clicked();

signals:
    void yesClicked();
    void noClicked();
};

#endif // CUSTOMDIALOG_H
