#include <QCoreApplication>
#include <QDebug>
#include "folderscanner.h"
#include "CryptoManager.h"
#include <iostream>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    std::string input;
    int choice;

    while(true) {
        std::cout << "start\n";
        std::cout << "1. encrypt\n";
        std::cout << "2. decrypt\n";
        std::cout << "3. exit\n";
        std::cout << "choose: ";

        std::cin >> choice;

        if(choice == 3) {
            std::cout << "exit!\n";
            break;
        }

        if(choice != 1 && choice != 2) {
            std::cout << "try again.\n";
            continue;
        }

        // Получаем путь к папке
        std::cout << "path folder: ";
        std::cin.ignore(); // очищаем буфер
        std::getline(std::cin, input);
        QString folderPath = QString::fromStdString(input);

        // Получаем пароль
        std::cout << "password: ";
        std::getline(std::cin, input);
        QString password = QString::fromStdString(input);

        // Определяем режим
        bool encryptMode = (choice == 1);

        std::cout << (encryptMode ? "\nencription..." : "\ndecryption...") << std::endl;

        // Генерируем ключ
        QByteArray key = CryptoManager::getInstance().keyFromPassword(password);

        std::cout << "key_1.\n";
        // Запускаем сканер
        FolderScanner scanner;
        std::cout << "Calling processDirectory with: " << folderPath.toStdString() << std::endl;
        bool result = scanner.processDirectory(folderPath, key, encryptMode);
        std::cout << "processDirectory returned: " << result << std::endl;

        if(result)
        {
            std::cout << "sucsess!\n";
        }
        else
        {
            std::cout << "erorr!\n";
        }
    }
    a.quit();
    return 0;
}
