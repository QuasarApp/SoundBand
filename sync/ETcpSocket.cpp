#include "ETcpSocket.h"
#include "exaptions.h"
#include <QTimer>

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

ETcpSocket::ETcpSocket(const QString& address, unsigned short port){
    source = new QTcpSocket();
    source->connectToHost(address, port);
    if(!source->waitForConnected(DEEP_SCANER_INTERVAL) || !source->open(QIODevice::ReadWrite)){
        throw AddNodeExaption();
    }
    init();
}

void ETcpSocket::init(){
    array = new QByteArray;
    time = 0;
    fSynced = false;

    connect(source, SIGNAL(connected()), this, SLOT(connected_()));
    connect(source, SIGNAL(disconnected()), this, SLOT(disconnected_()));
    connect(source, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(error_(QAbstractSocket::SocketError)));
    connect(source, SIGNAL(hostFound()), this,SLOT(hostFound_()));
    connect(source,
            SIGNAL(proxyAuthenticationRequired(const QNetworkProxy &, QAuthenticator *)),
            this, SLOT(proxyAuthenticationRequired_(const QNetworkProxy &, QAuthenticator *)));
    connect(source, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this ,SLOT(stateChanged_(QAbstractSocket::SocketState)));
    connect(source, SIGNAL(readyRead()), this, SLOT(readReady_()));
}

bool ETcpSocket::_driverResponse(const SyncPackage& from) {

    if(!from.isValid()){
        return false;
    }

    SyncPackage pac;

    switch (from.type) {
    case t_Header:
        syncList.clear();
        precisionSync = from.getPrecision();
        lastTime = ChronoTime::now();
        pac.sourceBytes = ChronoTime::now();
        pac.nativeTime = from.getTime();
        pac.type = t_Source;
        pac.firstByte = 0;

        _Write(pac.parseTo(), true);

        syncList[0] = pac;


        break;
    case t_Source:
        syncList[from.getIndex()] = from;
        pac.type = t_Responce;
        pac.firstByte = from.getIndex();
        pac.sourceBytes = ChronoTime::now();

        _Write(pac.parseTo(), true);
        break;
    case t_Responce:
        syncList[from.getIndex()].ping = ChronoTime::now() - lastTime;
        lastTime = ChronoTime::now();

        if(syncList.size() >= precisionSync){
            pac.type = t_End;

            auto ping = syncList.first().ping;
            auto index = syncList.begin();
            for (auto i = syncList.begin(); i != syncList.end(); i++) {
                if (i.value().ping < ping) {
                    ping = i.value().ping;
                    index = i;
                }
            }

            pac.firstByte = index->firstByte;
            pac.sourceBytes = index->ping;
            pac.nativeTime = index->nativeTime;

            _Write(pac.parseTo(), true);
            return true;
        }

        pac.type = t_Source;
        pac.firstByte = from.getIndex() + 1;
        pac.sourceBytes = ChronoTime::now();
        pac.nativeTime = from.getTime();

        _Write(pac.parseTo(), true);

        syncList[pac.firstByte] = pac;

        break;
    case t_End: {

        if(syncList.size() <= from.getIndex()){
            return false;
        }

        auto ping = from.getPing();

        if (ping > 2) {
            sync();
            return false;
        }

        time = from.getNative() - syncList[from.getIndex()].getTime() - from.getPing() / 2;
        fSynced = true;
        qDebug() << "syncTime :" << time;

        emit synced();

        break;
    }

    default:
        break;
    }
    return true;
}

void ETcpSocket::_driverStart() {
    syncList.clear();

    SyncPackage pac;

    precisionSync = SYNC_COUNT;
    pac.type = t_Header;
    pac.firstByte = precisionSync;
    pac.sourceBytes = ChronoTime::now();

    _Write(pac.parseTo(), true);
}

void ETcpSocket::_driver(QByteArray *data){
    SyncPackage pac;
    if(!pac.parseFrom(*data)){
        return;
    }
    _driverResponse(pac);
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
    emit ProxyAuthenticationRequired(this, proxy, authenticator);
}

void ETcpSocket::stateChanged_(QAbstractSocket::SocketState socketState){
    emit StateChanged(this,socketState);
}

void ETcpSocket::readReady_(){

    bool sizewrite = array->isEmpty();
    array->append(source->readAll());
    QDataStream stream(array, QIODevice::ReadOnly);
    if(sizewrite)
        stream >> size;

#ifdef QT_DEBUG
    qDebug()<<"messae size:" << size;
    qDebug()<<"message package size:" << array->size();
#endif
    if(size == array->size())
    {
        array->remove(0, sizeof(qint32));
        if(array->back()){
            _driver(array);
            delete array;
            array = new QByteArray();
            return;
        }
        array->remove(array->size() - 1, 1);
        ReadyStack.push_back(array);
        array=new QByteArray();
        emit Message(this);
    }else{

        if(size < array->size()){
            array->clear();
        }

        emit donwload(array->size(), size);
    }
}

QString ETcpSocket::peerName() const{
    return QString("%0:%1").arg(source->peerAddress().toString()).arg(source->peerPort());
}

QString ETcpSocket::localName() const{
    return QString("%0:%1").arg(source->localAddress().toString()).arg(source->localPort());
}

QByteArray* ETcpSocket::topStack(){
    if(ReadyStack.size())
        return ReadyStack.front();
    return nullptr;
}

milliseconds ETcpSocket::getTime()const{
    return time;
}

QTcpSocket* ETcpSocket::getSource()const{
    return source;
}

void ETcpSocket::sync(){
    if(fSynced){
        return;
    }
    _driverStart();

}

bool ETcpSocket::isSynced()const{
    return fSynced;
}



void ETcpSocket::nextItem(bool free){
    if( ReadyStack.size()){
        if(free){
            ReadyStack.front()->clear();
            delete ReadyStack.front();
        }
        ReadyStack.pop_front();
    }
}

int ETcpSocket::sizeDescriptPackege(){
    return sizeof(qint32);
}

QString ETcpSocket::toStringTcp(){
    return source->peerAddress().toString();
}

bool ETcpSocket::Write(const QByteArray &data){
    return _Write(data);
}

bool ETcpSocket::_Write(const QByteArray&data, bool isDriver){
    if(source->state()==QTcpSocket::ConnectedState){
       QByteArray array;
       QDataStream stream(&array, QIODevice::ReadWrite);

       stream << qint32(0);
       array.append(data);
       array.append(qint8(isDriver));
       stream.device()->seek(0);
       stream<<qint32(array.size());

#ifdef QT_DEBUG
       qDebug() << "size :" << array.size();
       qint64 temp = source->write(array);
       qDebug() << "size  write:" << temp << " size packege:" << array.size();
       return temp == (array.size());
#else
       return source->write(array) == (array.size());
#endif
   }
    return false;
}

bool ETcpSocket::isValid(){
    return source->isValid() && source->isOpen();
}

ETcpSocket::~ETcpSocket()
{
    for(QByteArray*i:ReadyStack){
        i->clear();
        delete i;
    }

    syncList.clear();

    disconnect(source,SIGNAL(connected()),this,SLOT(connected_()));
    disconnect(source,SIGNAL(disconnected()),this,SLOT(disconnected_()));
    disconnect(source,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(error_(QAbstractSocket::SocketError)));
    disconnect(source,SIGNAL(hostFound()),this,SLOT(hostFound_()));
    disconnect(source,SIGNAL(proxyAuthenticationRequired(const QNetworkProxy &, QAuthenticator *)),this,SLOT(proxyAuthenticationRequired_(const QNetworkProxy &, QAuthenticator *)));
    disconnect(source,SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(stateChanged_(QAbstractSocket::SocketState)));
    disconnect(source,SIGNAL(readyRead()),this,SLOT(readReady_()));
    source->deleteLater();
}
