#ifndef SYNC_H
#define SYNC_H
#include "song.h"
#include "node.h"
#include "LocalScanner.h"
#include <chrono>
class QSqlDatabase;
class QMediaPlayer;
class QSqlQuery;
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
    QList<SongHeader>* playList;
    QSqlQuery *qyery;
    QList<ETcpSocket*> servers;
    bool fbroadcaster;
    LocalScanner deepScaner;


    /**
     * @brief rescan - search for existing servers
     * result saved in servers
     */
    void rescan(bool deep = false);
    /**
     * @brief initDB initialize local database of song
     */
    void initDB();
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
     * @brief fromDataBase return a song from local database by id.
     * @param id of song saved in local database.
     * @return song drom local database.
     */
    Song fromDataBase(const int id);
    /**
     * @brief now - get now time on microsecunds
     * @return - count of microsecunds
     */
    milliseconds now();
    /**
     * @brief from cast to chrono secunds
     * @param mcrs microseconds of uint_64
     * @return microseconds of chrono
     */
    Clock from(const milliseconds &mcrs);
    /**
     * @brief createPackage - Create a package that shows current state of the node
     * @param type - Type of an answer
     * @param pac - the resulting value
     * @return true if everything's done
     */
    bool createPackage(Type type ,package& pac);
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
    bool play(Song &song, Syncer *syncdata = nullptr);
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
     */
    void pause();
    /**
     * @brief stop playning song.
     */
    void stop();
    /**
     * @brief jump - jump to new position of playning media data.
     * @param seek - a new position of media data.
     */
    void jump(const int seek);
    /**
     * @brief sync with server
     * @param sync - data of sync
     */
    bool sync(const Syncer& sync);
    /**
     * @brief getVersion
     * @return curent version of library
     */
    QString getVersion();
    Sync();
    ~Sync();
};
}


#endif // SYNC_H