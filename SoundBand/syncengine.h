#ifndef SYNCENGINE_H
#define SYNCENGINE_H
#include "../sync/sync.h"
#include <QObject>
#include <QPixmap>


/**
 * @brief The SyncEngine class - this class is interface between syncLine and qml application.
 */
class SyncEngine : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString lastError READ lastError NOTIFY error)
    Q_PROPERTY(double pos READ pos NOTIFY posChanged)
    Q_PROPERTY(int repeat READ repeat WRITE setRepeat NOTIFY repeatChanged)

    enum Repeat{noRepeat, oneMusic, allPlayList};

private:
    syncLib::Sync *sync;
    syncLib::MySql * sqlApi;
    QString _curentPlayListName;
    QString _lastError;
    Repeat _repeat;
public:
    SyncEngine();

    /**
     * @brief songImageById
     * @param id - id of playingSong;
     * @return true if all done
     */
    bool songImageById(int id , QPixmap& image);

    /**
     * @brief songImageById
     * @param name - name of Song;
     * @return image of song
     */
    bool songImageByName(const QString & name, QPixmap &image);

    /**
     * @brief allPlayLists
     * @return names of all play lists
     */
    void allPlayLists(QStringList& playList) const;

    ~SyncEngine();
public slots:

    /**
     * @brief curentSongIndex
     * @return index of curent cong in selected playlist.
     */
    int curentSongIndex()const;

    /**
     * @brief curentSong
     * @return  curent song name
     */
    const QString& curentSong()const;

    /**
     * @brief curentPlayList
     * @return return curent Play List
     */
    const QList<syncLib::SongHeader> *curentPlayList() const;

    /**
     * @brief selectPlayList - selected a ne play list for player
     * @param list - name of play list
     * @return true if all done;
     */
    bool selectPlayList(const QString& list);

    /**
     * @brief curentPlayListName
     * @return name of curent play list
     */
    const QString& curentPlayListName()const;

    /**
     * @brief play - play curent music
     * @return true if all done.
     */
    bool play(int id);

    /**
     * @brief pause - set pause for curent song
     * @return true if all done.
     */
    bool pause(bool state);

    /**
     * @brief next - switch to next music
     * @return true if all done.
     */
    bool next();

    /**
     * @brief prev - switch to prev music
     * @return true if all done.
     */
    bool prev();

    /**
     * @brief listen corent server
     * @param index of server
     * @return true if all done
     */
    bool listen(int index);

    /**
     * @brief repeat
     * @return flag of Repeat
     */
    int repeat()const;

    /**
     * @brief setRepeat
     * @param flag
     */
    void setRepeat(int flag);

    /**
     * @brief setPlayList - select new play list.
     * @param name of selected play list
     * @return true if all done
     */
    bool setPlayList(const QString &name);

    /**
     * @brief lastError - message of last error
     * @return  message of last error
     */
    const QString& lastError()const;

    /**
     * @brief pos
     * @return posistion of curent song on double
     */
    double pos()const;

    /**
     * @brief getServerList
     * @return list of servers
     */
    const QList<ETcpSocket *> *getServerList() const;

    /**
     * @brief scan
     */
    void scan();

signals:

    /**
     * @brief curentSongChanged
     * This signal can be emitted when curent Music changed
     */
    void curentSongChanged();

    /**
     * @brief error
     * This signal can be emitted when error has been detected
     */
    void error();

    /**
     * @brief posChanged
     * This signal can be emitted when position of curent song changed
     */
    void posChanged();

    /**
     * @brief repeatChanged
     * This signal can be emitted when repeat state changed
     */
    void repeatChanged();

    /**
     * @brief serversCountChanged
     * This signal can be emitted when finded new servers or removed old servers
     */
    void serversCountChanged();

    /**
     * @brief playListsCountChanged
     * This signal can be emitted when finded new playlists or removed old playlists
     */
    void playListsCountChanged();

    /**
     * @brief curentPlayListCountChanged
     * This signal can be emitted when finded new songs or removed old songs
     */
    void curentPlayListCountChanged();

};

#endif // SYNCENGINE_H
