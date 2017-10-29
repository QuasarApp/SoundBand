#ifndef SYNC_H
#define SYNC_H

class QString;
namespace syncLib {
class Song;
class Sync
{
private:
    bool save(Song song);
public:
    bool Play(QString url) const;
    void Pause() const;
    void stop() const;
    void jump() const;
    Sync();
    ~Sync();
};
}


#endif // SYNC_H
