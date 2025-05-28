#include "investor.h"
#include "iostream"
using namespace std;

Investor::Investor() {}

Investor::Investor(QVariantList investor)
{
    if (investor.isEmpty())
    {
        this->id = -1;
        this->name = "удален";
        this->description = "";
        this->password = "";
    }
    else
    {
        this->id = investor[0].toInt();
        this->name = investor[1].toString();
        this->description = investor[2].toString();
        this->password = investor[3].toString();
    }
}

int Investor::getId()
{
    return id;
}

QString Investor::getName()
{
    return name;
}

QString Investor::getDescription()
{
    return description;
}

QString Investor::getPassword()
{
    return password;
}

void Investor::setName(QString newName)
{
    name = newName;
}

void Investor::setDescription(QString newDescription)
{
    description = newDescription;
}

void Investor::setPassword(QString newPassword)
{
    password = newPassword;
}
