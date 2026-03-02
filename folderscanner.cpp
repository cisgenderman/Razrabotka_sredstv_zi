#include "folderscanner.h"
#include "CryptoManager.h"
#include <QDir>
#include <QFileInfo>
#include <QDebug>
#include <iostream>

FolderScanner::FolderScanner(QObject *parent) : QObject(parent)
{

}

bool FolderScanner::processDirectory(const QString &dirPath, const QByteArray &key, bool encryptMode)
{
    std::cout << "INSIDE processDirectory: " << dirPath.toStdString() << std::endl;
    return processDirectoryRecursive(dirPath, key, encryptMode);
}

bool FolderScanner::processDirectoryRecursive(const QString &dirPath, const QByteArray &key, bool encryptMode)
{
    std::cout << "processDirectoryRecursive START: " << dirPath.toStdString() << std::endl;

    QDir directory(dirPath);
    if (!directory.exists())
    {
        qCritical() << "папки не существует" << dirPath;
        return false;
    }

    std::cout << "Reading directory entries..." << std::endl;
    QFileInfoList entries = directory.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
    std::cout << "Entries found: " << entries.size() << std::endl;

    bool success = true;

    for (const QFileInfo &entry : entries)
    {
        std::cout << "Processing entry: " << entry.fileName().toStdString() << std::endl;

        if (entry.isDir())
        {
            std::cout << "It's a directory" << std::endl;
            if (!processDirectoryRecursive(entry.absoluteFilePath(), key, encryptMode))
            {
                success = false;
            }
        }
        else if (entry.isFile())
        {
            std::cout << "It's a FILE! Filename: " << entry.fileName().toStdString() << std::endl;

            QString inFile = entry.absoluteFilePath();
            QString outFile = inFile + (encryptMode ? ".enc" : ".dec");

            std::cout << "Calling encryptFile for: " << inFile.toStdString() << std::endl;

            bool result;
            if (encryptMode)
            {
                result = CryptoManager::getInstance().encryptFile(inFile, outFile, key);
            }
            else
            {
                result = CryptoManager::getInstance().decryptFile(inFile, outFile, key);
            }

            std::cout << "encryptFile returned: " << result << std::endl;

            if (result)
            {
                std::cout << "OK" << std::endl;
                emit fileProcessed(inFile, true);
            }
            else
            {
                std::cout << "ERROR" << std::endl;
                emit fileProcessed(inFile, false);
                success = false;
            }
        }
    }

    std::cout << "processDirectoryRecursive FINISHED: " << dirPath.toStdString() << std::endl;
    return success;
}
