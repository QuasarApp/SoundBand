#include "syncengine.h"
#include <QPicture>
#include "../sync/exaptions.h"

SyncEngine::SyncEngine()
{
    sync = new syncLib::Sync();
    sqlApi = sync->getSqlApi();

    connect(sync, SIGNAL(networkStateChange()), this, SIGNAL(serversCountChanged()));
}

int SyncEngine::curentSongIndex()const{
    return sync->getCurentSongIndex();
}

const QString& SyncEngine::curentSong()const{
    return sync->getCurentSong()->name;
}

QStringList SyncEngine::getPlayList(const QString &list){
    QStringList result;
    result.clear();

    if(!sqlApi->updateAvailableSongs(result, list)){
        _lastError = tr("play list not found!");
        emit error();
        return result;
    }

    return result;
}

QStringList SyncEngine::curentPlayList(){

    QStringList result;
    result.clear();

    const QList<syncLib::SongHeader> *list = sync->getPlayList();

    for(syncLib::SongHeader song : *list){
        result.push_back(song.name);
    }

    return result;
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

bool SyncEngine::play(){
    try{
        return sync->play(sqlApi->getSongId(_curentPlayListName));
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

    _lastError = tr("This option not supported.");
    emit error();

    return false;
}

bool SyncEngine::prev(){

    _lastError = tr("This option not supported.");
    emit error();

    return false;
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

    return sync->updatePlayList(name);
}

const QString& SyncEngine::lastError() const{
    return _lastError;
}

double SyncEngine::pos()const{

    if(!sync->seek())
        return 0.0;

    return sync->getEndPoint() / (double)sync->seek();
}

SyncEngine::~SyncEngine(){
    disconnect(sync, SIGNAL(networkStateChange()), this, SIGNAL(serversCountChanged()));
    delete sync;
}


