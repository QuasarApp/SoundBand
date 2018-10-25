#ifndef PLAYLIST_H
#define PLAYLIST_H
#include <QMediaPlaylist>
#include "song.h"
#include "sync_global.h"

/**
 * @brief The PlayList class
 * palyList with songs info
 */
class SYNCSHARED_EXPORT PlayList
{
private:
    /**
     * @brief playList media play list
     */
    QMediaPlaylist *playList;

    /**
     * @brief playListInfo - info from databas
     */
    QList<SongStorage> playListInfo;
public:
    PlayList();

    /**
     * @brief getInfo
     * @return info from database about songs
     */
    QList<SongStorage>* getInfo();

    /**
     * @brief clear this play list
     */
    void clear();

    /**
     * @brief addMedia media content into playList
     * @param song - media content
     * @return true if all done
     */
    bool addMedia(const SongStorage& song);

    /**
     * @brief next
     */
    void next()const;

    /**
     * @brief prev
     */
    void prev()const;

    /**
     * @brief isValid
     * @return true if this object valid
     */
    bool isValid()const;

    /**
     * @brief isEmpty
     * @return true if playlist empty
     */
    bool isEmpty()const;

    /**
     * @brief getlist
     * @return return pointer to media content
     */
    QMediaPlaylist* getList();

    /**
     * @brief currentHeader
     * @return header of curent song
     */
    const SongHeader* currentHeader()const;

    /**
     * @brief currentStorage
     * @return header of curent song
     */
    const SongStorage* currentSong()const;

    /**
     * @brief size
     * @return count of media items of playlist
     */
    int size()const;

    /**
     * @brief find
     * @return index of finded media
     */
    int find(const SongHeader& header)const;

    /**
     * @brief selectSong
     * @return true if song selected
     */
    bool selectSong(int index);

    /**
     * @brief selectSong
     * @param header of selecting song
     * @return if song selected return true;
     */
    bool selectSong(const SongHeader& header);

    ~PlayList();
};

#endif // PLAYLIST_H
