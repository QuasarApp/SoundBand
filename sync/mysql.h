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

    /**
     * @brief sqlErrorLog show sql error
     * @param qyery
     */
    void sqlErrorLog(const QString& qyery);

public:
    MySql(const QString& databasename);
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
     * @return true if all done
     */
    bool updateAvailableSongs(QList<SongHeader>& list);

    static bool exec(QSqlQuery *q, const QString& sqlFile);

    ~MySql();
};
}
#endif // MYSQL_H
