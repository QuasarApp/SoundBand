#include "node.h"
#include <QTcpSocket>
#include "song.h"
#include <QDataStream>
syncLib::package::package(){
    type = syncLib::package::t_void;
    source.clear();
    playTime = 0;
    playPoint = 0;
    size = 0;
}
syncLib::package::package(const QByteArray &array):
    syncLib::package::package(){
    parseFrom(array);
}
const Song& syncLib::package::getSong(){
    return source;
}

unsigned int syncLib::package::getPlayTime(){
    return playTime;
}

unsigned int syncLib::package::getPlayPoint(){
    return playPoint;
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
        return playTime > 0 && playPoint > 0;
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
            stream << playTime;
            stream << playPoint;
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
        stream >> playTime;
        stream >> playPoint;
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
        i->Write(data);
    }
}
void syncLib::Node::disconnectClient(QTcpSocket *c){
    c->getSource()->close();
    clients.removeOne(c);
    delete c;
}

syncLib::Node::~Node(){
    for(QTcpSocket *i:clients){
        i->getSource()->abort();
        delete i;
    }
    this->close();
}
