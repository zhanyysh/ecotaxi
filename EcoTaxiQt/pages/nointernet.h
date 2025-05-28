#ifndef NOINTERNET_H
#define NOINTERNET_H

#include <QWidget>

namespace Ui {
class NoInternet;
}

class NoInternet : public QWidget
{
    Q_OBJECT

public:
    explicit NoInternet(QWidget *parent = nullptr);
    ~NoInternet();

private slots:
    void on_ReloadButton_clicked();
    void on_ExitButton_clicked();

signals:
    bool reload();

private:
    Ui::NoInternet *ui;
};

#endif // NOINTERNET_H
