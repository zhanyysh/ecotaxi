#include "hash.h"

HASH::HASH() {}

QString HASH::generateHMAC(const QString &data)
{
  QByteArray keyBytes = QString("Il05EmiN3cR@ftMe0w$4shdi|3").toUtf8();
  QByteArray dataBytes = data.toUtf8();
  QMessageAuthenticationCode code(QCryptographicHash::Sha256);
  code.setKey(keyBytes);
  code.addData(dataBytes);
  QByteArray hmacResult = code.result();
  return QString(hmacResult.toHex());
}

bool HASH::compareHMACs(const QString &hmac1, const QString &hmac2)
{
  return hmac1 == hmac2;
}
