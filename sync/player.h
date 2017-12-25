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
    bool fSynced;

private slots:
    void _stateChanged(QMediaPlayer::State state);
public:

    Player(const QString& bufferFile, QObject *parent = Q_NULLPTR, Flags flags = Flags());

    /**
     * @brief setMediaFromBytes
     * @param array of song data
     * @return true if all done
     */
    bool setMediaFromBytes(const QByteArray& array);

    /**
     * @brief getPlayDelay
     * @return delay of playing song.
     * @warning: working only with setMediaFromBytes.
     */
    milliseconds getPlayDelay();
    ~Player();

};


#endif // PLAYER_H
