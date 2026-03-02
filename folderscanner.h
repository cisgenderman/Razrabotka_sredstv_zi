#ifndef FOLDERSCANNER_H
#define FOLDERSCANNER_H

#include <QObject>
#include <qbytearray>

class FolderScanner : public QObject
{
    Q_OBJECT
public:
    explicit FolderScanner(QObject *parent = nullptr);

    bool processDirectory(const QString &dirPath, const QByteArray &key, bool encryptMode);

signals:
    void fileProcessed(const QString &fileName, bool success);
    void directoryEntered(const QString &dirName);

private:
    bool processDirectoryRecursive(const QString &dirPath, const QByteArray &key, bool encryptMode);
};

#endif // FOLDERSCANNER_H
