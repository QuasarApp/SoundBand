#ifndef SONG_H
#define SONG_H
class QString;
class QByteArray;
class QDataStream;
//class Sync;
namespace syncLib {

class SongHeader
{
public:
    int id;
    QString name;
    int size;
    SongHeader();
    SongHeader& operator = (const SongHeader& right);
    bool operator == (const SongHeader& right);
    virtual unsigned int getSize();
    virtual ~SongHeader();
    friend QDataStream& operator << (QDataStream& stream, const SongHeader& song);
    friend QDataStream& operator >> (QDataStream& stream, SongHeader& song);
};

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
