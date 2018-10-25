#ifndef NODE_H
#define NODE_H
#include <QTcpServer>
#include "ETcpSocket.h"
#include <QTimer>
#include "basepackage.h"
#include "syncpackage.h"
#include "sync_global.h"

/**
 * @brief The Node class is tcp server class
 */
class SYNCSHARED_EXPORT Node:public QTcpServer{
    Q_OBJECT
private:
    QTimer *timer;
    int index;
protected:
    QList<ETcpSocket*> clients;
    bool fBroadcaster;
    int step;
private slots:
    void synced();
    void acceptError_(ETcpSocket*);
    void newConnection_();
    void readData(ETcpSocket*_client);
public:
    Node(const QString &addres = DEFAULT_ADRESS, int port = DEFAULT_PORT);

    /**
     * @brief isBroadcaster
     * @return true if this node is server
     */
    bool isBroadcaster()const;

    /**
     * @brief setBroadcaster set new state for this node
     */
    void setBroadcaster(bool newValue);

    /**
     * @brief WriteAll send package to all connected clients
     */
    void WriteAll(const QByteArray&);

    /**
     * @brief disconnectClient disconet a client
     */
    void disconnectClient(ETcpSocket*);

    /**
     * @brief getClients
     * @return list of all connected clients
     */
    QList<ETcpSocket*>* getClients();

    /**
     * @brief addNode add new client for network
     * @param node if of node
     * @param port port of node
     * @return true if all done
     */
    bool addNode(const QString &node, int port);

    /**
     * @brief addNode a connected node
     * @param node tcp socket
     * @return true if all done
     */
    bool addNode(ETcpSocket* node);

    ~Node();
signals:
    /**
     * @brief Error signal when a error detected
     */
    void Error(QString);

    /**
     * @brief Message signal when accepted a mewssage from other node
     */
    void Message(ETcpSocket*);

    /**
     * @brief ClientDisconnected - signal when node disconected from this node
     */
    void ClientDisconnected(ETcpSocket*);

    /**
     * @brief ClientConnected signal when connected a new node
     */
    void ClientConnected(ETcpSocket*);

    /**
     * @brief NodeSynced emited when socket synced
     */
    void NodeSynced(ETcpSocket*);

};



#endif // NODE_H
