#ifndef NODE_H
#define NODE_H
#include <QTcpServer>
class Song;
namespace syncLib {


class package
{
    /*parse map */

/*
 * 1 byle - type
 * 4 byte - size of data of package (it avelable if type is t_sync or t_song)
 * data
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
    unsigned int playTime;
    unsigned int playPoint;
public:
    package();
    package(const QByteArray& array);
    ~package();
    const Song& getSong() const;
    unsigned int getPlayTime() const;
    unsigned int getPlayPoint() const;
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
    EServer();
    void WriteAll(const QByteArray&);
    void disconnectClient(QTcpSocket*);
    QList<QTcpSocket*>* getClients();
    ~EServer();
signals:
    void Error(QString);
    void Message(QTcpSocket*);
    void ClientDisconnected(QTcpSocket*);
    void ClientConnected(QTcpSocket*);
};

}


#endif // NODE_H
