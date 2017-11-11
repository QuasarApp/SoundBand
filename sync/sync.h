#ifndef SYNC_H
#define SYNC_H

class QString;
class QSqlDatabase;
class QMediaPlayer;
class QSqlQuery;
namespace syncLib {

class Song;
class Node;
class Syncer;
/**
 * @brief The Sync class is main class of this library.
 * the 'sync' has supported synced playning media files on network and saving media data into local database.
 */
class Sync
{
private:
    Node *node;
    QSqlDatabase *db;
    QMediaPlayer *player;
    QSqlQuery *qyery;
    /**
     * @brief initDB initialize local database of song
     */
    void initDB();
    /**
     * @brief save media data into local database;
     * @param song savining media data.
     * @return true if song saved else return false
     */
    bool save(const Song &song);
public:
    /**
     * @brief Play song in this device, if device has not supported playning media data this method throw MediaExcrption.
     * @param song playning media data.
     * @param syncdata data of synbced playning of media data.
     * @return true if all done else false.
     */
    bool Play(const Song &song, Syncer* syncdata = nullptr);
    /**
     * @brief Play song from local media file.
     * @param url of local media file.
     * @return true if all done else false.
     */
    bool Play(QString url) const;
    /**
     * @brief Play song from local database by id.
     * @param id_song of song.
     * @return true if all done else false.
     */
    bool Play(int id_song);
    /**
     * @brief Pause playning song.
     */
    void Pause() const;
    /**
     * @brief stop playning song.
     */
    void stop() const;
    /**
     * @brief jump - jump to new position of playning media data.
     * @param seek - a new position of media data.
     */
    void jump(const int seek) const;
    Sync();
    ~Sync();
};
}


#endif // SYNC_H
