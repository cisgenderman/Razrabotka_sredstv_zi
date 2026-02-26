#ifndef CRYPTOMANAGER_H
#define CRYPTOMANAGER_H

#include <QByteArray>
#include <QString>
#include <QDebug>

// Класс-синглтон для операций шифрования (AES-256)
class CryptoManager
{
public:
    // Запрещаем копирование и присваивание
    CryptoManager(const CryptoManager&) = delete;
    CryptoManager& operator=(const CryptoManager&) = delete;

    // Единственный метод для получения экземпляра
    static CryptoManager& getInstance();

    // Основные методы: зашифровать/расшифровать файл
    // Возвращает true при успехе, false при ошибке
    bool encryptFile(const QString &inputFilePath, const QString &outputFilePath, const QByteArray &key);
    bool decryptFile(const QString &inputFilePath, const QString &outputFilePath, const QByteArray &key);

    // Вспомогательная функция для генерации ключа из пароля (через хеширование)
    QByteArray keyFromPassword(const QString &password);

private:
    // Приватный конструктор
    CryptoManager() = default;

    // Здесь будет код с использованием Crypto++ (см. следующий пункт)
    bool processAES(const QString &inputFilePath, const QString &outputFilePath,
                    const QByteArray &key, bool forEncryption);
};

#endif // CRYPTOMANAGER_H
