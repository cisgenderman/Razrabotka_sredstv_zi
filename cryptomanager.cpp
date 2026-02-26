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

QByteArray cryptomanager::encrypt(const )
