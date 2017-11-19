#ifndef NODE_H
#define NODE_H
#include <QTcpServer>
#include "ETcpSocket.h"
#include <song.h>
class Syncer;
namespace syncLib {

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
     t_song_h = 0x4,
     t_song = 0x8,
     t_stop = 0x10
};

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

private:
    TypePackage type;
    Song source;
    SongHeader header;
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
    TypePackage getType() const;
    bool isValid() const;
    void clear();
    QByteArray parseTo();
    bool parseFrom(const QByteArray& array);
    friend class Sync;
};

class Node:public QTcpServer{
    Q_OBJECT
protected:
    QList<ETcpSocket*> clients;
private slots:
    void acceptError_(ETcpSocket*);
    void newConnection_();
    void readData(ETcpSocket*_client);
public:
    Node();
    void WriteAll(const QByteArray&);
    void disconnectClient(ETcpSocket*);
    QList<ETcpSocket*>* getClients();
    bool addNode(const QString &node, int port = DEDAULT_PORT);
    bool addNode(ETcpSocket* node);
    ~Node();
signals:
    void Error(QString);
    void Message(const package&,ETcpSocket*);
    void ClientDisconnected(ETcpSocket*);
    void ClientConnected(ETcpSocket*);
};

}


#endif // NODE_H
