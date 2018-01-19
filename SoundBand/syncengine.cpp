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

const QStringList& SyncEngine::curentPlayList() const{

    QStringList &result = tempList;
    result.clear();

    QList<syncLib::SongHeader> *list = sync->getPlayList();

    for(QString & name : list){
        result.push_back(name);
    }

    return result;
}

const QString& SyncEngine::curentPlayListName() const{
    return _curentPlayListName;
}

const QStringList& SyncEngine::allPlayLists()const{
    QStringList &result = tempList;
    result.clear();

    sqlApi->getPlayLists(result);

    return result;
}

const QPixmap& SyncEngine::curentSongImage() const{

    throw NotSupported();

    return QPixmap(1, 1);
}

const QPixmap& SyncEngine::songImageById(int ) const{

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

SyncEngine::~SyncEngine(){
    delete sync;
}
