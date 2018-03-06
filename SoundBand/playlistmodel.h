#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include <QAbstractListModel>
#include <syncengine.h>

class SyncEngine;


/**
 * @brief The PlayListsModel class
 */
class PlayListModel : public QAbstractListModel
{
    Q_OBJECT

private:
    SyncEngine * syncEngine;
    const QList<syncLib::SongHeader> *playList;
    int itemCount;
    QString playListName;

private slots:
    /**
     * @brief onPlayListsChanged check new playLists
     */
    void onPlayListChanged();
protected:
    bool canFetchMore(const QModelIndex &parent) const override;
    void fetchMore(const QModelIndex &parent) override;

public:
    explicit PlayListModel(QObject *parent = nullptr);

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

public slots:

    /**
     * @brief setNewPlayList set new base play list
     * @param playList - name of playlist
     */
    void setNewPlayList(const QString &playList);

    /**
     * @brief select a song from playList;
     * @param id - if of song
     * @return true if all done
     */
    bool select(int id);

    /**
     * @brief unselect a song from playList;
     * @param id - if of song
     * @return true if all done
     */
    bool unSelect(int id);

    /**
     * @brief getSelected
     * @return list of selected songs
     */
    QList<int> getSelected();

    /**
     * @brief isSelected
     * @param id - id of checked song
     * @return true if song selected
     */
    bool isSelected(int id);
};

#endif // PLAYLISTMODEL_H
