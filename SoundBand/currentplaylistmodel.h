#ifndef CURENTPLAYLISTMODEL_H
#define CURENTPLAYLISTMODEL_H
#include <QAbstractListModel>
#include <syncengine.h>

class SyncEngine;


/**
 * @brief The PlayListsModel class
 */
class CurrentPlayListModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(int currentSongId READ currentSongId NOTIFY currentSongIdChanged)

private:
    SyncEngine * syncEngine;
    const QList<syncLib::SongStorage> *playList;
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
    explicit CurrentPlayListModel(QObject *parent = nullptr);

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
     * @brief currentSongIdChanged
     * emited when changed current song
     */
    void currentSongIdChanged();

public slots:

    /**
     * @brief currentSongId
     * @return id of current song
     */
    int currentSongId();

    /**
     * @brief currentSongName
     * @return name of current song
     */
    QString currentSongName();
};

#endif // CURENTPLAYLISTMODEL_H
