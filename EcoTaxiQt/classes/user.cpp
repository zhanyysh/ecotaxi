#include "user.h"
#include "iostream"
using namespace std;

User::User() {}


User::User(QVariantList user) {
    if (user.isEmpty()) {
        this->id = -1;
        this->name = "удален";
    }
    else {
        this->id = user[0].toInt();
        this->name = user[1].toString();
        this->password = user[2].toString();
        this->description = user[3].toString();
        this->isAdmin = user[4].toBool();
    }
}


int User::getId() {
    return id;
}

QString User::getName() {
    return name;
}

QString User::getPassword() {
    return password;
}

QString User::getDescription() {
    return description;
}

bool User::getIsAdmin() {
    return isAdmin;
}

void User::setName(QString newName) {
    name = newName;
}

void User::setPassword(QString newPassword) {
    password = newPassword;
}

void User::setDescription(QString newDescription) {
    description = newDescription;
}

void User::setIsAdmin(bool newIsAdmin) {
    isAdmin = newIsAdmin;
}