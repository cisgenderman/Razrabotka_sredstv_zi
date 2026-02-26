#include "cryptomanager.h"

cryptomanager* cryptomanager::instance = nullptr;

cryptomanager::cryptomanager(QObject *parent) :QObject(parent)
{
    qDebug() << "Крипто создан";
}

cryptomanager* cryptomanager::getInstance();
{
    if (instance == nullptr)
    {
        instance = new cryptomanager();
    }
    return instance;
}

QByteArray cryptomanager::encrypt(const QByteArray &data, const QByteArray &key)
{
    if (key.isEmpty())
    {
        return data;
    }

    QByteArray result = data;
    int keySize = key.size();

    for (int i = 0; i<result.size(); i++)
    {
        result[i] = result[i] ^ key[i % keySize];
    }
    return result;
}

QByteArray cryptomanager::decrypt(const QString &data, const QString &key)
{
    return encrypt(data, key);
}

QString cryptomanager::encryptString(const QString &text, const QString &key)
{
    QByteArray data = text.toUtf8();
    QByteArray keyData = key.toUtf8();
    QByteArray encrypted = encrypt(data, keyData);
    return QString::fromUtf8(encrypted.toBase64());
}

QString cryptomanager::decryptString(const QString &cipher, const QString &key)
{
    QByteArray data = QByteArray::fromBase64((cipher.toUtf8()));
    QByteArray keyData = key.toUtf8();
    QByteArray decrypted = decrypt(data, keyData);
    return QString::fromUtf8(decrypted);
}
