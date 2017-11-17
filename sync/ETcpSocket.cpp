#include "ETcpSocket.h"
#include "exaptions.h"

ETcpSocket::ETcpSocket()
{
    source=new QTcpSocket();
    init();
}
ETcpSocket::ETcpSocket(QTcpSocket*ptr)
{
    source=ptr;
    init();
}

ETcpSocket::ETcpSocket(const QString& address, int port){
    source = new QTcpSocket();
    if(!source->bind(QHostAddress(address),port) || !source->open(QIODevice::ReadWrite)){
        throw addNodeExaption();
    }
    init();
}

void ETcpSocket::init(){
    array=new QByteArray;
    connect(source,SIGNAL(connected()),this,SLOT(connected_()));
    connect(source,SIGNAL(disconnected()),this,SLOT(disconnected_()));
    connect(source,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(error_(QAbstractSocket::SocketError)));
    connect(source,SIGNAL(hostFound()),this,SLOT(hostFound_()));
    connect(source,SIGNAL(proxyAuthenticationRequired(const QNetworkProxy &, QAuthenticator *)),this,SLOT(proxyAuthenticationRequired_(const QNetworkProxy &, QAuthenticator *)));
    connect(source,SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(stateChanged_(QAbstractSocket::SocketState)));
    connect(source,SIGNAL(readyRead()),this,SLOT(readReady_()));
}

void ETcpSocket::error_(QAbstractSocket::SocketError i){
    emit Error(this,i);
}

void ETcpSocket::connected_(){
    emit Connected(this);
}

void ETcpSocket::disconnected_(){
    emit Disconnected(this);
}

void ETcpSocket::hostFound_(){
    emit HostFound(this);
}

void ETcpSocket::proxyAuthenticationRequired_(const QNetworkProxy &proxy, QAuthenticator *authenticator){
    emit ProxyAuthenticationRequired(this,proxy,authenticator);
}

void ETcpSocket::stateChanged_(QAbstractSocket::SocketState socketState){
    emit StateChanged(this,socketState);
}

void ETcpSocket::readReady_(){
    bool sizewrite=array->isEmpty();
    //while(source->bytesAvailable())
    array->append(source->readAll());
    QDataStream stream(array,QIODevice::ReadOnly);
    if(sizewrite)
        stream>>size;
#ifdef QT_DEBUG
    qDebug()<<"messae size:"<<size;
    qDebug()<<"message package size:"<<array->size();
#endif
    if(size==array->size())
    {
        array->remove(0,sizeof(qint32));
        ReadyStack.push_back(array);
        array=new QByteArray;
        emit Message(this);
    }else{
        emit donwload(array->size(),size);
    }
   // emit ReadReady(this);
}

QString ETcpSocket::name() const{
    return source->peerAddress().toString();
}

QByteArray* ETcpSocket::topStack(){
    if(ReadyStack.size())
        return ReadyStack.front();
    return NULL;
}

QTcpSocket* ETcpSocket::getSource()const{
    return source;
}

void ETcpSocket::nextItem(){
    if(ReadyStack.size())
        ReadyStack.pop_front();
}

int ETcpSocket::sizeDescriptPackege(){
    return sizeof(qint32);
}

QString ETcpSocket::toStringTcp(){
    return source->peerAddress().toString();
}

bool ETcpSocket::Write(const QByteArray&data){
    if(source->state()==QTcpSocket::ConnectedState){
       QByteArray array;
       QDataStream stream(&array,QIODevice::ReadWrite);
       stream<<qint32(0);
       //stream<<data;
       array.append(data);
       stream.device()->seek(0);
       stream<<qint32(array.size());
#ifdef QT_DEBUG
       qDebug()<<"size :"<<array.size();
       qint64 temp= source->write(array);
       qDebug()<<"size  write:"<<temp<<" size packege:"<<array.size();
       return temp==(array.size());
#else
       return source->write(array)==(array.size());
#endif
   }
    return false;
}

ETcpSocket::~ETcpSocket()
{
    for(QByteArray*i:ReadyStack){
        i->clear();
        delete i;
    }
    disconnect(source,SIGNAL(connected()),this,SLOT(connected_()));
    disconnect(source,SIGNAL(disconnected()),this,SLOT(disconnected_()));
    disconnect(source,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(error_(QAbstractSocket::SocketError)));
    disconnect(source,SIGNAL(hostFound()),this,SLOT(hostFound_()));
    disconnect(source,SIGNAL(proxyAuthenticationRequired(const QNetworkProxy &, QAuthenticator *)),this,SLOT(proxyAuthenticationRequired_(const QNetworkProxy &, QAuthenticator *)));
    disconnect(source,SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(stateChanged_(QAbstractSocket::SocketState)));
    disconnect(source,SIGNAL(readyRead()),this,SLOT(readReady_()));
    source->deleteLater();
}
