#include "CryptoManager.h"
#include <QFile>
#include <fstream>
#include <iostream>
#include <QCryptographicHash>
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
    std::cout << "DEBUG: keyFromPassword started" << std::endl;
    std::string pass = password.toStdString();
    std::cout << "DEBUG: pass size = " << pass.size() << std::endl;

    SecByteBlock key(AES::DEFAULT_KEYLENGTH);
    std::cout << "DEBUG: SecByteBlock created, size = " << key.size() << std::endl;

    try {
        SHA256().CalculateDigest(key, reinterpret_cast<const byte*>(pass.data()), pass.size());
        std::cout << "DEBUG: SHA256 calculated" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "DEBUG: Exception: " << e.what() << std::endl;
    }

    QByteArray result(reinterpret_cast<const char*>(key.data()), key.size());
    std::cout << "DEBUG: keyFromPassword finished" << std::endl;
    return result;
}

bool CryptoManager::processAES(const QString &inputFilePath, const QString &outputFilePath,
                               const QByteArray &key, bool forEncryption) {
    std::cout << "DEBUG: processAES started" << std::endl;
    std::cout << "DEBUG: input file: " << inputFilePath.toStdString() << std::endl;
    std::cout << "DEBUG: output file: " << outputFilePath.toStdString() << std::endl;
    std::cout << "DEBUG: key size: " << key.size() << std::endl;

    QString tempFile = inputFilePath + ".tmp";
    try {
        std::string inFile = inputFilePath.toStdString();
        std::string tempFileStr = tempFile.toStdString();


        // Проверяем, существует ли входной файл
        if (!QFile::exists(inputFilePath))
        {
            std::cout << "DEBUG: Input file does not exist!" << std::endl;
            return false;
        }

        byte iv[AES::BLOCKSIZE] = {0};
        SecByteBlock keyBlock(reinterpret_cast<const byte*>(key.data()), key.size());

        std::cout << "DEBUG: Starting Crypto++ operation..." << std::endl;

        if (forEncryption)
        {
            CBC_Mode<AES>::Encryption encryptor;
            encryptor.SetKeyWithIV(keyBlock, keyBlock.size(), iv);

            FileSource fs(inFile.c_str(), true,
                          new StreamTransformationFilter(encryptor,
                                                         new FileSink(tempFileStr.c_str())
                                                         )
                          );
        }
        else
        {
            CBC_Mode<AES>::Decryption decryptor;
            decryptor.SetKeyWithIV(keyBlock, keyBlock.size(), iv);

            FileSource fs(inFile.c_str(), true,
                          new StreamTransformationFilter(decryptor,
                                                         new FileSink(tempFileStr.c_str())
                                                         )
                          );
        }

        std::cout << "DEBUG: Crypto++ operation completed" << std::endl;

        // Заменяем исходный файл зашифрованным
        if (QFile::exists(tempFile))
        {
            QFile::remove(inputFilePath);
            QFile::rename(tempFile, inputFilePath);
        }
        else
        {
            return false;
        }

        return true;

    }
    catch (const CryptoPP::Exception& e)
    {
        std::cout << "DEBUG: Crypto++ error: " << e.what() << std::endl;
        if (QFile::exists(tempFile))
        {
            QFile::remove(tempFile);
        }
        return false;
    }
    catch (const std::exception& e)
    {
        std::cout << "DEBUG: std exception: " << e.what() << std::endl;
        if (QFile::exists(tempFile))
        {
            QFile::remove(tempFile);
        }
        return false;
    }
    catch (...)
    {
        std::cout << "DEBUG: Unknown exception" << std::endl;
        if (QFile::exists(tempFile))
        {
            QFile::remove(tempFile);
        }
        return false;
    }
}

bool CryptoManager::encryptFile(const QString &inputFilePath, const QString &outputFilePath, const QByteArray &key)
{
    return processAES(inputFilePath, outputFilePath, key, true);
}

bool CryptoManager::decryptFile(const QString &inputFilePath, const QString &outputFilePath, const QByteArray &key)
{
    return processAES(inputFilePath, outputFilePath, key, false);
}
