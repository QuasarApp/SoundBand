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
    QList<syncLib::SongHeader> playList;
    QString playListName;
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
     * @brief select a song from playList or unselected if item has been selected;
     * @param id - if of song
     * @return true if all done
     */
    bool select(int id);

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

    /**
     * @brief setNewList load a new play list into model
     * @param name - name of loaded playlist
     */
    void setNewList(const QString & name);
};

#endif // PLAYLISTMODEL_H
