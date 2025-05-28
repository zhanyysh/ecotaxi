#include "type.h"
#include "iostream"
using namespace std;

Type::Type() {}

Type::Type(QVariantList type)
{
    if (type.isEmpty())
    {
        this->id = -1;
        this->name = "удален";
        this->description = "";
        this->forAdmin = false;
    }
    else
    {
        this->id = type[0].toInt();
        this->name = type[1].toString();
        this->description = type[2].toString();
        this->forAdmin = type[3].toBool();
    }
}

int Type::getId()
{
    return id;
}

QString Type::getName()
{
    return name;
}

QString Type::getDescription()
{
    return description;
}

bool Type::getForAdmin()
{
    return forAdmin;
}

void Type::setName(QString newName)
{
    name = newName;
}

void Type::setDescription(QString newDescription)
{
    description = newDescription;
}

void Type::setForAdmin(bool newForAdmin)
{
    forAdmin = newForAdmin;
}
