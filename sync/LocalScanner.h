#ifndef LocalServers_H
#define LocalServers_H
#include <QNetworkInterface>
#include <QHostInfo>
#include <QList>
#include <QTimer>
#include "ETcpSocket.h"

class LocalScanner:public QObject{
    Q_OBJECT
private:
    QList<ETcpSocket*> socets;
    QList<ETcpSocket*> results;
    QTimer wiat;
    void clear();
    void clearSocets();
private slots:
    void scaned_();
    void connected(ETcpSocket *);
public:
    void scane();
    void setInterval(int msec);
    static QHostAddress thisAdress();
    LocalScanner();
    ~LocalScanner();
signals:
    void scaned(QList<ETcpSocket*>*);
};
#endif // LocalServers_H
