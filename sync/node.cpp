#include "node.h"
#include "exaptions.h"

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

    if(type & TypePackage::t_play){
        ret = ret && true;

    }

    if(type & TypePackage::t_sync){
        ret = ret && (playdata.run > 0 && playdata.seek > 0);

    }

    if(type & TypePackage::t_song_h){
        ret = ret && header.size > 0;

    }

    if(type & TypePackage::t_song){
        ret = ret && source.size > 0;

    }

    if(type & TypePackage::t_close){
        ret = ret && true;

    }

    if(type & TypePackage::t_stop){
        ret = ret && true;

    }

    return ret;

}

void package::clear(){
    type = TypePackage::t_void;
    source.clear();
    playdata.run = 0;
    playdata.seek = 0;
}

QByteArray package::parseTo(){
    QByteArray temp;
    QDataStream stream(&temp, QIODevice::WriteOnly);
    temp.clear();
    if(isValid()){
        stream <<  static_cast<unsigned char>(type);

        if(type & TypePackage::t_sync){
            stream << playdata.run;
            stream << playdata.seek;

        }

        if(type & TypePackage::t_song_h){
            stream << header;

        }

        if(type & TypePackage::t_song){
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
        stream >> playdata.run;
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

Node::Node():QTcpServer(){

    connect(this,SIGNAL(newConnection()),SLOT(newConnection_()));
}

void Node::acceptError_(ETcpSocket*c){
    c->getSource()->close();
    clients.removeOne(c);
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
        i->getSource()->write(data);
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


