#include "playlistmodel.h"

using namespace syncLib;

PlayListModel::PlayListModel(QObject *parent) :
    QAbstractListModel(parent),
    syncEngine(nullptr),
    playList(nullptr)
{
    itemCount = 0;
}

void PlayListModel::setSource(SyncEngine *engine){
    if(syncEngine)
        disconnect(syncEngine, SIGNAL(curentPlayListCountChanged()) ,this, SLOT(onPlayListChanged()));
    syncEngine = engine;
    connect(syncEngine, SIGNAL(curentPlayListCountChanged()),this ,SLOT(onPlayListChanged()));
}

QHash<int, QByteArray> PlayListModel::roleNames()const{
    QHash<int, QByteArray> roles;
    roles[nameRole] = "songName";
    roles[idRole] = "songId";
    roles[imageRole] = "songImage";
    return roles;
}

void PlayListModel::onPlayListChanged(){
    beginResetModel();
    if(syncEngine->selectPlayList(playListName)){
        playList = syncEngine->curentPlayList();
    }
    endResetModel();
}

bool PlayListModel::canFetchMore(const QModelIndex & /* index */) const
{
    if (playList && itemCount < playList->size())
        return true;
    else
        return false;
}

void PlayListModel::fetchMore(const QModelIndex & /* index */)
{
    int remainder = playList->size() - itemCount;
    int itemsToFetch = qMin(100, remainder);

    beginInsertRows(QModelIndex(), itemCount, itemCount + itemsToFetch - 1);

    itemCount += itemsToFetch;

    endInsertRows();
}

int PlayListModel::rowCount(const QModelIndex & /* parent */) const
{
    return itemCount;
}

QVariant PlayListModel::data(const QModelIndex &index, int role) const
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
    case imageRole:
        return tr("not supported");
        break;
    default:
        break;
    }

    return QVariant();
}

void PlayListModel::setNewPlayList(const QString &playList){
    playListName = playList;
    onPlayListChanged();
}
