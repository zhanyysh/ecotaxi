#ifndef REMOVEDB_H
#define REMOVEDB_H

#include <QWidget>

#include "../managers/dbmanager.h"

namespace Ui {
class RemoveDB;
}

class RemoveDB : public QWidget
{
    Q_OBJECT

public:
    explicit RemoveDB(QWidget *parent = nullptr);
    ~RemoveDB();

private slots:
    void on_pushButton_clicked();

private:
    Ui::RemoveDB *ui;

signals:
    void yesClicked();
};

#endif // REMOVEDB_H
