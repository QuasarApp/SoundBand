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

#endif // EXAPTIONS_H
