#ifndef SYNCENGINE_H
#define SYNCENGINE_H
#include "../sync/sync.h"
#include <QObject>
#include <QPixmap>
#include <QSettings>


/**
 * @brief The SyncEngine class - this class is interface between syncLine and qml application.
 */
class SyncEngine : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString lastError READ lastError NOTIFY error)
    Q_PROPERTY(double pos READ pos WRITE setPos NOTIFY posChanged)
    Q_PROPERTY(int repeat READ repeat WRITE setRepeat NOTIFY repeatChanged)
    Q_PROPERTY(QString currentPlayListName READ currentPlayListName NOTIFY currentPlayListNameChanged)
    Q_PROPERTY(int playState READ playState NOTIFY playStateChanged)


private:
    Sync *sync;
    MySql * sqlApi;
    QString _lastError;
    QSettings settings;

private slots:
    void seekChanged(qint64);

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

    /**
     * @brief init - inited all avelable songs
     */
    bool init();

    ~SyncEngine();
public slots:

    /**
     * @brief currentSongName
     * @return Name of current cong in selected playlist.
     */
    QString currentSongName()const;

    /**
     * @brief currentSong
     * @return  current song id
     */
    int currentSongId()const;

    /**
     * @brief currentPlayList
     * @return return current Play List
     */
    const QList<SongStorage> *currentPlayList() const;

    /**
     * @brief currentPlayListName
     * @return name of current play list
     */
    const QString& currentPlayListName()const;

    /**
     * @brief play - play current music
     * @return true if all done.
     */
    bool play(int id);

    /**
     * @brief pause - set pause for current song
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
     * @brief getPlayList
     * @param playList return value (song headers)
     * @param name - name of selected playList
     * @return
     */
    bool getPlayList(QList<SongStorage> &playList, const QString& name);

    /**
     * @brief lastError - message of last error
     * @return  message of last error
     */
    const QString& lastError()const;

    /**
     * @brief pos
     * @return posistion of current song on double
     */
    double pos()const;

    /**
     * @brief setPos
     * @param newPos
     */
    void setPos(double newPos);

    /**
     * @brief setValume - set a new valume of playin a song
     */
    void setValume(int);

    /**
     * @brief getValume
     * @return valune ofsong
     */
    int getValume()const;

    /**
     * @brief getServerList
     * @return list of servers
     */
    const QList<ETcpSocket *> *getServerList() const;

    /**
     * @brief scan
     */
    void scan();

    /**
     * @brief addSong added a new song from file
     * @param songUrl - url to song
     * @return true if all done
     */
    bool addSong(const QString &songUrl);

    /**
     * @brief removeSong remove song from database
     * @param id - id of removing song
     * @return true if all done
     */
    bool removeSong(int id);

    /**
     * @brief createPlayList
     * @return true if new play list created
     */
    bool createPlayList(const QString &name);

    /**
     * @brief removePlayList
     * @return true if new play list removed
     */
    bool removePlayList(const QString &name);

    /**
     * @brief addToPlayList added selected song into playlist
     * @param id of song
     * @param playList name of playlist
     * @return true if all done
     */
    bool addToPlayList(int id, const QString& playList);

    /**
     * @brief addToPlayList remove selected song from playlist
     * @param id of song
     * @param playList name of playlist
     * @return true if all done
     */
    bool removeFromPlayList(int id, const QString& playList);

    /**
     * @brief playState
     * @return playState current song ( )
     * @value 0 is no repeat
     * @value 1 is single repeat
     * @value 2 is single playlist
     * @value 3 is repeat playlist
     * @value 4 is random play
     */
    int playState()const;

signals:

    /**
     * @brief error
     * This signal can be emitted when error has been detected
     */
    void error();

    /**
     * @brief posChanged
     * This signal can be emitted when position of current song changed
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
     * @brief songsCountChanged
     * This signal can be emitted when finded new song or removed old song
     */
    void songsCountChanged();

    /**
     * @brief currentPlayListCountChanged
     * This signal can be emitted when finded new songs or removed old songs
     */
    void currentPlayListCountChanged();

    /**
     * @brief currentSongChanged
     * emited when changed a playing song
     */
    void currentSongChanged();

    /**
     * @brief currentPlayListNameChanged
     *  emited when selected new PlayList;
     */
    void currentPlayListNameChanged();

    /**
     * @brief playStateChanged
     * emited when state of playing song changed
     */
    void playStateChanged();

};

#endif // SYNCENGINE_H
