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
#include <QDoubleValidator>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QSortFilterProxyModel>
#include "../pages/calendarpage.h"

class PaymentFilterProxyModel : public QSortFilterProxyModel {
public:
    QString investorFilter;
    QDate dateFrom, dateTo;
    QString descFilter;

    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override {
        QAbstractItemModel *model = sourceModel();
        // Инвестор
        if (!investorFilter.isEmpty()) {
            QString investor = model->index(sourceRow, 1, sourceParent).data().toString();
            if (investor != investorFilter) return false;
        }
        // Дата
        if (dateFrom.isValid() || dateTo.isValid()) {
            QDate date = QDate::fromString(model->index(sourceRow, 2, sourceParent).data().toString(), "dd.MM.yyyy");
            if (dateFrom.isValid() && date < dateFrom) return false;
            if (dateTo.isValid() && date > dateTo) return false;
        }
        // Описание
        if (!descFilter.isEmpty()) {
            QString desc = model->index(sourceRow, 4, sourceParent).data().toString();
            if (!desc.contains(descFilter, Qt::CaseInsensitive)) return false;
        }
        return true;
    }
};

PaymentFilterProxyModel *proxyModel = nullptr;

PaymentHistory::PaymentHistory(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PaymentHistory)
{
    ui->setupUi(this);
    setupTable();
    loadPayments();

    // Настройка фильтров
    proxyModel = new PaymentFilterProxyModel();
    proxyModel->setSourceModel(model);
    ui->tableView->setModel(proxyModel);

    // Даты по умолчанию
    QDate fromDate = QDate::currentDate().addMonths(-1);
    QDate toDate = QDate::currentDate();
    ui->dateFromButton->setText(fromDate.toString("dd.MM.yyyy"));
    ui->dateToButton->setText(toDate.toString("dd.MM.yyyy"));
    proxyModel->dateFrom = fromDate;
    proxyModel->dateTo = toDate;

    // Сигналы фильтров
    connect(ui->investorFilterComboBox, &QComboBox::currentTextChanged, this, [=](const QString &text){
        proxyModel->investorFilter = text;
        proxyModel->invalidate();
    });
    connect(ui->descFilterLineEdit, &QLineEdit::textChanged, this, [=](const QString &text){
        proxyModel->descFilter = text;
        proxyModel->invalidate();
    });
    connect(ui->dateFromButton, &QPushButton::clicked, this, [=](){
        QDate current = QDate::fromString(ui->dateFromButton->text(), "dd.MM.yyyy");
        CalendarPage *calendar = new CalendarPage(current);
        connect(calendar, &CalendarPage::changeDate, this, [=](QDate date){
            ui->dateFromButton->setText(date.toString("dd.MM.yyyy"));
            proxyModel->dateFrom = date;
            proxyModel->invalidate();
        });
        calendar->show();
    });
    connect(ui->dateToButton, &QPushButton::clicked, this, [=](){
        QDate current = QDate::fromString(ui->dateToButton->text(), "dd.MM.yyyy");
        CalendarPage *calendar = new CalendarPage(current);
        connect(calendar, &CalendarPage::changeDate, this, [=](QDate date){
            ui->dateToButton->setText(date.toString("dd.MM.yyyy"));
            proxyModel->dateTo = date;
            proxyModel->invalidate();
        });
        calendar->show();
    });
}

PaymentHistory::~PaymentHistory()
{
    delete ui;
}

void PaymentHistory::setupTable()
{
    model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels({"ID", "Инвестор", "Дата", "Сумма", "Описание"});
    ui->tableView->setModel(model);
    ui->tableView->setColumnHidden(0, true);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void PaymentHistory::loadPayments()
{
    model->removeRows(0, model->rowCount());
    QSqlQuery query;
    query.prepare("SELECT p.id, i.name, p.date, p.amount, p.description FROM payments p "
                 "JOIN investors i ON p.investor_id = i.id "
                 "ORDER BY p.date DESC");
    if (query.exec()) {
        while (query.next()) {
            QList<QStandardItem*> row;
            row << new QStandardItem(query.value(0).toString());
            row << new QStandardItem(query.value(1).toString());
            row << new QStandardItem(query.value(2).toDate().toString("dd.MM.yyyy"));
            row << new QStandardItem(QString::number(query.value(3).toDouble(), 'f', 2));
            row << new QStandardItem(query.value(4).toString());
            model->appendRow(row);
        }
    } else {
        qDebug() << "Error loading payments:" << query.lastError().text();
    }
    if (proxyModel) proxyModel->invalidate();

    // Обновляем фильтр инвесторов
    QString current = ui->investorFilterComboBox->currentText();
    ui->investorFilterComboBox->blockSignals(true);
    ui->investorFilterComboBox->clear();
    ui->investorFilterComboBox->addItem("");
    QSqlQuery q("SELECT DISTINCT i.name FROM investors i JOIN payments p ON p.investor_id = i.id ORDER BY i.name");
    while (q.next()) {
        ui->investorFilterComboBox->addItem(q.value(0).toString());
    }
    int idx = ui->investorFilterComboBox->findText(current);
    if (idx >= 0) ui->investorFilterComboBox->setCurrentIndex(idx);
    ui->investorFilterComboBox->blockSignals(false);
}

void PaymentHistory::on_AddPaymentButton_clicked()
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Добавить выплату");
    dialog->setFixedSize(400, 400);
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    // Инвестор
    QHBoxLayout *investorLayout = new QHBoxLayout();
    QLabel *investorLabel = new QLabel("<b><span style='color:#007700; font-size:22px;'>Инвестор:</span></b>", dialog);
    QComboBox *investorComboBox = new QComboBox(dialog);
    investorComboBox->setMinimumWidth(120);
    investorComboBox->setStyleSheet("font-size: 22px; font-weight: bold; background: white; border: 2px solid #007700; border-radius: 8px; padding: 6px 10px;");
    investorLayout->addWidget(investorLabel);
    investorLayout->addWidget(investorComboBox);
    
    // Дата
    QHBoxLayout *dateLayout = new QHBoxLayout();
    QLabel *dateLabel = new QLabel("<b><span style='color:#007700; font-size:22px;'>Дата:</span></b>", dialog);
    QDateEdit *dateEdit = new QDateEdit(dialog);
    dateEdit->setDate(QDate::currentDate());
    dateEdit->setStyleSheet("font-size: 22px; font-weight: bold; background: white; border: 2px solid #007700; border-radius: 8px; padding: 6px 10px;");
    dateLayout->addWidget(dateLabel);
    dateLayout->addWidget(dateEdit);
    
    // Сумма
    QHBoxLayout *amountLayout = new QHBoxLayout();
    QLabel *amountLabel = new QLabel("<b><span style='color:#007700; font-size:22px;'>Сумма:</span></b>", dialog);
    QLineEdit *amountEdit = new QLineEdit(dialog);
    amountEdit->setPlaceholderText("Введите сумму");
    amountEdit->setValidator(new QDoubleValidator(0, 1000000, 2, amountEdit));
    amountEdit->setMinimumWidth(120);
    amountEdit->setStyleSheet("font-size: 22px; font-weight: bold; background: white; border: 2px solid #007700; border-radius: 8px; padding: 6px 10px;");
    amountLayout->addWidget(amountLabel);
    amountLayout->addWidget(amountEdit);

    // Описание
    QHBoxLayout *descLayout = new QHBoxLayout();
    QLabel *descLabel = new QLabel("<b><span style='color:#007700; font-size:22px;'>Описание:</span></b>", dialog);
    QTextEdit *descEdit = new QTextEdit(dialog);
    descEdit->setPlaceholderText("Введите описание (необязательно)");
    descEdit->setMinimumHeight(100);
    descEdit->setStyleSheet("font-size: 22px; font-weight: bold; background: white; border: 2px solid #007700; border-radius: 8px; padding: 8px 10px;");
    descLayout->addWidget(descLabel);
    descLayout->addWidget(descEdit);
    
    // Загружаем список инвесторов
    QSqlQuery query;
    query.prepare("SELECT name FROM investors ORDER BY name");
    if (query.exec()) {
        while (query.next()) {
            investorComboBox->addItem(query.value(0).toString());
        }
    }
    
    // Кнопки
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *okButton = new QPushButton("OK", dialog);
    QPushButton *cancelButton = new QPushButton("Отмена", dialog);
    okButton->setStyleSheet("QPushButton { background-color: #007700; color: white; font-size: 28px; font-weight: bold; border-radius: 10px; padding: 10px 30px; } QPushButton:hover { background-color: #005500; }");
    cancelButton->setStyleSheet("QPushButton { background-color: #007700; color: white; font-size: 28px; font-weight: bold; border-radius: 10px; padding: 10px 30px; } QPushButton:hover { background-color: #005500; }");
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    
    // Добавляем всё в layout
    layout->addLayout(investorLayout);
    layout->addLayout(dateLayout);
    layout->addLayout(amountLayout);
    layout->addLayout(descLayout);
    layout->addLayout(buttonLayout);
    
    // Сигналы
    connect(okButton, &QPushButton::clicked, [=]() {
        QString investorName = investorComboBox->currentText();
        QDate date = dateEdit->date();
        double amount = amountEdit->text().replace(",", ".").toDouble();
        QString description = descEdit->toPlainText();
        
        if (investorName.isEmpty()) {
            QMessageBox::warning(dialog, "Ошибка", "Выберите инвестора");
            return;
        }
        if (amount <= 0) {
            QMessageBox::warning(dialog, "Ошибка", "Введите корректную сумму");
            return;
        }
        
        QSqlQuery query;
        query.prepare("INSERT INTO payments (investor_id, date, amount, description) "
                     "SELECT id, :date, :amount, :description FROM investors WHERE name = :name");
        query.bindValue(":date", date);
        query.bindValue(":amount", amount);
        query.bindValue(":description", description);
        query.bindValue(":name", investorName);
        
        if (query.exec()) {
            loadPayments();
            dialog->accept();
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