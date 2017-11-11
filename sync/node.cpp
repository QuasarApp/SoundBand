#include "node.h"
#include <QTcpSocket>
#include "song.h"
#include <QDataStream>

#include "config.h"

syncLib::package::package(){
    type = syncLib::package::t_void;
    source.clear();
    playdata.run = 0;
    playdata.seek = 0;
    size = 0;
}
syncLib::package::package(const QByteArray &array):
    syncLib::package::package(){
    parseFrom(array);
}
const Song& syncLib::package::getSong(){
    return source;
}

Syncer syncLib::package::getPlayData(){
    return playdata;
}

syncLib::package::TypePackage syncLib::package::getType(){
    return type;
}

bool syncLib::package::isValid(){
    switch (type) {
    case package::t_void:
        return false;
    case package::t_close:
        return true;
    case package::t_sync:
        return Syncer.run > 0 && Syncer.seek > 0;
    case package::t_song:
        return source.size > 0;
    case package::t_stop:
        return true;
    default:
        return false;
    }
}

QByteArray syncLib::package::parseTo(){
    QByteArray temp;
    QDataStream stream(&temp);
    temp.clear();
    if(isValid()){
        switch (type) {
        case package::t_void:
            break;
        case package::t_close:
            stream << int();
            stream << (unsigned char)(type);
            stream.device()->seek(0);
            stream << temp.size();
            break;
        case package::t_sync:
            stream << int();
            stream << (unsigned char)(type);
            stream << playdata.run;
            stream << playdata.seek;
            stream.device()->seek(0);
            stream << temp.size();
            break;
        case package::t_song:
            stream << int();
            stream << (unsigned char)(type);
            stream << source;
            stream.device()->seek(0);
            stream << temp.size();
            break;
        case package::t_stop:
            stream << int();
            stream << (unsigned char)(type);
            stream.device()->seek(0);
            stream << temp.size();
            break;
        default:
            break;
        }
    return temp;
    }
}

bool syncLib::package::parseFrom(const QByteArray &array){
    type = t_void;
    QDataStream stream(&array);
    switch (type) {
    case package::t_void:
        return false;
    case package::t_close:
        return true;
    case package::t_sync:
        stream >> playdata.run;
        stream >> playdata.seek;
        return isValid();
    case package::t_song:
        stream >> source;
        return isValid();
    case package::t_stop:
        return true;
    default:
        return isValid();
    }
}

syncLib::Node::Node():QTcpServer(){
    connect(this,SIGNAL(acceptError(QAbstractSocket::SocketError)),SLOT(acceptError_(QAbstractSocket::SocketError)));
    connect(this,SIGNAL(newConnection()),SLOT(newConnection_()));
}
void syncLib::Node::acceptError_(QTcpSocket*c){
    c->close();
    clients.removeOne(c);
    emit ClientDisconnected(c);
    delete c;
}
QList<QTcpSocket*>* syncLib::Node::getClients(){
    return &clients;
}
void syncLib::Node::newConnection_(){
    QTcpSocket *newClient=new QTcpSocket(nextPendingConnection());
    clients.push_back(newClient);
    connect(newClient,SIGNAL(Disconnected(ETcpSocket*)),this,SLOT(acceptError_(QTcpSocket*)));
    connect(newClient,SIGNAL(Message(ETcpSocket*)),this,SLOT(readData(QTcpSocket*)));
    emit ClientConnected(newClient);
}
void syncLib::Node::readData(QTcpSocket *c){
    emit Message(c);
}
void syncLib::Node::WriteAll(const QByteArray &data){
    for(QTcpSocket*i:clients){
        i->write(data);
    }
}
void syncLib::Node::disconnectClient(QTcpSocket *c){
    c->close();
    clients.removeOne(c);
    delete c;
}

bool syncLib::Node::addNode(const QString &node,int port){

    QTcpSocket *temp = new QTcpSocket;
    if(temp->bind(node,port) && temp->open(QIODevice::ReadWrite)){
        clients.append(temp);
        return true;
    }
    return false;
}

bool syncLib::Node::addNode(QTcpSocket *node){
    if(node->isOpen()){
        clients.append(node);
        return true;
    }
    return false;
}

syncLib::Node::~Node(){
    for(QTcpSocket *i:clients){
        i->abort();
        delete i;
    }
    this->close();
}
