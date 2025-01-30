#ifndef AESENCRYPTIONMANAGER_H
#define AESENCRYPTIONMANAGER_H

#include <QString>
#include <QByteArray>
#include "qaesencryption.h"

class AesEncryptionManager {
public:
    AesEncryptionManager(const QString &login, const QString &password);

    QByteArray encrypt(const QByteArray &data);
    QByteArray decrypt(const QByteArray &encryptedData);

private:
    QByteArray createAesKey(const QString &login, const QString &password);
    QByteArray key;
    QByteArray iv;
};

#endif // AESENCRYPTIONMANAGER_H
