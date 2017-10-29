#ifndef SONG_H
#define SONG_H
class QString;
class QByteArray;
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
    virtual ~SongHeader();
};

class Song : public SongHeader{
private:
    QByteArray source;
public:
    Song();
    Song(const SongHeader& from);
    ~Song();
};

}
#endif // SONG_H
