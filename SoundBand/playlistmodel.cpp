#include "playlistmodel.h"

PlayListModel::PlayListModel(QObject *parent) :
    QAbstractListModel(parent),
    syncEngine(nullptr)
{
    itemCount = 0;
    playListName = ALL_SONGS_LIST;
}

void PlayListModel::setSource(SyncEngine *engine){
    if(syncEngine)
        disconnect(syncEngine, SIGNAL(songsCountChanged()) ,this, SLOT(onPlayListChanged()));
    syncEngine = engine;
    connect(syncEngine, SIGNAL(songsCountChanged()),this ,SLOT(onPlayListChanged()));
    onPlayListChanged();
}

QHash<int, QByteArray> PlayListModel::roleNames()const{
    QHash<int, QByteArray> roles;
    roles[nameRole] = "songName";
    roles[idRole] = "songId";
    return roles;
}

void PlayListModel::setNewList(const QString &name){
    playListName = name;
    onPlayListChanged();
}

void PlayListModel::onPlayListChanged(){
    beginResetModel();
    syncEngine->getPlayList(playList, playListName);
    endResetModel();
}

bool PlayListModel::canFetchMore(const QModelIndex & /* index */) const
{
    if (itemCount != playList.size())
        return true;
    else
        return false;
}

void PlayListModel::fetchMore(const QModelIndex & /* index */)
{
    int remainder = playList.size() - itemCount;
    int itemsToFetch = qMin(100, remainder);

    if(itemsToFetch < 0){
        beginRemoveRows(QModelIndex(), 0, 0 - itemsToFetch - 1 );

        itemCount += itemsToFetch;

        endRemoveRows();
    }else{
        beginInsertRows(QModelIndex(), itemCount, itemCount + itemsToFetch - 1);

        itemCount += itemsToFetch;

        endInsertRows();
    }

}

int PlayListModel::rowCount(const QModelIndex & /* parent */) const
{
    return itemCount;
}

QVariant PlayListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= playList.size() || index.row() < 0)
        return QVariant();

    switch (role) {
    case nameRole:
        return playList[index.row()].name;
        break;
    case idRole:
        return playList[index.row()].id;
        break;
    default:
        break;
    }

    return QVariant();
}

bool PlayListModel::select(int id){

    for(QList<SongStorage>::Iterator i = playList.begin(); i < playList.end(); i++){
        if(i->id == id){

            if((i->isSelected = !i->isSelected)){
                syncEngine->addToPlayList(id, playListName);
            }
            else{
                syncEngine->removeFromPlayList(id, playListName);
            }

            return i->isSelected;
        }
    }

    return false;
}

QList<int> PlayListModel::getSelected(){
    QList<int> result;

    for(QList<SongStorage>::Iterator i = playList.begin(); i < playList.end(); i++){
        if(i->isSelected){
            result.push_back(i->id);
        }
    }
    return result;
}

bool PlayListModel::isSelected(int id){

    for(QList<SongStorage>::Iterator i = playList.begin(); i < playList.end(); i++){
        if(i->id == id){
            return i->isSelected;
        }
    }

    return false;
}

