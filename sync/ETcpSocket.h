#ifndef CLIENT_H
#define CLIENT_H
#include <QTcpSocket>
#include <QTcpServer>
#include <QList>
#include <QDataStream>
class ETcpSocket:public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name)
private:
    QTcpSocket *source;
    QByteArray *array;
    qint32 size;
    QList<QByteArray*> ReadyStack;
    void init();

private slots:
    void connected_();
    void disconnected_();
    void error_(QAbstractSocket::SocketError socketError);
    void hostFound_();
    void readReady_();
    void proxyAuthenticationRequired_(const QNetworkProxy &proxy, QAuthenticator *authenticator);
    void stateChanged_(QAbstractSocket::SocketState socketState);
public:
    explicit ETcpSocket();
    explicit ETcpSocket(QTcpSocket*);
    explicit ETcpSocket(const QString& addres,int port);
    QTcpSocket* getSource()const;
    QByteArray* topStack();
    void nextItem();
    int sizeDescriptPackege();
    bool Write(const QByteArray&);
    ~ETcpSocket();
public slots:
    QString name()const;
    QString toStringTcp();
signals:
    void donwload(int val,int max);
    void ReadyComplit(ETcpSocket*,QDataStream&);
    void Connected(ETcpSocket*);
    void Message(ETcpSocket*);
    void Disconnected(ETcpSocket*);
    void Error(ETcpSocket*,QAbstractSocket::SocketError socketError);
    void HostFound(ETcpSocket*);
    void ProxyAuthenticationRequired(ETcpSocket*,const QNetworkProxy &proxy, QAuthenticator *authenticator);
    void StateChanged(ETcpSocket*,QAbstractSocket::SocketState socketState);
    //void Connected(QTcpSocket*);
    //void errorConnect(QTcpSocket*,QAbstractSocket::SocketError);
};

#endif // CLIENT_H
