#include "kwhperiod.h"
#include <QString>
#include <QDate>
#include <QVariantList>

KwhPeriod::KwhPeriod() {}

KwhPeriod::KwhPeriod(QVariantList period)
{
    this->id = period[0].toInt();
    this->fromDate = period[1].toDate();
    this->toDate = period[2].toDate();
    this->multiplier = period[3].toFloat();
    this->description = period.size() > 4 ? period[4].toString() : "";
}

int KwhPeriod::getId()
{
    return id;
}

QDate KwhPeriod::getFromDate()
{
    return fromDate;
}

QDate KwhPeriod::getToDate()
{
    return toDate;
}

float KwhPeriod::getMultiplier()
{
    return multiplier;
}

QString KwhPeriod::getDescription()
{
    return description;
}

void KwhPeriod::setId(int newId)
{
    id = newId;
}

void KwhPeriod::setFromDate(QDate newFromDate)
{
    fromDate = newFromDate;
}

void KwhPeriod::setToDate(QDate newToDate)
{
    toDate = newToDate;
}

void KwhPeriod::setMultiplier(float newMultiplier)
{
    multiplier = newMultiplier;
}

void KwhPeriod::setDescription(QString newDescription)
{
    description = newDescription;
} 