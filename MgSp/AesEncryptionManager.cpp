#include "AesEncryptionManager.h"
#include <QCryptographicHash>
#include <QRandomGenerator>
#include <QDebug>
#include <QRandomGenerator>

AesEncryptionManager::AesEncryptionManager(const QString &login, const QString &password) {
    key = createAesKey(login, password);

    // Используем хэш пароля в качестве IV, вместо случайного IV
    iv = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).left(16); // 16 байт для AES
}

QByteArray AesEncryptionManager::createAesKey(const QString &login, const QString &password) {
    QString combined = login + password;
    QByteArray hashedKey = QCryptographicHash::hash(combined.toUtf8(), QCryptographicHash::Sha256);
    return hashedKey.left(32); // Для AES-256 нужен ключ длиной 32 байта
}

QByteArray AesEncryptionManager::encrypt(const QByteArray &data) {
    QAESEncryption encryption(QAESEncryption::AES_256, QAESEncryption::CBC);

    // Выполняем шифрование
    QByteArray encrypted = encryption.encode(data, key, iv);
    return iv + encrypted; // Возвращаем IV + зашифрованные данные
}

QByteArray AesEncryptionManager::decrypt(const QByteArray &encryptedData) {
    QAESEncryption encryption(QAESEncryption::AES_256, QAESEncryption::CBC);

    // Извлекаем IV из начала данных
    QByteArray iv = encryptedData.left(16);
    QByteArray cipherText = encryptedData.mid(16);

    // Выполняем расшифровку
    QByteArray decrypted = encryption.decode(cipherText, key, iv);
    return decrypted;
}
