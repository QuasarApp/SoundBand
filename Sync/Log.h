#ifndef LOG_H
#define LOG_H

#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include "sync_global.h"

enum SYNCSHARED_EXPORT LogType{ERROR, WARNING, INFORMATION, NONE};

class SYNCSHARED_EXPORT Log
{
public:
    explicit Log(const QString &fileName);
    ~Log();
    void setShowDateTime(bool value);
    void write(const QString &value, LogType logType = NONE);

private:
    QFile *file;
    bool m_showDate;

};

#endif // LOG_H
