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

class addNodeExaption:public std::exception
{
public:
    QString what(){
        return QObject::tr("Address not available");
    }
};

class createPackageExaption:public std::exception
{
public:
    QString what(){
        return QObject::tr("Сould not generate network packet");
    }
};

class badAnswerExaption:public std::exception
{
public:
    QString what(){
        return QObject::tr("could not parse message nodes.");
    }
};

#endif // EXAPTIONS_H
