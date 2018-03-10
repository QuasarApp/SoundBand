#ifndef EXAPTIONS_H
#define EXAPTIONS_H

#include <exception>
#include <QString>
#include <QTranslator>
#include "config.h"

#ifdef  LOGER
#include "Log.h"
#endif

#ifdef  LOGER
   extern Log debug_log;
#endif

/**
 * @brief The MediaException class
 */

class BaseException
{
protected:
    QString _what;

public:
    virtual const char* what()
    {
        return _what.toLatin1().data();
    }

    void log()const{
#ifdef  LOGER
        debug_log.write(_what, ERROR);
#endif
        return;
    }
};

class MediaException:public BaseException
{
public:
    MediaException(){
        _what = QObject::tr("Your operating system or platform has not supported media files.");
        log();

    }
};

class AddNodeExaption:public BaseException
{
public:
    AddNodeExaption(){
        _what = QObject::tr("Address not available");
        log();
    }
};

class initNodeError:public BaseException
{
public:
    initNodeError(){
        _what = QObject::tr("The node on this device could not be deployed.");
        log();
    }
};

class CreatePackageExaption:public BaseException
{
public:
    CreatePackageExaption(){
        _what = QObject::tr("Сould not generate network packet");
        log();
    }
};

class SyncCountError:public BaseException
{
public:
    SyncCountError(){
        _what = QObject::tr("Could not sync audio.");
        log();
    }
};

class BadAnswerExaption:public BaseException
{
public:
    BadAnswerExaption(){
        _what = QObject::tr("could not parse message nodes.");
        log();
    }
};

class BrodcastConflict:public BaseException
{
public:
    BrodcastConflict(){
        _what = QObject::tr("The server received the packet from the server.");
        log();
    }
};

class SyncError:public BaseException
{
public:
    SyncError(){
        _what = QObject::tr("The playlist is empty, the player has nothing to play.");
        log();
    }
};

class InitDBError:public BaseException
{
public:
    InitDBError(){
        _what = QObject::tr("Error creating database.");
        log();
    }
};

class NetworkError:public BaseException
{
public:
    NetworkError(){
        _what = QObject::tr("There was an error on the socket, the connection will be closed.");
        log();
    }
};

class DataBaseError:public BaseException
{
public:
    DataBaseError(){
        _what = QObject::tr("Find duplicate of database item.");
        log();
    }
};

class NotSupported:public BaseException
{
public:
    NotSupported(){
        _what = QObject::tr("This option not supported.");
        log();
    }
};

#endif // EXAPTIONS_H
