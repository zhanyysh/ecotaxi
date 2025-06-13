#ifndef KWHPERIOD_H
#define KWHPERIOD_H

#include <QDate>
#include <QDateTime>
#include <QString>
#include <QVariantList>

class KwhPeriod
{
private:
    int id;
    QDate fromDate;
    QDate toDate;
    float multiplier;
    QString description;

public:
    KwhPeriod();
    KwhPeriod(QVariantList period);
    
    int getId();
    QDate getFromDate();
    QDate getToDate();
    float getMultiplier();
    QString getDescription();

    void setId(int newId);
    void setFromDate(QDate newFromDate);
    void setToDate(QDate newToDate);
    void setMultiplier(float newMultiplier);
    void setDescription(QString newDescription);
};

#endif // KWHPERIOD_H 