#ifndef NODE_H
#define NODE_H
#include <QTcpServer>
#include "config.h"
class Song;
class Syncer;
namespace syncLib {

/**
 * @brief The package class. Package for translite media data on network
 *
 * parse map:
 * 1 byle - type
 * 4 byte - size of data of package (it avelable if type is t_sync or t_song)
 * data
 */
class package
{
    /*parse map */

/*
 * 1 byle - type
 * 4 byte - size of data of package (it avelable if type is t_sync or t_song)
 * data
*/
    /**
     * @brief The TypePackage enum
     * t_void - this package empty and not valid.
     * t_close - the information about close channel.
     * t_sync - the infomation about sync playning media file on network.
     * t_song - the package with this type is necessary for translite media data on network.
     * t_stop - the package with type 'stop' necessary for stoping playning media files.
     */
    enum TypePackage{
         t_void = 0x0,
         t_close = 0x1,
         t_sync = 0x2,
         t_song = 0x4,
         t_stop = 0x8
    };
private:
    TypePackage type;
    unsigned int size;
    Song source;
    Syncer playdata;
public:
    package();
    package(const QByteArray& array);
    ~package();
    /**
     * @brief getSong
     * @return Song
     */
    const Song& getSong() const;
    /**
     * @brief getPlayTime
     * @return time of playning media data
     */
    Syncer getPlayData() const;
    unsigned char getType() const;
    bool isValid() const;
    QByteArray parseTo() const;
    bool parseFrom(const QByteArray& array);
};

class Node:public QTcpServer{
    Q_OBJECT
protected:
    QList<QTcpSocket*> clients;
private slots:
    void acceptError_(QTcpSocket*);
    void newConnection_();
    void readData(QTcpSocket*_client);
public:
    Node();
    void WriteAll(const QByteArray&);
    void disconnectClient(QTcpSocket*);
    QList<QTcpSocket*>* getClients();
    bool addNode(const QString &node, int port = DEDAULT_PORT);
    bool addNode(QTcpSocket* node);
    ~Node();
signals:
    void Error(QString);
    void Message(QTcpSocket*);
    void ClientDisconnected(QTcpSocket*);
    void ClientConnected(QTcpSocket*);
};

}


#endif // NODE_H
