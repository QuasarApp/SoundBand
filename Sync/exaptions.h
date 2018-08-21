#ifndef EXAPTIONS_H
#define EXAPTIONS_H

#include <exception>
#include <QString>
#include <QTranslator>
#include "config.h"
#include "sync_global.h"

#ifdef  LOGER
#include "Log.h"
#endif

#ifdef  LOGER
   extern Log debug_log;
#endif

/**
 * @brief The MediaException class
 */

class SYNCSHARED_EXPORT BaseException
{
protected:
    QString _what;

public:
    virtual const char* what();

    void log()const;

    virtual ~BaseException() {}
};

class SYNCSHARED_EXPORT MediaException:public BaseException
{
public:
    MediaException();
};

class SYNCSHARED_EXPORT AddNodeExaption:public BaseException
{
public:
    AddNodeExaption();
};

class SYNCSHARED_EXPORT initNodeError:public BaseException
{
public:
    initNodeError();
};

class SYNCSHARED_EXPORT CreatePackageExaption:public BaseException
{
public:
    CreatePackageExaption();
};

class SYNCSHARED_EXPORT SyncCountError:public BaseException
{
public:
    SyncCountError();
};

class SYNCSHARED_EXPORT BadAnswerExaption:public BaseException
{
public:
    BadAnswerExaption();
};

class SYNCSHARED_EXPORT BrodcastConflict:public BaseException
{
public:
    BrodcastConflict();
};

class SYNCSHARED_EXPORT SyncError:public BaseException
{
public:
    SyncError();
};

class SYNCSHARED_EXPORT InitDBError:public BaseException
{
public:
    InitDBError();
};

class SYNCSHARED_EXPORT NetworkError:public BaseException
{
public:
    NetworkError();
};

class SYNCSHARED_EXPORT DataBaseError:public BaseException
{
public:
    DataBaseError();
};

class SYNCSHARED_EXPORT NotSupported:public BaseException
{
public:
    NotSupported();
};

#endif // EXAPTIONS_H
