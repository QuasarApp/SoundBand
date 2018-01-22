#include "syncengine.h"
#include <QPicture>
#include "../sync/exaptions.h"

SyncEngine::SyncEngine()
{
    sync = new syncLib::Sync();
    sqlApi = sync->getSqlApi();
}

const QString& SyncEngine::curentSong()const{
    return sync->getCurentSong()->name;
}

const QStringList& SyncEngine::curentPlayList(){

    QStringList &result = tempList;
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

const QStringList& SyncEngine::allPlayLists(){
    QStringList &result = tempList;
    result.clear();

    sqlApi->getPlayLists(result);

    return result;
}

QPixmap SyncEngine::curentSongImage() const{

    throw NotSupported();

    return QPixmap(1, 1);
}

QPixmap SyncEngine::songImageById(int ) const{

    throw NotSupported();

    return QPixmap(1, 1);
}

bool SyncEngine::play(){
    return sync->play(sqlApi->getSongId(_curentPlayListName));
}

bool SyncEngine::pause(bool state){
    return sync->pause(state);
}

bool SyncEngine::next(){

    throw NotSupported();

    return false;
}

bool SyncEngine::prev(){

    throw NotSupported();

    return false;
}

bool SyncEngine::listen(int index){

    const QList<ETcpSocket*> &servers = sync->getServersList();

    if(servers.size() <= index){
        return false;
    }

    return sync->listen( servers[index]);
}

const QStringList& SyncEngine::getServerList(){
    const QList<ETcpSocket*>& list = sync->getServersList();

    tempList.clear();

    for(ETcpSocket* socket : list){
        tempList.push_back(socket->peerName());
    }

    return tempList;

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
    delete sync;
}


