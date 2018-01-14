#include "sync.h"
#include <QtSql>
#include <QMultimedia>
#include <QSqlQuery>
#include "exaptions.h"
#include "chronotime.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

namespace syncLib{

Sync::Sync(const QString address, int port, const QString &datadir):
    node(nullptr),
    player(nullptr),
    curentSong(nullptr)
{
    node = new Node(address , this->port = port);

    player = new Player(BUFFER_NAME,nullptr,QMediaPlayer::LowLatency);
    if(!player->isAvailable()){
        throw MediaException();
    }

    fbroadcaster = false;
    resyncCount = 0;
    lastSyncTime = 0;
    ping = 0;

    sql = new MySql(datadir);
    sql->updateAvailableSongs(playList);

    connect(node, SIGNAL(Message(ETcpSocket*)), SLOT(packageRender(ETcpSocket*)));
    connect(&deepScaner, SIGNAL(scaned(QList<ETcpSocket*>*)), SLOT(deepScaned(QList<ETcpSocket*>*)));
    connect(player, SIGNAL(positionChanged(qint64)), SIGNAL(seekChanged(qint64)));
    connect(player, SIGNAL(stateChanged(QMediaPlayer::State)), SLOT(endPlay(QMediaPlayer::State)));
}

bool Sync::findHeader(const Song &song){

    for(SongHeader & header: playList){
        if(header == static_cast<SongHeader>(song)){
            curentSong = &header;
            return true;
        }
    }

    return false;
}

bool Sync::play(const SongHeader &header, bool fbroadcast){

    Song song;
    SongHeader newheader = header;
    newheader.id = -1;
    if(!sql->load(newheader, song)){
        return false;
    }

    return Sync::play(song, fbroadcast);
}

bool Sync::play(const Song &song, bool fbroadcast){

    if(!song.isValid()){
        return false;
    }

    if(!player->setMediaFromBytes(song.getSource())){
        return false;
    }

    fbroadcaster = fbroadcast;

    if(!findHeader(song) && sql->save(song) > -1 &&
            sql->updateAvailableSongs(playList) && !findHeader(song)){

        return false;
    }

    if(fbroadcaster){
        player->play();
        sync();
    }else{
        player->syncBegin();
    }

    return true;
}

bool Sync::play(int id_song, bool fbroadcast){

    SongHeader header;
    header.id = id_song;
    Song song;
    sql->load(header, song);

    return Sync::play(song, fbroadcast);
}

bool Sync::play(QString url){
    int id = addNewSong(url);
    if(id < 0){
        return false;
    }

    return Sync::play(id);
}

bool Sync::pause(bool state){

    if(!fbroadcaster)
        return false;

    if(state){
        player->pause();
    }else{
        player->play();
        sync();

    }

    return true;
}

void Sync::stop(){
    player->stop();
}

void Sync::jump(const qint64 seek){
    player->setPosition(seek);
}

bool Sync::isReadyToSync()const{
    return  !fbroadcaster && player->isSeekable()
            && (player->state() == QMediaPlayer::PlayingState);

}

bool Sync::sync(const Syncer &sync, milliseconds ping){
    if(!isReadyToSync()){
        return false;
    }
    player->setPosition(sync.seek + ping);
    player->syncEnd();

    return true;

}

void Sync::sync(){

    if(fbroadcaster)
        QTimer::singleShot(SYNC_TIME, [=]() {

            package pac;
            if(!createPackage(t_sync, pac)){
                throw CreatePackageExaption();
                return;
            }
            node->WriteAll(pac.parseTo());
        });
}

bool Sync::addNode(const QString ip, int port){
     if(!node->addNode(ip, port))
         return false;

     rescan();
     return true;
}

void Sync::scan(){

    rescan(true);
}

const QList<ETcpSocket*>& Sync::getServersList() const{
    return servers;
}

bool Sync::listen(ETcpSocket *server){
    if(!server){
        return false;
    }

    if(!server->getSource()->isOpen() && server->getSource()->open(QIODevice::ReadWrite)){
        return false;
    }
    package pac;

    if(!createPackage(t_play, pac)){
        return false;
    }

    return server->Write(pac.parseTo());
}

bool Sync::createPackage(Type type, package &pac){

    pac.clear();

    pac.type = type;

    if(type & TypePackage::t_sync){
        if(lastSyncTime)
            pac.playdata.seek = player->position();
        else
            lastSyncTime = ChronoTime::now();

    }

    if(type & TypePackage::t_song_h && fbroadcaster){
        if(!curentSong)
            return false;

        pac.header = *curentSong;

    }

    if(type & TypePackage::t_song && fbroadcaster){
        if(!curentSong)
            return false;

        if(!sql->load(*curentSong, pac.source))
            return false;

    }

    if(fbroadcaster)
        pac.type = TypePackage(pac.type | t_brodcaster);

    return pac.isValid();
}

void Sync::packageRender(ETcpSocket *socket){

    QByteArray *array;
    while((array = socket->topStack())){
        package pkg;
        if(!pkg.parseFrom((*array))){
            throw BadAnswerExaption();
            socket->nextItem();
            continue;
        }
//     package answer;

//     scaning servers

        if(pkg.getType() & t_brodcaster && servers.indexOf(socket) == -1){
            servers.append(socket);
            emit networkStateChange();
        }

        if(!(pkg.getType() & t_brodcaster) && servers.indexOf(socket) != -1){
            servers.removeOne(socket);
            emit networkStateChange();
        }

        if(pkg.getType() & t_brodcaster){

//            if requst from server

            // calc ping for sync
            bool fFromRequst = false;
            if(lastSyncTime){
                ping = ChronoTime::now() - lastSyncTime;
                lastSyncTime = 0;
                fFromRequst = true;
            }

            if(pkg.getType() & t_sync &&
                    !sync(pkg.getPlayData(), (fFromRequst)? ping: ping/2)){

                QTimer::singleShot(RESYNC_TIME, [=]() {
                    package pac;

                    if(resyncCount < MAX_RESYNC_COUNT){

                        if(!createPackage(t_sync, pac)){
                            throw CreatePackageExaption();
                            return;
                        }
                        resyncCount++;

                    }else{
                        resyncCount = 0;
                        throw SyncCountError();
                        return;
                    }

                    node->WriteAll(pac.parseTo());
                });

            }
            else if (pkg.getType() & t_sync){
                resyncCount = 0;
            }

            if(pkg.getType() & t_play && !play(pkg.getHeader(), false) && !play(pkg.getSong(), false)){

                Type requestType = t_song_h;

                if(pkg.getType() & t_song_h)
                    requestType = t_song;

                package answer;
                if(!createPackage(requestType | t_play, answer)){
                    throw CreatePackageExaption();
                    socket->nextItem();
                    continue;
                }
                socket->Write(answer.parseTo());
            }
            else if(pkg.getType() & t_play){

                package answer;
                if(!createPackage(t_sync, answer)){
                    throw CreatePackageExaption();
                    socket->nextItem();
                    continue;
                }
                socket->Write(answer.parseTo());
            }



            if(pkg.getType() & t_close){
                socket->getSource()->close();
                node->getClients()->removeOne(socket);
                delete socket;
                return;
            }

            if(pkg.getType() & t_what){
                package answer;
                if(!createPackage(t_void, answer)){
                    throw CreatePackageExaption();
                    socket->nextItem();
                    continue;
                }
                socket->Write(answer.parseTo());
            }

        }else{

//            if requst from client
            if(pkg.getType() & t_play & t_sync){
                if(!curentSong){
                    throw SyncError();
                    socket->nextItem();
                    continue;
                }
            }

            package answer;
            if(!createPackage(pkg.getType() & ~t_what & ~t_stop & ~t_brodcaster, answer)){
                throw CreatePackageExaption();
                socket->nextItem();
                continue;
            }
            socket->Write(answer.parseTo());

            if(pkg.getType() & t_close){
                socket->getSource()->close();
                node->getClients()->removeOne(socket);
                delete socket;
                return;
            }

        }

        socket->nextItem();
    }
}

void Sync::rescan(bool deep){
    package pac;
    if(!createPackage(t_what,pac)){
        throw CreatePackageExaption();
        return;
    }
    node->WriteAll(pac.parseTo());

    if(deep){
       deepScaner.setInterval(DEEP_SCANER_INTERVAL);
       deepScaner.scane(port);
    }
}

void Sync::deepScaned(QList<ETcpSocket *> * list){
    package pac;
    if(!createPackage(t_what, pac)){
        throw CreatePackageExaption();
        return;
    }
    QByteArray array = pac.parseTo();
    for(ETcpSocket * i: *list){
        node->addNode(i);
        i->Write(array);
    }
}

void Sync::endPlay(QMediaPlayer::State state){
    if(state == QMediaPlayer::StoppedState){
        curentSong = nullptr;
        fbroadcaster = false;
    }
}

QString Sync::getVersion(){
    return QString(tr("Version") + "%0.%1.%2").arg(MAJOR_VERSION).arg(MINOR_VERSION).arg(REVISION_VERSION);
}

bool Sync::setValume(unsigned int valume){
    if(valume > 100 || !player->isSynced())
        return false;

    player->setVolume(valume);

    return true;
}

unsigned int Sync::getValume() const{
    return player->volume();
}

unsigned int Sync::seek() const{
    return player->position();
}

const QList<SongHeader>* Sync::getPlayList() const{
    return &playList;
}

const SongHeader* Sync::getCurentSong() const{
    return curentSong;
}

qint64 Sync::getEndPoint() const {
    return player->duration();
}

int Sync::addNewSong(const QString &url){
    int result = sql->save(url);
    sql->updateAvailableSongs(playList);
    return result;
}

Sync::~Sync(){
    delete node;
    delete player;
    delete sql;
    servers.clear();

}

}


