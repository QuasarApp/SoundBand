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

#endif // EXAPTIONS_H
