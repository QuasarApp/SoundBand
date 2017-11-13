#ifndef SONG_H
#define SONG_H
#include <QString>
#include <QByteArray>
#include <QDataStream>
#include <config.h>
/**
 * @brief Time_point on nanosecunds (uint64_t)
 */
typedef quint64 microseconds;

namespace syncLib {

/**
 * @brief The Syncer struct
 *
 */
struct Syncer
{
    /**
     * @brief seek - wher is play media file
     */
    unsigned int seek;
    /**
     * @brief run when is play media file (int)
     */
    microseconds run;
};

/**
 * @brief The SongHeader class sound header with media information
 * (id,size and name)
 */
class SongHeader
{
public:
    int id;
    QString name;
    int size;
    SongHeader();
    SongHeader& operator = (const SongHeader& right);
    bool operator == (const SongHeader& right);
    virtual unsigned int getSize()const;
    virtual ~SongHeader();
    friend QDataStream& operator << (QDataStream& stream, const SongHeader& song);
    friend QDataStream& operator >> (QDataStream& stream, SongHeader& song);
};
/**
 * @brief The Song class
 * into this calss added mediadata of playable media file.
 */
class Song : public SongHeader{
private:
    QByteArray source;
public:
    Song();
    Song(const SongHeader& from);
    void clear();
    unsigned int getSize() const;
    ~Song();
    friend QDataStream& operator << (QDataStream& stream, const Song& song);
    friend QDataStream& operator >> (QDataStream& stream, Song& song);
    friend class Sync;
};
}
#endif // SONG_H
