#include "pages/login.h"
#include "managers/stylemanager.h"
#include "managers/encryptionmanager.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    EncryptionManager &em = EncryptionManager::getInstance();

    em.setKey("kjafibsnkfnerougbervnmfejgrubdinsm");

    styleManager s(&a);

    login w;

    w.show();
    return a.exec();
}
