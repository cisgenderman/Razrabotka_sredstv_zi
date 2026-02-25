#include <QCoreApplication>
#include <QDebug>
#include "folderscanner.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if (argc != 4)
    {
        qInfo() << "Использование: <шифрование/расшифрование> <путь к папке> <пароль>";
        return 1;
    }

    QString mode = QString(argv[1]).toLower();
    QString folderPath = QString(argv[2]);
    QString password = QString(argv[3]);

    bool encryptMode;
    if (mode == "шифрование")
    {
        encryptMode = true;
    }
    else if (mode == "расшифрование")
    {
        encryptMode = false;
    }
    else
    {
        qCritical() << "не правильно выбран режим";
        return 1;
    }

    QByteArray key = CryptoManager::getInstance().keyFromPassword(password);

    FolderScanner scanner;
    bool result = scanner.processDirecoryr(folderPath, key, encryptMode);

    if (result)
    {
        qInfo() << "операция выполнена успешно";
        return 0;
    }
    else
    {
        qCritical() << "операция выполнена с ошибками";
        return 1;
    }
}
