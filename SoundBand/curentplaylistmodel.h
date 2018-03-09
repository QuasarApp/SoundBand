#ifndef CURENTPLAYLISTMODEL_H
#define CURENTPLAYLISTMODEL_H
#include <QAbstractListModel>
#include <syncengine.h>

class SyncEngine;


/**
 * @brief The PlayListsModel class
 */
class CurentPlayListModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(int curentSongId READ curentSongId NOTIFY curentSongIdChanged)

private:
    SyncEngine * syncEngine;
    const QList<syncLib::SongHeader> *playList;
    int itemCount;

private slots:
    /**
     * @brief onPlayListsChanged check new playLists
     */
    void onPlayListChanged();
protected:
    bool canFetchMore(const QModelIndex &parent) const override;
    void fetchMore(const QModelIndex &parent) override;

public:
    explicit CurentPlayListModel(QObject *parent = nullptr);

    /**
     * @brief The ServerListRoles enum
     * nameRole - name of playlist
     * imageRole - image of playlist
     */
    enum ServerListRoles {
        nameRole = Qt::UserRole + 1,
        idRole
    };

    /**
     * @brief setSource
     * @param engine
     */
    void setSource(SyncEngine *engine);

    /**
     * @brief AnimalModel::roleNames
     * @return pair of roles and value
     */
    QHash<int, QByteArray> roleNames() const;

    /**
     * @brief rowCount
     * @param parent
     * @return count of playlists
     */
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    /**
     * @brief data standart dataselector
     * @param index
     * @param role
     * @return
     */
    QVariant data(const QModelIndex &index, int role = nameRole) const override;

signals:

    /**
     * @brief curentSongIdChanged
     * emited when changed curent song
     */
    void curentSongIdChanged();

public slots:

    /**
     * @brief curentSongId
     * @return id of curent song
     */
    int curentSongId();
};

#endif // CURENTPLAYLISTMODEL_H
