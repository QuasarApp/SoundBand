#ifndef SONG_H
#define SONG_H
#include <QString>
#include <QByteArray>
#include <QDataStream>
#include "chronotime.h"

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
    milliseconds seek;
};

/**
 * @brief The SongHeader class sound header with media information
 * (id,size and name)
 */
class SongHeader{

public:
    bool isSelected;
    int id;
    QString name;
    int size;
    SongHeader();
    SongHeader& operator = (const SongHeader& right);
    bool operator == (const SongHeader& right);
    virtual unsigned int getSize() const;
    bool isNameValid() const;
    virtual bool isValid() const;
    virtual ~SongHeader();

    /**
     * serialize data:
     * id,
     * size,
     * and,
     * name
     */
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
    const QByteArray& getSource()const;
    unsigned int getSize() const;
    bool isValid() const;
    ~Song();
    friend QDataStream& operator << (QDataStream& stream, const Song& song);
    friend QDataStream& operator >> (QDataStream& stream, Song& song);
    friend class MySql;
};
}
#endif // SONG_H
