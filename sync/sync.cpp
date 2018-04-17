#include "sync.h"
#include <QtSql>
#include <QMultimedia>
#include <QSqlQuery>
#include "exaptions.h"
#include "chronotime.h"
#include "cmath"

#ifdef QT_DEBUG
#include <QDebug>
#endif

namespace syncLib{

Sync::Sync(const QString &address, int port, const QString &datadir):
    node(nullptr),
    player(nullptr)
{
    node = new Node(address , this->port = port);

    player = new Player(BUFFER_NAME,nullptr,QMediaPlayer::LowLatency);
    if(!player->isAvailable()){
        throw MediaException();
    }
    playList = new PlayList;

    player->setPlaylist(playList->getList());

    sql = new MySql(datadir);

    connect(node, SIGNAL(Message(ETcpSocket*)), SLOT(packageRender(ETcpSocket*)));
    connect(&deepScaner, SIGNAL(scaned(QList<ETcpSocket*>*)), SLOT(deepScaned(QList<ETcpSocket*>*)));
    connect(player, SIGNAL(positionChanged(qint64)), SIGNAL(seekChanged(qint64)));
    connect(player, SIGNAL(stateChanged(QMediaPlayer::State)), SLOT(endPlay(QMediaPlayer::State)));

}

MySql* Sync::getSqlApi(){
    return sql;
}

bool Sync::setSingle(const SongStorage& media){
    playList->clear();
    if(!playList->addMedia(media)) {
        return false;
    }

    emit currentPlayListChanged();
    return true;
}

bool Sync::updateSongs(PlayList& list, const QString& playList){
    if(!sql->updateAvailableSongs(list, playList)){
        return false;
    }

    if(lastUsedPlayList != playList){
        lastUsedPlayList = playList;
        emit selectedNewPlatList();
    }

    emit currentPlayListChanged();
    return true;
}

const QString& Sync::getPlayListName() const{
    return lastUsedPlayList;
}

bool Sync::play(bool fbroadcast){
    node->setBroadcaster(fbroadcast);

    if(fbroadcast){
        player->play();
        sync();
    }else{
        player->syncBegin();
    }
    emit currentSongChanged();

    return true;
}

bool Sync::play(const SongStorage &song, bool fbroadcast){

    if(!song.isValid()){
        return false;
    }

    if(playList->selectSong(song)){
        return play(fbroadcast);
    }

    playList->clear();
    if(!playList->addMedia(song)){
        return false;
    }

    return play(fbroadcast);
}

bool Sync::play(const SongHeader &header, bool fbroadcast){

    if(!header.isValid()){
        return false;
    }

    if(playList->selectSong(header)){
        return play(fbroadcast);
    }

    SongStorage song;
    SongHeader newheader = header;
    newheader.id = -1;
    if(!sql->load(newheader, song)){
        return false;
    }

    return play(song, fbroadcast);
}

bool Sync::play(const Song &song, bool fbroadcast){

    if(!song.isValid()){
        return false;
    }

    SongStorage savedSong;
    if(!sql->find(static_cast<const SongHeader&>(song), savedSong) && sql->save(song) > -1 &&
            !sql->find((SongHeader&)song, savedSong)){

        return false;
    }

    return play(savedSong, fbroadcast);
}

bool Sync::play(int id_song, bool fbroadcast){

    if(id_song < 0){
        return false;
    }

    SongHeader header;
    SongStorage song;
    header.id = id_song;

    if(!sql->load(header, song)){
        return false;
    }

    return Sync::play(song, fbroadcast);
}

bool Sync::play(QString url){
    int id = addNewSong(url);
    if(id < 0){
        return false;
    }

    return Sync::play(id);
}

QMediaPlaylist::PlaybackMode Sync::repeat()const{
    return playList->getList()->playbackMode();
}

void Sync::setRepeat(QMediaPlaylist::PlaybackMode flag){
    playList->getList()->setPlaybackMode(flag);
}

bool Sync::pause(bool state){

    if(player->state() == QMediaPlayer::StoppedState){
        if(playList->isEmpty())
            return false;

        return play();
    }

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
    sync();

}

bool Sync::isReadyToSync()const{
    return  !node->isBroadcaster() && player->isSeekable()
            && (player->state() == QMediaPlayer::PlayingState);

}

bool Sync::sync(const Syncer &sync){
    milliseconds now =  sync.timeOn - ChronoTime::now();
    if(!isReadyToSync() || now < 0){
        return false;
    }

    QTimer::singleShot(now, [=](){
        player->setPosition(sync.seek);

        player->syncEnd();
    } );

    return true;

}



/**
 * @todo thi nead send a hedaer
*/
void Sync::sync(bool forse){

    if(node->isBroadcaster()) {

        if (forse) {
            package pac;
            if(!createPackage(t_sync, pac)){
                throw CreatePackageExaption();
                return;
            }
            node->WriteAll(pac.parseTo());
        } else {
            QTimer::singleShot(SYNC_TIME, [=]() {

                package pac;
                if(!createPackage(t_sync, pac)){
                    CreatePackageExaption();
                    return;
                }
                node->WriteAll(pac.parseTo());
            });
        }
    }

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

    if(!server->isValid()){
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

bool Sync::createPackage(Type type, package &pac, milliseconds time){

    pac.clear();

    pac.type = type;

    bool isbroadcaster = node->isBroadcaster();

    if(type & TypePackage::t_sync && isbroadcaster){
        pac.playdata.seek = player->position() + SYNC_TIME;
        pac.playdata.timeOn = ChronoTime::now(time) + SYNC_TIME;

    }

    if(type & TypePackage::t_song_h && isbroadcaster ){
        if(playList->getList()->currentIndex() < 0)
            return false;

         pac.header = *playList->currentHeader();
    }

    if(type & TypePackage::t_song && isbroadcaster){
        if(playList->getList()->currentIndex() < 0)
            return false;

        if(!playList->currentSong()->toSong(pac.source))
            return false;

    }

    if(type & TypePackage::t_syncTime){
            pac.time = time;
    }

    if(isbroadcaster)
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

            if(pkg.getType() & t_syncTime){

                package answer;
                if(!createPackage(t_syncTime, answer, ChronoTime::now() - socket->getTime())){
                    throw CreatePackageExaption();
                    socket->nextItem();
                    continue;
                }
                socket->Write(answer.parseTo());
            }

            if(pkg.getType() & t_sync){
                sync(pkg.getPlayData());
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
                servers.removeOne(socket);
                emit networkStateChange();
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

        } else if (node->isBroadcaster()) {
 //            if requst from client


            if(pkg.getType() & t_syncTime){
                package answer;

                if(pkg.time == 0){
                    socket->isSynced = true;
                    if(!createPackage(t_sync, answer, socket->getTime())){
                        throw CreatePackageExaption();
                        socket->nextItem();
                        continue;
                    }
                } else {
                    socket->setTime(pkg.time);
                    if(!createPackage(t_syncTime, answer, ChronoTime::now(socket->getTime()))){
                        throw CreatePackageExaption();
                        socket->nextItem();
                        continue;
                    }
                }

                socket->Write(answer.parseTo());

            }

            if(pkg.getType() & t_sync){
                package answer;

                if(!socket->isSynced) {

                    if(!createPackage(t_syncTime, answer, ChronoTime::now(socket->getTime()))){
                        throw CreatePackageExaption();
                        socket->nextItem();
                        continue;
                    }
                } else {
                    if(!createPackage(t_sync, answer, socket->getTime())){
                        throw CreatePackageExaption();
                        socket->nextItem();
                        continue;
                    }
                }
                socket->Write(answer.parseTo());
            }

            package answer;
            if(createPackage(pkg.getType() & ~t_sync & ~t_what & ~t_close & ~t_brodcaster, answer)){
                socket->Write(answer.parseTo());
            }

            if(pkg.getType() & t_close){
                socket->getSource()->close();
                node->getClients()->removeOne(socket);
                delete socket;
            }

        }

        socket->nextItem();
    }
}

void Sync::rescan(bool deep){
    package pac;
    if(!createPackage(t_what, pac)){
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

    switch (state) {
    case QMediaPlayer::StoppedState:
        node->setBroadcaster(false);
        break;
    case QMediaPlayer::PlayingState:
        sync();
        break;

    case QMediaPlayer::PausedState:
        break;
    default:
        break;
    }

    emit playStateChanged();
}

QString Sync::getVersion(){
    return QString(tr("Version") + "%0.%1.%2").
            arg(MAJOR_VERSION).arg(MINOR_VERSION).arg(REVISION_VERSION);
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

const QList<SongStorage>* Sync::getPlayList() const{
    return playList->getInfo();
}

int Sync::getCurrentSongIndex()const{
    return playList->getList()->currentIndex();
}

const SongStorage* Sync::getCurrentSong() const{
    return playList->currentSong();
}

qint64 Sync::getEndPoint() const {
    return player->duration();
}

int Sync::addNewSong(const QString &url){
    int result = sql->save(url);
    updateSongs(*playList);
    return result;
}

bool Sync::updatePlayList(const QString &_playList){
    if(!updateSongs(*playList, _playList)){
        return false;
    }

    if(!playList->size())
        return false;

    if(node->isBroadcaster()){
        play(true);
    }

    return true;

}

bool Sync::next(){
    if(playList->isEmpty())
        return false;

    playList->next();
    emit currentSongChanged();
    return true;
}

bool Sync::prev(){
    if(playList->isEmpty())
        return false;

    playList->prev();
    emit currentSongChanged();
    return true;
}

QMediaPlayer::State Sync::playState()const{
    return player->state();
}

Sync::~Sync(){
    delete node;
    delete player;
    delete sql;
    delete playList;
    servers.clear();

}

}


