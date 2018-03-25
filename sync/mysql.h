#ifndef MYSQL_H
#define MYSQL_H
#include <QString>
#include "song.h"

class QSqlDatabase;
class QSqlQuery;

namespace syncLib {

class MySql
{
private:
    QSqlDatabase *db;
    QSqlQuery *qyery;
    QString dataBaseName;
    QString songDir;

    /**
     * @brief sqlErrorLog show sql error
     * @param qyery
     */
    void sqlErrorLog(const QString& qyery) const;

    /**
     * @brief saveToStorage save song as file into hdd
     * @param url - url of song after save
     * @param song - saved song
     * @return true if all done
     */
    bool saveToStorage(QUrl& url, const Song& song)const;

public:
    MySql(const QString& databasename);
    /**
     * @brief initDB initialize local database of song
     */
    void initDB(const QString& database = DATABASE_NAME );

    /**
     * @brief setSoundDir
     * @param str
     */
    void setSoundDir(const QString& str);

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
     * @brief save media file, bud from url.
     */
    int save(const QString &url);

    /**
     * @brief fromDataBase return a song from local database by id.
     * @param id of song saved in local database.
     * @return song drom local database.
     */
    Song fromDataBase(const int id);

    /**
     * @brief updateAvelableSongs will update the list of participants of songs.
     * @param list - [out value] list of avelable song.
     * @param playList - play list of songs.
     * @param forEdit - flag for editing play list. If this flag = true then return all available songs with corect flag 'isSelect'
     * @return true if all done
     */
    bool updateAvailableSongs(QList<SongHeader>& list, const QString &playList = "", bool forEditing = false);

    /**
     * @brief updateAvelableSongs will update the list of participants of songs.
     * @param list - [out value] list of avelable song.
     * @param playList - play list of songs (string).
     * @return true if all done
     */
    bool updateAvailableSongs(QStringList& list, const QString &playList = "");

    /**
     * @brief removeSong - remove song from local database.
     * @param header - heder removing song. first : song removed by id,
     * if id of head = -1 then song removed by name and size, and if song no finded this finction return false.
     * @return trye if all done.
     */
    bool removeSong(const SongHeader& header);

    /**
     * @brief addPlayList add a new play list into database.
     * @param newPlayList - a new paly list.
     * @return trye if all done.
     */
    bool addPlayList(const QString& newPlayList, const QString& desc = "");

    /**
     * @brief addToPlayList - add song to play list.
     * @param header - header of song.  first : song removed by id,
     * if id of head = -1 then song removed by name and size, and if song no finded this finction return false.
     * @param newPlaylist
     * @return true if all done.
     */
    bool addToPlayList(const SongHeader& header, const QString& playList);

    /**
     * @brief addToPlayList - add song to play list.
     * @param header - header of song.  first : song removed by id,
     * if id of head = -1 then song removed by name and size, and if song no finded this finction return false.
     * @param newPlaylist
     * @return true if all done.
     */
    bool removeFromPlayList(const SongHeader& header, const QString& playList);

    /**
     * @brief removePlayList - remove play list from local database.
     * @param playList - removed play list.
     * @return true if all done.
     */
    bool removePlayList(const QString& playList);

    /**
     * @brief exec - execute сustom sql file.
     * @param q - sql query
     * @param sqlFile - sql - file with sql code.
     * @return trye if all done.
     */
    static bool exec(QSqlQuery *q, const QString& sqlFile);

    /**
     * @brief getPlayLists show all created playlists.
     * @param list - list of play lists.
     * @return trye if all done
     */
    bool getPlayLists(QStringList &list) const;

    /**
     * @brief getSongId get song id by name
     * @param name - name of song
     * @return true if finded song,
     *  bud if count of finded songs is greater than 1 return false
     */
    int getSongId(const QString& name);

    /**
     * @brief clear - clear free space in local database
     */
    void clear();

    ~MySql();
};
}
#endif // MYSQL_H
