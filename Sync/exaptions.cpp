#include "exaptions.h"

#ifdef  LOGER
    Log debug_log(LOG_FILE);
#endif

    const char *BaseException::what() {
        return _what.toLatin1().data();
    }

    void BaseException::log() const{
#ifdef  LOGER
        debug_log.write(_what, ERROR);
#endif
        return;
    }

    MediaException::MediaException(){
        _what = QObject::tr("Your operating system or platform has not supported media files.");
        log();

    }

    AddNodeExaption::AddNodeExaption(){
        _what = QObject::tr("Address not available");
        log();
    }

    initNodeError::initNodeError(){
        _what = QObject::tr("The node on this device could not be deployed.");
        log();
    }

    CreatePackageExaption::CreatePackageExaption(){
        _what = QObject::tr("Сould not generate network packet");
        log();
    }

    SyncCountError::SyncCountError(){
        _what = QObject::tr("Could not sync audio.");
        log();
    }

    BadAnswerExaption::BadAnswerExaption(){
        _what = QObject::tr("could not parse message nodes.");
        log();
    }

    BrodcastConflict::BrodcastConflict(){
        _what = QObject::tr("The server received the packet from the server.");
        log();
    }

    SyncError::SyncError(){
        _what = QObject::tr("The playlist is empty, the player has nothing to play.");
        log();
    }

    InitDBError::InitDBError(){
        _what = QObject::tr("Error creating database.");
        log();
    }

    NetworkError::NetworkError(){
        _what = QObject::tr("There was an error on the socket, the connection will be closed.");
        log();
    }

    DataBaseError::DataBaseError(){
        _what = QObject::tr("Find duplicate of database item.");
        log();
    }

    NotSupported::NotSupported(){
        _what = QObject::tr("This option not supported.");
        log();
    }
