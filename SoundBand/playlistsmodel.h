#ifndef PLAYLISTSMODEL_H
#define PLAYLISTSMODEL_H

class SyncEngine;
#include <QAbstractListModel>

/**
 * @brief The PlayListsModel class
 */
class PlayListsModel : public QAbstractListModel
{
    Q_OBJECT

private:
    const SyncEngine * syncEngine;
    QStringList playLists;
    int itemCount;

private slots:
    /**
     * @brief onPlayListsChanged check new playLists
     */
    void onPlayListsChanged();
protected:
    bool canFetchMore(const QModelIndex &parent) const override;
    void fetchMore(const QModelIndex &parent) override;

public:
    explicit PlayListsModel(QObject *parent = nullptr);

    /**
     * @brief The ServerListRoles enum
     * nameRole - name of playlist
     * descriptionRole - description of playlist
     */
    enum ServerListRoles {
        nameRole = Qt::UserRole + 1,
        descriptionRole
    };

    /**
     * @brief setSource
     * @param engine
     */
    void setSource(const SyncEngine* engine);

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
};
#endif // PLAYLISTSMODEL_H
