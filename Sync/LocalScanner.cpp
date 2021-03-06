#include "LocalScanner.h"
#include "config.h"

LocalScanner::LocalScanner():
    QObject()
{
   wiat.setInterval(1000);
   connect(&wiat,SIGNAL(timeout()),SLOT(scaned_()));
}

void LocalScanner::clear(){
    results.clear();
}

void LocalScanner::setInterval(int msec){
    wiat.setInterval(msec);
}

void LocalScanner::clearSocets(){
    for(ETcpSocket* i:socets)
        delete i;
    socets.clear();
}

QHostAddress LocalScanner::thisAddress(){
//code was taken from https://stackoverflow.com/questions/13835989/get-local-ip-address-in-qt
    foreach (const QNetworkInterface &netInterface, QNetworkInterface::allInterfaces()) {
        QNetworkInterface::InterfaceFlags flags = netInterface.flags();
        if( (bool)(flags & QNetworkInterface::IsRunning) && !(bool)(flags & QNetworkInterface::IsLoopBack)){
            foreach (const QNetworkAddressEntry &address, netInterface.addressEntries()) {
                if(address.ip().protocol() == QAbstractSocket::IPv4Protocol)
                    return address.ip();
            }
        }
    }

    return QHostAddress::LocalHost;
}

void LocalScanner::scane(int port){
    if(!socets.empty())
        return ;
    QList<QHostAddress> adress= QNetworkInterface::allAddresses();
    clear();
    wiat.start();
    for(QHostAddress &ip:adress){
        if(ip.protocol() == QAbstractSocket::IPv4Protocol && ip != QHostAddress(QHostAddress::LocalHost)){
            for(int i=0;i<256;i++){
                QString adr= ip.toString();
                adr=adr.left(adr.lastIndexOf("."))+"."+QString::number(i);
                if(adr == ip.toString())
                    continue;
                ETcpSocket *temp=new ETcpSocket;
                connect(temp,SIGNAL(Connected(ETcpSocket*)),SLOT(connected(ETcpSocket*)));
                temp->getSource()->connectToHost(adr, port);
                socets.push_back(temp);
            }
        }
    }
}

void LocalScanner::scaned_(){
    wiat.stop();
    emit scaned(&results);
    clearSocets();
}

void LocalScanner::connected(ETcpSocket *c){
    socets.removeOne(c);
    results.push_back(c);
}

LocalScanner::~LocalScanner(){
    for(ETcpSocket* i: socets)
        delete i;
    socets.clear();

}
