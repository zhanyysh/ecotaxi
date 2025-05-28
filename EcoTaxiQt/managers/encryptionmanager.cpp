#include "EncryptionManager.h"

// Get the singleton instance
EncryptionManager& EncryptionManager::getInstance() {
    static EncryptionManager instance;
    return instance;
}

// Set the key for encryption/decryption
void EncryptionManager::setKey(const QString& key) {
    this->key = key;
}

// Encrypt the input value using Vigenère cipher
QString EncryptionManager::encrypt(const QString& value) {
    QString encryptedText;
    int keyLength = key.length();
    int keyIndex = 0;

    for (QChar ch : value) {
        // Handle ASCII printable characters (32 to 126)
        if (ch.unicode() >= 32 && ch.unicode() <= 126) {
            int shift = key[keyIndex % keyLength].unicode();
            QChar encryptedChar = QChar(((ch.unicode() - 32 + (shift % 95)) % 95) + 32);
            encryptedText.append(encryptedChar);
            keyIndex++;
        } else {
            // Leave characters outside printable ASCII range unchanged
            encryptedText.append(ch);
        }
    }

    return encryptedText;
}

// Decrypt the input value using Vigenère cipher
QString EncryptionManager::decrypt(const QString& value) {
    QString decryptedText;
    int keyLength = key.length();
    int keyIndex = 0;

    for (QChar ch : value) {
        // Handle ASCII printable characters (32 to 126)
        if (ch.unicode() >= 32 && ch.unicode() <= 126) {
            int shift = key[keyIndex % keyLength].unicode();
            QChar decryptedChar = QChar(((ch.unicode() - 32 - (shift % 95) + 95) % 95) + 32);
            decryptedText.append(decryptedChar);
            keyIndex++;
        } else {
            // Leave characters outside printable ASCII range unchanged
            decryptedText.append(ch);
        }
    }

    return decryptedText;
}
