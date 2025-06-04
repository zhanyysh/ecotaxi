#include "paymenthistory.h"
#include "ui_paymenthistory.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QDateEdit>
#include <QDoubleSpinBox>
#include <QPushButton>

PaymentHistory::PaymentHistory(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PaymentHistory)
{
    ui->setupUi(this);
    setupTable();
    loadPayments();
}

PaymentHistory::~PaymentHistory()
{
    delete ui;
}

void PaymentHistory::setupTable()
{
    model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels({"ID", "Инвестор", "Дата", "Сумма"});
    ui->tableView->setModel(model);
    ui->tableView->setColumnHidden(0, true);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void PaymentHistory::loadPayments()
{
    model->removeRows(0, model->rowCount());
    
    QSqlQuery query;
    query.prepare("SELECT p.id, i.name, p.date, p.amount FROM payments p "
                 "JOIN investors i ON p.investor_id = i.id "
                 "ORDER BY p.date DESC");
    
    if (query.exec()) {
        while (query.next()) {
            QList<QStandardItem*> row;
            row << new QStandardItem(query.value(0).toString());
            row << new QStandardItem(query.value(1).toString());
            row << new QStandardItem(query.value(2).toDate().toString("dd.MM.yyyy"));
            row << new QStandardItem(QString::number(query.value(3).toDouble(), 'f', 2));
            model->appendRow(row);
        }
    } else {
        qDebug() << "Error loading payments:" << query.lastError().text();
    }
}

void PaymentHistory::on_AddPaymentButton_clicked()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Добавить выплату");
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    // Создаем виджеты для формы
    QHBoxLayout *investorLayout = new QHBoxLayout();
    QLabel *investorLabel = new QLabel("Инвестор:", dialog);
    QComboBox *investorComboBox = new QComboBox(dialog);
    investorLayout->addWidget(investorLabel);
    investorLayout->addWidget(investorComboBox);
    
    QHBoxLayout *dateLayout = new QHBoxLayout();
    QLabel *dateLabel = new QLabel("Дата:", dialog);
    QDateEdit *dateEdit = new QDateEdit(dialog);
    dateEdit->setDate(QDate::currentDate());
    dateLayout->addWidget(dateLabel);
    dateLayout->addWidget(dateEdit);
    
    QHBoxLayout *amountLayout = new QHBoxLayout();
    QLabel *amountLabel = new QLabel("Сумма:", dialog);
    QDoubleSpinBox *amountSpinBox = new QDoubleSpinBox(dialog);
    amountSpinBox->setMaximum(1000000);
    amountLayout->addWidget(amountLabel);
    amountLayout->addWidget(amountSpinBox);
    
    // Загружаем список инвесторов
    QSqlQuery query;
    query.prepare("SELECT name FROM investors ORDER BY name");
    if (query.exec()) {
        while (query.next()) {
            investorComboBox->addItem(query.value(0).toString());
        }
    }
    
    // Добавляем кнопки
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *okButton = new QPushButton("OK", dialog);
    QPushButton *cancelButton = new QPushButton("Отмена", dialog);
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    
    // Добавляем все в основной layout
    layout->addLayout(investorLayout);
    layout->addLayout(dateLayout);
    layout->addLayout(amountLayout);
    layout->addLayout(buttonLayout);
    
    // Подключаем сигналы
    connect(okButton, &QPushButton::clicked, [=]() {
        QString investorName = investorComboBox->currentText();
        QDate date = dateEdit->date();
        double amount = amountSpinBox->value();
        
        if (investorName.isEmpty()) {
            QMessageBox::warning(dialog, "Ошибка", "Выберите инвестора");
            return;
        }
        
        QSqlQuery query;
        query.prepare("INSERT INTO payments (investor_id, date, amount) "
                     "SELECT id, :date, :amount FROM investors WHERE name = :name");
        query.bindValue(":date", date);
        query.bindValue(":amount", amount);
        query.bindValue(":name", investorName);
        
        if (query.exec()) {
            loadPayments();
            dialog->accept();
            QMessageBox::information(this, "Успех", "Выплата успешно добавлена");
        } else {
            QMessageBox::critical(dialog, "Ошибка", "Не удалось добавить выплату: " + query.lastError().text());
        }
    });
    
    connect(cancelButton, &QPushButton::clicked, dialog, &QDialog::reject);
    
    dialog->exec();
    dialog->deleteLater();
}

void PaymentHistory::on_SettingsButton_clicked()
{
    // TODO: Реализовать открытие настроек
}

void PaymentHistory::on_BackButton_clicked()
{
    emit backClicked();
} 