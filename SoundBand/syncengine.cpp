#include "syncengine.h"

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

}

SyncEngine::~SyncEngine(){
    delete sync;
}
