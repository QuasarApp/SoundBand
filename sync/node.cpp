#include "node.h"
#include "exaptions.h"
#include "LocalScanner.h"

namespace syncLib{

package::package()
{
    clear();
}

package::package( QByteArray &array):
    package::package(){
    parseFrom(array);
}

const SongHeader& package::getHeader() const{
    return header;
}

const Song& package::getSong() const{
    return source;
}

const Syncer& package::getPlayData() const{
    return playdata;
}

const Type& package::getType() const{
    return type;
}

bool package::isValid() const{

    bool ret = true;
    if(type == TypePackage::t_void){
        return false;

    }

    if(type & TypePackage::t_sync && type & t_brodcaster){
        ret = ret && (playdata.seek > 0);

    }

    if(type & TypePackage::t_song_h && type & t_brodcaster){
        ret = ret && header.size > 0;

    }

    if(type & TypePackage::t_song && type & t_brodcaster){
        ret = ret && source.size > 0;

    }

    return ret;

}

void package::clear(){
    type = TypePackage::t_void;
    source.clear();
    playdata.seek = 0;
}

QByteArray package::parseTo(){
    QByteArray temp;
    QDataStream stream(&temp, QIODevice::WriteOnly);
    temp.clear();
    if(isValid()){
        stream <<  static_cast<unsigned char>(type);

        if(type & TypePackage::t_sync && type & t_brodcaster){
            stream << playdata.seek;

        }

        if(type & TypePackage::t_song_h && type & t_brodcaster){
            stream << header;

        }

        if(type & TypePackage::t_song && type & t_brodcaster){
            stream << source;

        }

    }
    return temp;
}

bool package::parseFrom(QByteArray &array){
    type = TypePackage::t_void;
    QDataStream stream(&array, QIODevice::ReadOnly);

    unsigned char temp_type;
    stream >> temp_type;
    type = static_cast<TypePackage> (temp_type);

    if(type & TypePackage::t_sync){
        stream >> playdata.seek;

    }

    if(type & TypePackage::t_song_h){
        stream >> header;

    }

    if(type & TypePackage::t_song){
        stream >> source;

    }

    return isValid();
}

package::~package(){}

Node::Node(const QString &addres, int port):QTcpServer(){
    QString address = addres;
    if(address == DEFAULT_ADRESS){
            address = LocalScanner::thisAddress().toString();
    }
    if(!listen(QHostAddress(address), port)){
#ifdef QT_DEBUG
        qDebug() << errorString();
#endif
        throw initNodeError();
        return ;
    }
#ifdef QT_DEBUG
    qDebug() << "node started on:" << serverAddress().toString() << "port:" << serverPort();
#endif
    connect(this,SIGNAL(newConnection()),SLOT(newConnection_()));
}

void Node::acceptError_(ETcpSocket*c){
    c->getSource()->close();
    clients.removeOne(c);
#ifdef QT_DEBUG
    qDebug() << "node diskonected error:" <<c->getSource()->errorString() << " node:" << c->peerName();
#endif
    emit ClientDisconnected(c);
    delete c;
}

QList<ETcpSocket*>* Node::getClients(){
    return &clients;
}

void Node::newConnection_(){
    ETcpSocket *newClient=new ETcpSocket(nextPendingConnection());
    clients.push_back(newClient);
    connect(newClient,SIGNAL(Disconnected(ETcpSocket*)),this,SLOT(acceptError_(ETcpSocket*)));
    connect(newClient,SIGNAL(Message(ETcpSocket*)),this,SLOT(readData(ETcpSocket*)));
    emit ClientConnected(newClient);
}

void Node::readData(ETcpSocket *c){
    emit Message(c);
}

void Node::WriteAll(const QByteArray &data){
    for(ETcpSocket*i:clients){
        i->Write(data);
#ifdef QT_DEBUG
        qDebug() << i->peerName();
#endif
    }
}

void Node::disconnectClient(ETcpSocket *c){
    c->getSource()->close();
    clients.removeOne(c);
    delete c;
}

bool Node::addNode(const QString &node,int port){
    ETcpSocket *temp;

    try{
        temp = new ETcpSocket(node,port);
    }catch(AddNodeExaption  &e){
#ifdef QT_DEBUG
        qDebug() << e.what();
#endif
        return false;
    }

    clients.push_back(temp);
    return true;

}

bool Node::addNode(ETcpSocket *node){
    if(node->getSource()->isOpen()){
        connect(node,SIGNAL(Disconnected(ETcpSocket*)),this,SLOT(acceptError_(ETcpSocket*)));
        connect(node,SIGNAL(Message(ETcpSocket*)),this,SLOT(readData(ETcpSocket*)));
        clients.append(node);
        return true;
    }
    return false;
}

Node::~Node(){
    for(ETcpSocket *i:clients){
        i->getSource()->close();
        delete i;
    }
    this->close();
}

}


