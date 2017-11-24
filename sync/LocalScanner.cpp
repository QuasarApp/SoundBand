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

QHostAddress LocalScanner::thisAdress(){
    QList<QHostAddress> adress= QNetworkInterface::allAddresses();
    for(QHostAddress &ip:adress)
        if(ip.protocol() == QAbstractSocket::IPv4Protocol && ip != QHostAddress(QHostAddress::LocalHost))
            return ip;
    return QHostAddress::LocalHost;
}

void LocalScanner::scane(){
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
                ETcpSocket *temp=new ETcpSocket;
                connect(temp,SIGNAL(Connected(ETcpSocket*)),SLOT(connected(ETcpSocket*)));
                temp->getSource()->connectToHost(adr,DEDAULT_PORT);
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
    results.push_back(c);
}

LocalScanner::~LocalScanner(){
    for(ETcpSocket* i: socets)
        delete i;
    socets.clear();

}
