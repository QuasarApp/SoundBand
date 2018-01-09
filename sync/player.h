#ifndef PLAYER_H
#define PLAYER_H

#include <QMediaPlayer>
#include "chronotime.h"

/**
 * @brief The Player class is layer for QMediaPlayer.
 */
class Player : public QMediaPlayer
{
    Q_OBJECT
private:
    QString buffer;
    milliseconds playDelay;
    QMediaPlayer::State predState;
    int bufferVolume;
    bool fSynced;

public:

    Player(const QString& bufferFile, QObject *parent = Q_NULLPTR, Flags flags = Flags());

    /**
     * @brief setMediaFromBytes
     * @param array of song data
     * @return true if all done
     */
    bool setMediaFromBytes(const QByteArray& array);

    /**
     * @brief syncBegin
     * @return true if palyer waiting for sunced.
     */
    bool syncBegin();

    /**
     * @brief syncEnd
     * @return true if player waiting for finished sync.
     */
    bool syncEnd();

    /**
     * @brief isSynced
     * @return true if palyer has been synced.
     */
    bool isSynced()const;

    ~Player();

};


#endif // PLAYER_H
