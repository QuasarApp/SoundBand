#ifndef SYNC_H
#define SYNC_H
#include "song.h"
#include "node.h"
#include "LocalScanner.h"
#include <chrono>
#include "config.h"

#include <QMediaPlayer>

class QSqlDatabase;
class QSqlQuery;
class QBuffer;
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
    QSqlDatabase *db;
    QMediaPlayer *player;
    QBuffer *buffer;
    QList<SongHeader> playList;
    SongHeader * curentSong;
    QSqlQuery *qyery;
    QList<ETcpSocket*> servers;
    bool fbroadcaster;
    LocalScanner deepScaner;
    int port;
    QString dataBaseName;
    /**
     * @brief findHeader set curent song if playList have playng song
     * @return true if all done
     */
    bool findHeader(const Song& song);
    /**
     * @brief sqlErrorLog show sql error
     * @param qyery
     */
    void sqlErrorLog(const QString& qyery);

    /**
     * @brief rescan - search for existing servers
     * result saved in servers
     */
    void rescan(bool deep = false);
    /**
     * @brief initDB initialize local database of song
     */
    void initDB(const QString& database = DATABASE_NAME );
    /**
     * @brief load song of database;
     * @brief song -
     * @brief result - the resulting value;
     * @return true if everything's done
     */
    bool load(const SongHeader &song, Song &result);
    /**
     * @brief save media data into local database.
     * @param song savining media data.
     * @return id of song saved on local database.
     */
    int save(const Song &song);
    /**
     * @brief updateAvelableSongs will update the list of participants of songs.
     * @return true if all done
     */
    bool updateAvailableSongs();
    /**
     * @brief fromDataBase return a song from local database by id.
     * @param id of song saved in local database.
     * @return song drom local database.
     */
    Song fromDataBase(const int id);
    /**
     * @brief createPackage - Create a package that shows current state of the node
     * @param type - Type of an answer
     * @param pac - the resulting value
     * @return true if everything's done
     */
    bool createPackage(Type type , package& pac, const ETcpSocket *for_ = NULL);
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
     * @brief Play song in this device, if device has not supported playning media data this method throw MediaExcrption.
     * @param header of song
     * @param syncdata data of synbced playning of media data.
     * @return true if all done else false.
     */
    bool play(const SongHeader &header,  const Syncer *syncdata = nullptr);
    /**
     * @brief Play song in this device, if device has not supported playning media data this method throw MediaExcrption.
     * @param song playning media data.
     * @param syncdata data of synbced playning of media data.
     * @return true if all done else false.
     */
    bool play(const Song &song, const Syncer *syncdata = nullptr);
    /**
     * @brief Play song from local media file.
     * @param url of local media file.
     * @return true if all done else false.
     */
    bool play(QString url);
    /**
     * @brief Play song from local database by id.
     * @param id_song of song.
     * @return true if all done else false.
     */
    bool play(int id_song, Syncer* syncdata = nullptr);
    /**
     * @brief Pause playning song.
     * state - state of playning
     */
    void pause(bool state);
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
    bool sync(const Syncer& sync);
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
     * @brief addNewSong push a new song into local database.
     * @param name - name of pushed song
     * @return id of song.
     */
    int addNewSong(const QString &url);

    /**
     * @brief getEndPoint
     * @return end point of playng song.
     */
    qint64 getEndPoint() const;

    Sync(const QString address = DEFAULT_ADRESS, int port = DEFAULT_PORT, const QString& datadir = DATABASE_NAME);
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
