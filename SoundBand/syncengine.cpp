#include "syncengine.h"
#include <QPicture>
#include "../sync/exaptions.h"

SyncEngine::SyncEngine()
{
    sync = new syncLib::Sync();
    sqlApi = sync->getSqlApi();

    connect(sync, SIGNAL(networkStateChange()), this, SIGNAL(serversCountChanged()));
    connect(sync, SIGNAL(curentPlayListChanged()), this, SIGNAL(curentPlayListCountChanged()));
    connect(sync, SIGNAL(seekChanged(qint64)), this, SLOT(seekChanged(qint64)));
    connect(sync, SIGNAL(curentSongChanged()), this, SIGNAL(curentSongChanged()));

}

void SyncEngine::seekChanged(qint64){
    emit posChanged();
}

QString SyncEngine::curentSongName()const{
    auto song = sync->getCurentSong();

    if(!song)
        return tr("none");

    return song->name;
}

int SyncEngine::curentSongId()const{
    auto song = sync->getCurentSong();

    if(!song)
        return -1;

    return song->id;
}

bool SyncEngine::selectPlayList(const QString &list){

    if(!sync->updatePlayList(list)){
        _lastError = tr("play list not found!");
        emit error();
        return false;
    }

    return true;
}

bool SyncEngine::init(){
    sync->updatePlayList(settings.value(CURRENT_PLAYLIST_KEY, ALL_SONGS_LIST).toString());
    return true;
}

const QList<syncLib::SongHeader>* SyncEngine::curentPlayList() const{

    return sync->getPlayList();
}

const QString& SyncEngine::curentPlayListName() const{
    return _curentPlayListName;
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
    return _repeat;
}

void SyncEngine::setRepeat(int flag){
    _repeat = (Repeat)flag;
}

bool SyncEngine::setPlayList(const QString& name){

    if(!sync->updatePlayList(name)){
        return false;
    }

    settings.setValue(CURRENT_PLAYLIST_KEY, name);
    return true;
}

bool SyncEngine::getPlayList(QList<syncLib::SongHeader> &playList, const QString &name){
    return sqlApi->updateAvailableSongs(playList, name);
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

    if(playList == _curentPlayListName)
        emit curentPlayListCountChanged();

    return true;

}

bool SyncEngine::removeFromPlayList(int id, const QString &playList){

    syncLib::SongHeader header;
    header.id = id;

    if(!sqlApi->removeFromPlayList(header, playList)){
        return false;
    }

    if(playList == _curentPlayListName)
        emit curentPlayListCountChanged();

    return true;

}

SyncEngine::~SyncEngine(){
    disconnect(sync, SIGNAL(networkStateChange()), this, SIGNAL(serversCountChanged()));
    delete sync;
}


