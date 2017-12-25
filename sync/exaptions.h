#ifndef EXAPTIONS_H
#define EXAPTIONS_H

#include <exception>
#include <QString>
#include <QTranslator>
/**
 * @brief The MediaException class
 */
class MediaException:public std::exception
{
public:
    QString what(){
        return QObject::tr("Your operating system or platform has not supported media files.");
    }
};

class AddNodeExaption:public std::exception
{
public:
    QString what(){
        return QObject::tr("Address not available");
    }
};

class initNodeError:public std::exception
{
public:
    QString what(){
        return QObject::tr("The node on this device could not be deployed.");
    }
};

class CreatePackageExaption:public std::exception
{
public:
    QString what(){
        return QObject::tr("Сould not generate network packet");
    }
};

class BadAnswerExaption:public std::exception
{
public:
    QString what(){
        return QObject::tr("could not parse message nodes.");
    }
};

class BrodcastConflict:public std::exception
{
public:
    QString what(){
        return QObject::tr("The server received the packet from the server.");
    }
};

class SyncError:public std::exception
{
public:
    QString what(){
        return QObject::tr("The playlist is empty, the player has nothing to play.");
    }
};

class InitDBError:public std::exception
{
public:
    QString what(){
        return QObject::tr("Error creating database.");
    }
};

#endif // EXAPTIONS_H
