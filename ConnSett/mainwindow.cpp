#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "managers/encryptionmanager.h"
#include "QMessageBox"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loadData();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::saveData()
{
    QMessageBox popup;
    QFile file("conn.data");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file for writing.";
        popup.setTextFormat(Qt::MarkdownText);
        popup.setText("Ошибка");

        popup.exec();
        return;
    }

    QTextStream out(&file);
    EncryptionManager& manager = EncryptionManager::getInstance();

    // Get data from QLineEdit widgets, encrypt it and save it
    QString hostData = manager.encrypt(ui->HostEdit->text());
    QString portData = manager.encrypt(ui->PortEdit->text());
    QString dbName = manager.encrypt(ui->DBEdit->text());
    QString username = manager.encrypt(ui->UserEdit->text());
    QString password = manager.encrypt(ui->PasswordEdit->text());

    // Write encrypted data to file, each piece of data on a new line
    out << hostData << "\n";
    out << portData << "\n";
    out << dbName << "\n";
    out << username << "\n";
    out << password << "\n";

    file.close();

    popup.setTextFormat(Qt::MarkdownText);
    popup.setText("Данные сохранены");

    popup.exec();
}

void MainWindow::loadData()
{
    QFile file("conn.data");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file for reading.";
        return;
    }

    QTextStream in(&file);
    EncryptionManager& manager = EncryptionManager::getInstance();

    // Read encrypted data from file and decrypt it
    QString hostData = manager.decrypt(in.readLine());
    QString portData = manager.decrypt(in.readLine());
    QString dbName = manager.decrypt(in.readLine());
    QString username = manager.decrypt(in.readLine());
    QString password = manager.decrypt(in.readLine());

    // Set the decrypted data in QLineEdit widgets
    ui->HostEdit->setText(hostData);
    ui->PortEdit->setText(portData);
    ui->DBEdit->setText(dbName);
    ui->UserEdit->setText(username);
    ui->PasswordEdit->setText(password);

    file.close();
}

void MainWindow::on_SaveButton_clicked()
{
    saveData();
}

