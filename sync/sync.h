#ifndef SYNC_H
#define SYNC_H
#include "song.h"
#include "node.h"
#include "LocalScanner.h"
#include <chrono>
#include "config.h"
#include "mysql.h"
#include "player.h"

namespace syncLib {

typedef std::chrono::time_point<std::chrono::high_resolution_clock> Clock;

class Node;

/**
 * @brief The Sync class is main class of this library.
 * the 'sync' has supported synced playning media files on network and saving media data into local database.
 */
class Sync : public QObject
{
    Q_OBJECT
private:
    Node *node;
    Player *player;
    QList<SongHeader> playList;
    SongHeader *curentSong;
    QList<ETcpSocket*> servers;
    bool fbroadcaster;
    int resyncCount;
    int lastSyncTime;
    int ping;
    LocalScanner deepScaner;
    MySql *sql;
    int port;
    /**
     * @brief findHeader set curent song if playList have playng song
     * @return true if all done
     */
    bool findHeader(const Song& song);

    /**
     * @brief rescan - search for existing servers
     * result saved in servers
     */
    void rescan(bool deep = false);

    /**
     * @brief createPackage - Create a package that shows current state of the node
     * @param type - Type of an answer
     * @param pac - the resulting value
     * @return true if everything's done
     */
    bool createPackage(Type type , package& pac);
private slots:

    /**
     * @brief packageRender - the handler of all messages received.
     * @param socket
     */
    void packageRender(ETcpSocket* socket);

    /**
     * @brief deepScaned scaning in local network
     */
    void deepScaned(QList<ETcpSocket *> *);

    /**
     * @brief endPlay signal has ending playing
     */
    void endPlay(QMediaPlayer::State state);

public:

    /**
     * @brief getSqlApi
     * @return pointer of sql api
     */
    MySql* getSqlApi();

    /**
     * @brief Play song in this device, if device has not supported playning media data this method throw MediaExcrption.
     * @param header of song
     * @param fbroadcast - server broadcasting sound.
     * @return true if all done else false.
     */
    bool play(const SongHeader &header,  bool fbroadcast = true);
    /**
     * @brief Play song in this device, if device has not supported playning media data this method throw MediaExcrption.
     * @param song playning media data.
     * @param fbroadcast - server broadcasting sound.
     * @return true if all done else false.
     */
    bool play(const Song &song, bool fbroadcast = true);
    /**
     * @brief Play song from local media file.
     * @param url of local media file.
     * @return true if all done else false.
     */
    bool play(QString url);
    /**
     * @brief Play song from local database by id.
     * @param id_song of song.
     * @param fbroadcast - server broadcasting sound.
     * @return true if all done else false.
     */
    bool play(int id_song, bool fbroadcast = true);
    /**
     * @brief Pause playning song.
     * state - state of playning
     */
    bool pause(bool state);
    /**
     * @brief stop playning song.
     */
    void stop();

    /**
     * @brief jump - jump to new position of playning media data.
     * @param seek - a new position of media data.
     */
    void jump(const qint64 seek);

    /**
     * @brief sync with server
     * @param sync - data of sync
     */
    bool sync(const Syncer& sync, milliseconds ping);

    /**
     * @brief isReadyToSync
     * @return true if node ready to sync;
     */
    bool isReadyToSync()const;

    /**
     * @brief sync with clients
     */
    void sync();

    /**
     * @brief addNode add new connect
     * @param ip of connection
     * @param port of connection
     * @return true if all done
     */
    bool addNode(const QString ip, int port);

    /**
     * @brief scan - search for existing servers
     * result saved in servers
     */
    void scan();

    /**
     * @brief getServersList
     * @return  list of servers
     */
    const QList<ETcpSocket*>& getServersList() const;

    /**
     * @brief listen - a server
     * @param server - host
     * @return true id all done
     */
    bool listen(ETcpSocket* server);

    /**
     * @brief getVersion
     * @return curent version of library
     */
    QString getVersion();

    /**
     * @brief setValume max valume is 100
     * @return true if all done
     */
    bool setValume(unsigned int value);

    /**
     * @brief getvalume
     * This property holds the current playback volume.
     * The playback volume is scaled linearly,
     *  ranging from 0 (silence) to 100 (full volume).
     *  Values outside this range will be clamped.
     */
    unsigned int getValume() const;

    /**
     * @brief seek
     * @return curent playning milisecond
     */
    unsigned int seek()const;

    /**
     * @brief getPlayList
     * @return list of available songs
     */
    const QList<SongHeader> *getPlayList() const;

    /**
     * @brief getCurentSong
     * @return playing song.
     */
    const SongHeader *getCurentSong() const;

    /**
     * @brief getEndPoint
     * @return end point of playng song.
     */
    qint64 getEndPoint() const;

    /**
     * @brief addNewSong push a new song into local database.
     * @param name - name of pushed song
     * @return id of song.
     */
    int addNewSong(const QString &url);

    /**
     * @brief updatePlayList this method set a new playlist
     * @param id - id of playlist
     * @return true if all don
     */
    bool updatePlayList(const QString& _playList);

    Sync(const QString &address = DEFAULT_ADRESS, int port = DEFAULT_PORT, const QString& datadir = DATABASE_NAME);
    ~Sync();

signals:
    /**
     * @brief seekChanged
     * @param seek
     * Signal the position of the content has changed to position,
     * expressed in milliseconds.
     */
    void seekChanged(qint64 seek);

    /**
     * @brief networkStateChange
     * signal if changed count of activity servers.
     */
    void networkStateChange();

};
}


#endif // SYNC_H
