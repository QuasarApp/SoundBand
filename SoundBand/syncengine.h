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

    enum Repeat{noRepeat, oneMusic, allPlayList};

private:
    syncLib::Sync *sync;
    syncLib::MySql * sqlApi;
    QString _curentPlayListName;
    QString _lastError;
    Repeat _repeat;
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
     * @brief curentSongIndex
     * @return index of curent cong in selected playlist.
     */
    int curentSongIndex()const;

    /**
     * @brief curentSong
     * @return  curent song id
     */
    int curentSongId()const;

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
     * @brief getPlayList
     * @param playList return value (song headers)
     * @param name - name of selected playList
     * @return
     */
    bool getPlayList(QList<syncLib::SongHeader> &playList, const QString& name);

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
     * @brief songsCountChanged
     * This signal can be emitted when finded new song or removed old song
     */
    void songsCountChanged();

    /**
     * @brief curentPlayListCountChanged
     * This signal can be emitted when finded new songs or removed old songs
     */
    void curentPlayListCountChanged();

};

#endif // SYNCENGINE_H
