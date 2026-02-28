#include "CryptoManager.h"
#include <fstream>
#include <iostream>

#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <cryptopp/sha.h>
#include <cryptopp/files.h>
#include <cryptopp/secblock.h>

using namespace CryptoPP;

CryptoManager& CryptoManager::getInstance()
{
    static CryptoManager instance;
    return instance;
}

QByteArray CryptoManager::keyFromPassword(const QString &password)
{
    // Преобразуем пароль в байтовый массив
    std::string pass = password.toStdString();
    SecByteBlock key(AES::DEFAULT_KEYLENGTH); // 256 бит = 32 байта

    // Используем SHA-256 для получения ключа фиксированной длины из пароля
    SHA256().CalculateDigest(key, reinterpret_cast<const byte*>(pass.data()), pass.size());

    return QByteArray(reinterpret_cast<const char*>(key.data()), key.size());
}

bool CryptoManager::processAES(const QString &inputFilePath, const QString &outputFilePath,
                               const QByteArray &key, bool forEncryption) {
    try {
        std::string inFile = inputFilePath.toStdString();
        std::string outFile = outputFilePath.toStdString();

        // Устанавливаем ключ и вектор инициализации (IV)
        // Внимание: В реальных проектах IV должен быть случайным и сохраняться!
        // Для упрощения лабораторной используем фиксированный IV (небезопасно!)
        byte iv[AES::BLOCKSIZE] = {0}; // В реальности должен быть случайным

        SecByteBlock keyBlock(reinterpret_cast<const byte*>(key.data()), key.size());

        if (forEncryption) {
            // Шифрование
            CBC_Mode<AES>::Encryption encryptor;
            encryptor.SetKeyWithIV(keyBlock, keyBlock.size(), iv);

            FileSource fs(inFile.c_str(), true,
                          new StreamTransformationFilter(encryptor,
                                                         new FileSink(outFile.c_str())
                                                         )
                          );
        } else {
            // Дешифрование
            CBC_Mode<AES>::Decryption decryptor;
            decryptor.SetKeyWithIV(keyBlock, keyBlock.size(), iv);

            FileSource fs(inFile.c_str(), true,
                          new StreamTransformationFilter(decryptor,
                                                         new FileSink(outFile.c_str())
                                                         )
                          );
        }
        return true;
    } catch (const CryptoPP::Exception& e) {
        std::cerr << "Crypto++ error: " << e.what() << std::endl;
        return false;
    }
}

bool CryptoManager::encryptFile(const QString &inputFilePath, const QString &outputFilePath, const QByteArray &key) {
    return processAES(inputFilePath, outputFilePath, key, true);
}

bool CryptoManager::decryptFile(const QString &inputFilePath, const QString &outputFilePath, const QByteArray &key) {
    return processAES(inputFilePath, outputFilePath, key, false);
}
