#include "currentplaylistmodel.h"

using namespace syncLib;

CurrentPlayListModel::CurrentPlayListModel(QObject *parent) :
    QAbstractListModel(parent),
    syncEngine(nullptr),
    playList(nullptr)
{
    itemCount = 0;
}

void CurrentPlayListModel::setSource(SyncEngine *engine){
    if(syncEngine){
        disconnect(syncEngine, SIGNAL(currentPlayListCountChanged()) , this, SLOT(onPlayListChanged()));
        disconnect(syncEngine, SIGNAL(currentSongChanged()), this, SIGNAL(currentSongIdChanged()));
    }

    syncEngine = engine;
    connect(syncEngine, SIGNAL(currentPlayListCountChanged()), this ,SLOT(onPlayListChanged()));
    connect(syncEngine, SIGNAL(currentSongChanged()), this ,SIGNAL(currentSongIdChanged()));
}

QHash<int, QByteArray> CurrentPlayListModel::roleNames()const{
    QHash<int, QByteArray> roles;
    roles[nameRole] = "songName";
    roles[idRole] = "songId";
    return roles;
}

void CurrentPlayListModel::onPlayListChanged(){
    beginResetModel();
    playList = syncEngine->currentPlayList();
    endResetModel();
}

bool CurrentPlayListModel::canFetchMore(const QModelIndex & /* index */) const
{
    if (playList && itemCount < playList->size())
        return true;
    else
        return false;
}

void CurrentPlayListModel::fetchMore(const QModelIndex & /* index */)
{
    int remainder = playList->size() - itemCount;
    int itemsToFetch = qMin(100, remainder);

    beginInsertRows(QModelIndex(), itemCount, itemCount + itemsToFetch - 1);

    itemCount += itemsToFetch;

    endInsertRows();
}

int CurrentPlayListModel::rowCount(const QModelIndex & /* parent */) const
{
    return itemCount;
}

QVariant CurrentPlayListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= playList->size() || index.row() < 0)
        return QVariant();

    switch (role) {
    case nameRole:
        return playList->at(index.row()).name;
        break;
    case idRole:
        return playList->at(index.row()).id;
        break;
    default:
        break;
    }

    return QVariant();
}

int CurrentPlayListModel::currentSongId(){
    return syncEngine->currentSongId();
}

QString CurrentPlayListModel::currentSongName(){
    return syncEngine->currentSongName();
}
