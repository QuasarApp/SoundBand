#include "node.h"
#include "exaptions.h"
#include "LocalScanner.h"


Node::Node(const QString &addres, int port):QTcpServer(){
    QString address = addres;
    fBroadcaster = false;
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

bool Node::isBroadcaster()const{
    return fBroadcaster;
}

void Node::setBroadcaster(bool newValue){
    fBroadcaster = newValue;
}

QList<ETcpSocket*>* Node::getClients(){
    return &clients;
}

void Node::newConnection_(){
    ETcpSocket *newClient=new ETcpSocket(nextPendingConnection());
    clients.push_back(newClient);
    connect(newClient, SIGNAL(Disconnected(ETcpSocket*)),
            this, SLOT(acceptError_(ETcpSocket*)));
    connect(newClient, SIGNAL(Message(ETcpSocket*)), this, SLOT(readData(ETcpSocket*)));
    connect(newClient, SIGNAL(synced()), this, SLOT(synced()));

    emit ClientConnected(newClient);
}

void Node::synced(){
    emit NodeSynced(static_cast<ETcpSocket*>(this->sender()));
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



