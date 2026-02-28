#ifndef CRYPTOMANAGER_H
#define CRYPTOMANAGER_H

#include <QByteArray>
#include <QString>
#include <QDebug>

class CryptoManager
{
public:
    CryptoManager(const CryptoManager&) = delete;
    CryptoManager& operator=(const CryptoManager&) = delete;

    static CryptoManager& getInstance();

    bool encryptFile(const QString &inputFilePath, const QString &outputFilePath, const QByteArray &key);
    bool decryptFile(const QString &inputFilePath, const QString &outputFilePath, const QByteArray &key);

    QByteArray keyFromPassword(const QString &password);

private:
    CryptoManager() = default;

    bool processAES(const QString &inputFilePath, const QString &outputFilePath,
                    const QByteArray &key, bool forEncryption);
};

#endif // CRYPTOMANAGER_H
