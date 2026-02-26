#include "folderscanner.h"
#include <QDir>
#include <QFileInfo>
#include <QDebug>

FolderScanner::FolderScanner(QObject *parent) : QObject(parent)
{

}

bool FolderScanner::processDirectory(const QString &dirPath, const QByteArray &key, bool encryptMode)
{
    return processDirectoryRecursive(dirPath, key, encryptMode);
}

bool FolderScanner::processDirectoryRecursive(const QString &dirPath, const QByteArray &key, bool encryptMode)
{
    QDir directory(dirPath);
    if (!directory.exists())
    {
        qCritical() << "папки не существует" << dirPath;
        return false;
    }

    bool success = true;
    QFileInfoList entries = directory.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QFileInfo &entry :entries)
    {
        if (entry.isDir())
        {
            emit directoryEntered(entry.absoluteFilePath());
            if (!processDirectoryRecursive(entry.absoluteFilePath(), key, encryptMode))
            {
                success = false;
            }
            else if (entry.isFile())
            {
                QString inFile = entry.absoluteFilePath();
                QString outFile = inFile + (encryptMode ? ".enc" : ".dec");
                qInfo() << (encryptMode ? "Шифрование" : "Расшифрование") << inFile;

                bool result;

                if (encryptMode)
                {
                    result = CryptoManager::getInstance().encryptFile(inFile, outFile, key);
                }
                else
                {
                    result = CryptoManager::getInstance().encryptFile(inFile, outFile, key);
                }

                if (result)
                {
                    qInfo() << "ok";
                    emit fileProcessed(inFile, false);
                    success = false;
                }
            }
        }
    }
    return success;
}
