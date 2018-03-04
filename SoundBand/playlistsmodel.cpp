#include "playlistsmodel.h"
#include "syncengine.h"

PlayListsModel::PlayListsModel(QObject *parent) :
    QAbstractListModel(parent),
    syncEngine(nullptr)
{
    itemCount = 0;
}

void PlayListsModel::setSource(const SyncEngine *engine){
    if(syncEngine)
        disconnect(syncEngine, SIGNAL(serversCountChanged()) ,this, SLOT(onPlayListsChanged()));
    syncEngine = engine;
    connect(syncEngine, SIGNAL(serversCountChanged()),this ,SLOT(onPlayListsChanged()));
    onPlayListsChanged();
}

QHash<int, QByteArray> PlayListsModel::roleNames()const{
    QHash<int, QByteArray> roles;
    roles[nameRole] = "playListName";
    roles[descriptionRole] = "playListDescription";
    return roles;
}

void PlayListsModel::onPlayListsChanged(){
    beginResetModel();
    syncEngine->allPlayLists(playLists);
    endResetModel();
}

bool PlayListsModel::canFetchMore(const QModelIndex & /* index */) const
{
    if (itemCount < playLists.size())
        return true;
    else
        return false;
}

void PlayListsModel::fetchMore(const QModelIndex & /* index */)
{
    int remainder = playLists.size() - itemCount;
    int itemsToFetch = qMin(100, remainder);

    beginInsertRows(QModelIndex(), itemCount, itemCount + itemsToFetch - 1);

    itemCount += itemsToFetch;

    endInsertRows();
}

int PlayListsModel::rowCount(const QModelIndex & /* parent */) const
{
    return itemCount;
}

QVariant PlayListsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= playLists.size() || index.row() < 0)
        return QVariant();

    switch (role) {
    case nameRole:
        return playLists[index.row()];
        break;
    case descriptionRole:
        return tr("not supported");
        break;
    default:
        break;
    }

    return QVariant();
}
