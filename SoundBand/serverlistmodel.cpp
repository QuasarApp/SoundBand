#include "serverlistmodel.h"
#include "syncengine.h"

ServerListModel::ServerListModel(QObject *parent) :
    QAbstractListModel(parent),
    syncEngine(nullptr),
    servers(nullptr)
{
    itemCount = 0;
}

void ServerListModel::setSource(const SyncEngine *engine){
    if(syncEngine)
        disconnect(syncEngine, SIGNAL(serversCountChanged()) ,this, SLOT(onServersListsChanged()));
    syncEngine = engine;
    connect(syncEngine, SIGNAL(serversCountChanged()),this ,SLOT(onServersListsChanged()));
}

QHash<int, QByteArray> ServerListModel::roleNames()const{
    QHash<int, QByteArray> roles;
    roles[nameRole] = "serverName";
    return roles;
}

void ServerListModel::onServersListsChanged(){
    beginResetModel();
    servers = syncEngine->getServerList();
    endResetModel();
}

bool ServerListModel::canFetchMore(const QModelIndex & /* index */) const
{
    if (servers && itemCount != servers->size())
        return true;
    else
        return false;
}

void ServerListModel::fetchMore(const QModelIndex & /* index */)
{
    int remainder = servers->size() - itemCount;
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

int ServerListModel::rowCount(const QModelIndex & /* parent */) const
{
    return itemCount;
}

QVariant ServerListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= servers->size() || index.row() < 0)
        return QVariant();

    if (role == nameRole) {
        return servers->at(index.row())->peerName();

    }
    return QVariant();
}
