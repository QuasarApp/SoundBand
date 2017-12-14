#ifndef PLAYER_H
#define PLAYER_H

#include <QMediaPlayer>

class Player : public QMediaPlayer
{
    Q_OBJECT
private:
    QString buffer;
public:
    Player(const QString& bufferFile, QObject *parent = Q_NULLPTR, Flags flags = Flags());
    bool setMediaFromBytes(const QByteArray& array);
    ~Player();

};


#endif // PLAYER_H
