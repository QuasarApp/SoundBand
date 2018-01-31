#ifndef LOG_H
#define LOG_H

#include <QFile>
#include <QTextStream>
#include <QDateTime>

enum LogType{ERROR, WARNING, INFORMATION, NONE};

class Log
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
