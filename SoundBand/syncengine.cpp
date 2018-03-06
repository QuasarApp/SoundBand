#include "syncengine.h"
#include <QPicture>
#include "../sync/exaptions.h"

SyncEngine::SyncEngine()
{
    sync = new syncLib::Sync();
    sqlApi = sync->getSqlApi();

    sync->updatePlayList(settings.value(CURRENT_PLAYLIST_KEY, ALL_SONGS_LIST).toString());

    connect(sync, SIGNAL(networkStateChange()), this, SIGNAL(serversCountChanged()));
}

int SyncEngine::curentSongIndex()const{
    return sync->getCurentSongIndex();
}

const QString& SyncEngine::curentSong()const{
    return sync->getCurentSong()->name;
}

bool SyncEngine::selectPlayList(const QString &list){

    if(!sync->updatePlayList(list)){
        _lastError = tr("play list not found!");
        emit error();
        return false;
    }

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

    if(!sync->updatePlayList(name)){
        return false;
    }

    settings.setValue(CURRENT_PLAYLIST_KEY, name);
    return true;
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


