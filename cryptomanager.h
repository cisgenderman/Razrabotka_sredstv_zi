#ifndef CRYPTOMANAGER_H
#define CRYPTOMANAGER_H

#include <QObject>
#include <QByteArray>
#include <QCryptographicHash>
#include <QDebug>
#include <QString>

class cryptomanager : public QObject
{
    Q_OBJECT

private:
    explicit cryptomanager(QObject *parent = nullptr);
    cryptomanager(const cryptomanager&) = delete;
    cryptomanager& operator=(const cryptomanager&) = delete;
    static cryptomanager *instance;

public:
    static cryptomanager* getInstance();
    QByteArray encrypt(const QByteArray &data, const QByteArray &key);
    QByteArray decrypt(const QByteArray &data, const QByteArray &key);
    QString encryptString(const QString &text, const QString &key);
    QString decryptString(const QString &cipher, const QString &key);

};

#endif // CRYPTOMANAGER_H
