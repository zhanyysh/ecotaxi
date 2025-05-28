#ifndef ENCRYPTIONMANAGER_H
#define ENCRYPTIONMANAGER_H

#include <QString>

class EncryptionManager {
public:
    // Get the single instance of EncryptionManager
    static EncryptionManager& getInstance();

    // Set the encryption/decryption key
    void setKey(const QString& key);

    // Encrypt the input text
    QString encrypt(const QString& value);

    // Decrypt the input text
    QString decrypt(const QString& value);

private:
    // Private constructor to enforce singleton pattern
    EncryptionManager() = default;

    // Private copy constructor and assignment operator to prevent copying
    EncryptionManager(const EncryptionManager&) = delete;
    EncryptionManager& operator=(const EncryptionManager&) = delete;

    // The encryption/decryption key
    QString key;
};

#endif // ENCRYPTIONMANAGER_H
