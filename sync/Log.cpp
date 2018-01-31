#include "Log.h"

Log::Log(const QString &fileName)
{
    m_showDate = true;
    if (!fileName.isEmpty()) {
        file = new QFile;
        file->setFileName(fileName);
        file->open(QIODevice::Append | QIODevice::Text);
    }
}

void Log::write(const QString &value, LogType logType) {
    QString text = value;// + "";
    switch (logType) {
    case INFORMATION:
        text = "INFORMATION: " + text;
        break;
    case WARNING:
        text = "WARNING: " + text;
        break;
    case ERROR:
        text = "ERROR: " + text;
        break;
    default:
        break;
    }
    if (m_showDate)
        text = QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss ") + text;
    QTextStream out(file);
    out.setCodec("UTF-8");
    if (file != 0){
        out << text + "\n";
    }
}

void Log::setShowDateTime(bool value) {
    m_showDate = value;
}

Log::~Log() {
    if (file != 0)
    file->close();
}
