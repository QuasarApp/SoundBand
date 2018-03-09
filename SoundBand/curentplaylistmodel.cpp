#include "curentplaylistmodel.h"

using namespace syncLib;

CurentPlayListModel::CurentPlayListModel(QObject *parent) :
    QAbstractListModel(parent),
    syncEngine(nullptr),
    playList(nullptr)
{
    itemCount = 0;
}

void CurentPlayListModel::setSource(SyncEngine *engine){
    if(syncEngine){
        disconnect(syncEngine, SIGNAL(curentPlayListCountChanged()) , this, SLOT(onPlayListChanged()));
        disconnect(syncEngine, SIGNAL(curentSongChanged()), this, SIGNAL(curentSongIdChanged()));
    }

    syncEngine = engine;
    connect(syncEngine, SIGNAL(curentPlayListCountChanged()), this ,SLOT(onPlayListChanged()));
    connect(syncEngine, SIGNAL(curentSongChanged()), this ,SIGNAL(curentSongIdChanged()));
}

QHash<int, QByteArray> CurentPlayListModel::roleNames()const{
    QHash<int, QByteArray> roles;
    roles[nameRole] = "songName";
    roles[idRole] = "songId";
    return roles;
}

void CurentPlayListModel::onPlayListChanged(){
    beginResetModel();
    playList = syncEngine->curentPlayList();
    endResetModel();
}

bool CurentPlayListModel::canFetchMore(const QModelIndex & /* index */) const
{
    if (playList && itemCount < playList->size())
        return true;
    else
        return false;
}

void CurentPlayListModel::fetchMore(const QModelIndex & /* index */)
{
    int remainder = playList->size() - itemCount;
    int itemsToFetch = qMin(100, remainder);

    beginInsertRows(QModelIndex(), itemCount, itemCount + itemsToFetch - 1);

    itemCount += itemsToFetch;

    endInsertRows();
}

int CurentPlayListModel::rowCount(const QModelIndex & /* parent */) const
{
    return itemCount;
}

QVariant CurentPlayListModel::data(const QModelIndex &index, int role) const
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

int CurentPlayListModel::curentSongId(){
    return syncEngine->curentSongId();
}

QString CurentPlayListModel::curentSongName(){
    return syncEngine->curentSongName();
}
