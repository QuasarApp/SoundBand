#ifndef SYNCPACKAGE_H
#define SYNCPACKAGE_H

#include "chronotime.h"
#include <QByteArray>

class ETcpSocket;

enum TypeSyncPackage{
     t_voidSync         =   0x00,
     t_Header           =   0x01,
     t_Responce         =   0x02,
     t_Source           =   0x04,
     t_End              =   0x08
};

class SyncPackage
{
private:

    TypeSyncPackage type;
    qint8 firstByte;
    milliseconds ping;
    milliseconds nativeTime;
    milliseconds sourceBytes;

public:
    SyncPackage();

    /**
     * @brief getType
     * @return type of package
     */
    TypeSyncPackage getType() const;

    /**
     * @brief getIndex
     * @return return index
     */
    char getIndex()const;

    /**
     * @brief getDelay
     * @return return delay
     */
    const milliseconds& getNative()const;

    /**
     * @brief getTime
     * @return time
     */
    const milliseconds& getTime()const;

    /**
     * @brief getPing
     * @return ping
     */
    const milliseconds& getPing()const;

    /**
     * @brief getPrecision
     * @return precision
     */
    char getPrecision()const;

    /**
     * @brief isSended
     * @return true if package sended
     */
    bool isSended()const;

    /**
     * @brief isValid
     * @return true if package is valid
     */
    bool isValid() const;

    /**
     * @brief clear all date of package
     */
    void clear();

    /**
     * @brief parseTo parse this package to byte array
     * @return byte array
     */
    QByteArray parseTo();

    /**
     * @brief parseFrom create a package from bytes
     * @param array of bytes
     * @return true if package valid
     */
    bool parseFrom(QByteArray& array);

    ~SyncPackage();
    friend class ETcpSocket;
};

#endif // SYNCPACKAGE_H
