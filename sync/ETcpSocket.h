#ifndef CLIENT_H
#define CLIENT_H
#include <QTcpSocket>
#include <QTcpServer>
#include <QList>
#include <QDataStream>
#include "syncpackage.h"


/**
 * @brief The ETcpSocket class
 * example :
 * ETcpSocket *tcp;
 * try{
 *  tcp = new ETcpSocket(addres,port);
 * }catch(addNodeExaption e){
 *  e.what();
 * }
 * QByteArray *array;
 * while(array = tcp.getSource()){
 *  package pkg(*array);
 *  package ans = ansver(pkg);
 *  tcp.Write(ans);
 *  tcp->nextItem();

 *
 * }
 *
 */


class ETcpSocket:public QObject
{
    Q_OBJECT
private:
    QTcpSocket *source;
    QByteArray *array;
    qint32 size;
    milliseconds time;
    milliseconds lastTime;
    char precisionSync;
    QList<QByteArray*> ReadyStack;
    bool fSynced;
    QMap<char ,SyncPackage> syncList;
    void init();

    /**
     * @brief _driverResponse
     * @param pac
     * @return true if all done
     */
    bool _driverResponse(const SyncPackage &from);

    /**
     * @brief _driverResponse
     * @param pac
     * @return true if all done
     */
    void _driverStart();

    /**
     * @brief _driver
     * @return true is package of Driver
     */
    void _driver(QByteArray*);

    /**
     * @brief Write - sends a message to the network.
     * @param isDriver - flag of driver info
     * @return true if all done else false.
     */
    bool _Write(const QByteArray&, bool isDriver = false);

private slots:

    void connected_();
    void disconnected_();
    void error_(QAbstractSocket::SocketError socketError);
    void hostFound_();
    void readReady_();
    void proxyAuthenticationRequired_(const QNetworkProxy &proxy, QAuthenticator *authenticator);
    void stateChanged_(QAbstractSocket::SocketState socketState);
public:
    explicit ETcpSocket();
    explicit ETcpSocket(QTcpSocket*);
    explicit ETcpSocket(const QString& addres,int port);

    /**
     * @brief sync
     */
    void sync();

    /**
     * @brief isSynced
     * @return true
     */
    bool isSynced()const;

    /**
     * @brief setCheckInterval - set new interval of chking ping
     * Check will be conducted within this interval
     */
    void setCheckInterval(int newInterval);

    /**
     * @brief getCheckInterval
     * @return interval of cheking network ping.
     */
    int getCheckInterval()const;

    /**
     * @brief setTime set new Time
     */
    void setTime(milliseconds newTime);

    /**
     * @brief getTime
     * @return time of soccket;
     */
    milliseconds getTime()const;
    /**
     * @brief getSource
     * @return Qt TCP socket
     */
    QTcpSocket* getSource()const;
    /**
     * @brief topStack
     * @return top of a message stack.
     */
    QByteArray* topStack();
    /**
     * @brief nextItem - Toggle header to next message.
     * You need to call this function after the top Stack.
     * @param free - clear memory of top stack (true by default)
     */
    void nextItem(bool free = true);

    /**
     * @brief sizeDescriptPackege
     * @return size of Descript of Packege
     */
    int sizeDescriptPackege();

    /**
     * @brief isValid
     * @return true if socket active;
     */
    bool isValid();

    /**
     * @brief Write - sends a message to the network.
     * @return true if all done else false.
     */
    bool Write(const QByteArray&);

    ~ETcpSocket();
public slots:

    /**
     * @brief peerName
     * @return name of peer
     */
    QString peerName()const;

    /**
     * @brief name
     * @return aame of local
     */
    QString localName()const;

    /**
     * @brief toStringTcp
     * @return string of tcp address
     */
    QString toStringTcp();
signals:
    /**
     * @brief donwload - signal when received a package.
     * @param val - size of package
     * @param max - real received package size
     */
    void donwload(int val,int max);
    /**
     * @brief Connected - signal when connected with other socket.
     */
    void Connected(ETcpSocket*);
    /**
     * @brief Message - signal when received a Message.
     */
    void Message(ETcpSocket*);
    /**
     * @brief Disconnected -  signal when disconnected with other socket.
     */
    void Disconnected(ETcpSocket*);
    /**
     * @brief Error - signal if there are problems with the connection.
     * @param socketError - information about error.
     */
    void Error(ETcpSocket*,QAbstractSocket::SocketError socketError);
    /**
     * @brief HostFound This signal is emitted after connectToHost() has been called and the host lookup has succeeded.
     * Note: Since Qt 4.6.3 QAbstractSocket may emit hostFound()
     * directly from the connectToHost() call since a DNS result could have been cached.
     */
    void HostFound(ETcpSocket*);
    /**
     * @brief ProxyAuthenticationRequired
     * This signal can be emitted when a proxy that requires authentication is used.
     * The authenticator object can then be filled in with the required details to allow authentication and continue the connection.
     * Note: It is not possible to use a QueuedConnection to connect to this signal,
     * as the connection will fail if the authenticator has not been filled in with new information when the signal returns.
     */
    void ProxyAuthenticationRequired(ETcpSocket*,const QNetworkProxy &proxy, QAuthenticator *authenticator);
    /**
     * @brief StateChanged This signal is emitted whenever QAbstractSocket's state changes.
     *  The socketState parameter is the new state.
     */
    void StateChanged(ETcpSocket*,QAbstractSocket::SocketState socketState);


    /**
     * @brief synced emited when host is synced
     */
    void synced();


};

#endif // CLIENT_H
