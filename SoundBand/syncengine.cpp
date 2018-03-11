#include "syncengine.h"
#include <QPicture>
#include "../sync/exaptions.h"

SyncEngine::SyncEngine()
{
    sync = new syncLib::Sync();
    sqlApi = sync->getSqlApi();

    connect(sync, SIGNAL(networkStateChange()), this, SIGNAL(serversCountChanged()));
    connect(sync, SIGNAL(currentPlayListChanged()), this, SIGNAL(currentPlayListCountChanged()));
    connect(sync, SIGNAL(seekChanged(qint64)), this, SLOT(seekChanged(qint64)));
    connect(sync, SIGNAL(currentSongChanged()), this, SIGNAL(currentSongChanged()));

}

void SyncEngine::seekChanged(qint64){
    emit posChanged();
}

QString SyncEngine::currentSongName()const{
    auto song = sync->getCurrentSong();

    if(!song)
        return tr("none");

    return song->name;
}

int SyncEngine::currentSongId()const{
    auto song = sync->getCurrentSong();

    if(!song)
        return -1;

    return song->id;
}

bool SyncEngine::init(){
    sync->updatePlayList(settings.value(CURRENT_PLAYLIST_KEY, ALL_SONGS_LIST).toString());
    emit currentPlayListNameChanged();
    return true;
}

const QList<syncLib::SongHeader>* SyncEngine::currentPlayList() const{

    return sync->getPlayList();
}

const QString& SyncEngine::currentPlayListName() const{
    return sync->getPlayListName();
}

void SyncEngine::allPlayLists(QStringList &playList)const{
    sqlApi->getPlayLists(playList);
}

bool SyncEngine::songImageById(int id , QPixmap & image) {

    _lastError = tr("This option not supported.");
    emit error();

    return false;
}

bool SyncEngine::songImageByName(const QString& name, QPixmap &image) {

    _lastError = tr("This option not supported.");
    emit error();

    return false;
}

bool SyncEngine::play(int id){
    try{
        return sync->play(id);
    }catch(BaseException e){

        _lastError = e.what();
        emit error();
        return false;
    }
}

bool SyncEngine::pause(bool state){

    try{
        return sync->pause(state);
    }catch(BaseException e){

        _lastError = e.what();
        emit error();
        return false;
    }
}

bool SyncEngine::next(){
    return sync->next();
}

bool SyncEngine::prev(){
    return sync->prev();
}

bool SyncEngine::listen(int index){

    const QList<ETcpSocket*> &servers = sync->getServersList();

    if(servers.size() <= index){
        return false;
    }

    try{
        return sync->listen( servers[index]);
    }catch(BaseException e){

        _lastError = e.what();
        emit error();
        return false;
    }
}

const QList<ETcpSocket*>* SyncEngine::getServerList() const{
    return &sync->getServersList();

}

void SyncEngine::scan(){
    try{
        sync->scan();
    }catch(BaseException e){

        _lastError = e.what();
        emit error();
    }
}

int SyncEngine::repeat()const{
    return sync->repeat();
}

void SyncEngine::setRepeat(int flag){
    sync->setRepeat((syncLib::Repeat)flag);
}

bool SyncEngine::setPlayList(const QString& name){

    if(!sync->updatePlayList(name)){
        _lastError = tr("play list not found!");
        emit error();
        return false;
    }
    settings.setValue(CURRENT_PLAYLIST_KEY, name);

    emit currentPlayListNameChanged();
    return true;

}

bool SyncEngine::getPlayList(QList<syncLib::SongHeader> &playList, const QString &name){
    return sqlApi->updateAvailableSongs(playList, name, true);
}

const QString& SyncEngine::lastError() const{
    return _lastError;
}

void SyncEngine::setPos(double newPos){
    sync->jump(sync->getEndPoint() * newPos);
}

void SyncEngine::setValume(int valume){
    sync->setValume(valume);
}

int SyncEngine::getValume()const{
    return sync->getValume();
}

double SyncEngine::pos()const{

    if(!sync->seek())
        return 0.0;

    return (double)sync->seek() / sync->getEndPoint();
}

bool SyncEngine::addSong(const QString &songUrl){
    if(sqlApi->save(songUrl) < 0)
        return false;

    emit songsCountChanged();
    return true;
}

bool SyncEngine::removeSong(int id){
    syncLib::SongHeader header;
    header.id = id;
    if(!sqlApi->removeSong(header))
        return false;

    emit songsCountChanged();
    return true;
}

bool SyncEngine::createPlayList(const QString &name){
    if(!sqlApi->addPlayList(name))
        return false;

    emit playListsCountChanged();
    return true;
}

bool SyncEngine::removePlayList(const QString &name){
    if(!sqlApi->removePlayList(name))
        return false;

    emit playListsCountChanged();
    return true;
}

bool SyncEngine::addToPlayList(int id, const QString &playList){

    syncLib::SongHeader header;
    header.id = id;

    if(!sqlApi->addToPlayList(header, playList)){
        return false;
    }

    if(playList == sync->getPlayListName())
        emit currentPlayListCountChanged();

    return true;

}

bool SyncEngine::removeFromPlayList(int id, const QString &playList){

    syncLib::SongHeader header;
    header.id = id;

    if(!sqlApi->removeFromPlayList(header, playList)){
        return false;
    }

    if(playList == sync->getPlayListName())
        emit currentPlayListCountChanged();

    return true;

}

SyncEngine::~SyncEngine(){
    disconnect(sync, SIGNAL(networkStateChange()), this, SIGNAL(serversCountChanged()));
    delete sync;
}


