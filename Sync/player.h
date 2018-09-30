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
    milliseconds playDelay;
    QMediaPlayer::State predState;
    int bufferVolume;
    bool fSynced;

public:

    Player( QObject *parent = Q_NULLPTR, Flags flags = Flags());

    /**
     * @brief syncBegin - palyer waiting for sunced.
     */
    void syncBegin();

    /**
     * @brief syncEnd ending synced
     */
    void syncEnd();

    /**
     * @brief isSynced
     * @return true if palyer has been synced.
     */
    bool isSynced()const;

    ~Player();

};


#endif // PLAYER_H
