#ifndef LocalServers_H
#define LocalServers_H
#include <QNetworkInterface>
#include <QHostInfo>
#include <QList>
#include <QTimer>
#include "ETcpSocket.h"

/**
 * @brief The LocalScanner class
 * The principle of operation of a local scanner.
 * The local scanner gets its address on the local network,
 * the resulting address is converted into a subnet mask.
 * Then it checks for the presence of addresses in the range of the received mask.
 * The available addresses are added to the list.
 */
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
    /**
     * @brief scane - start scaning
     */
    void scane();
    /**
     * @brief setInterval - set a new waiting interval
     * @param msec
     */
    void setInterval(int msec);
    /**
     * @brief thisAdress - return addres of local network
     * @return addres
     */
    static QHostAddress thisAdress();
    LocalScanner();
    ~LocalScanner();
signals:
    /**
     * @brief scaned - sigbal if all done
     */
    void scaned(QList<ETcpSocket*>*);
};
#endif // LocalServers_H
