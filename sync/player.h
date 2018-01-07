#ifndef PLAYER_H
#define PLAYER_H

#include <QAudioDecoder>
#include <QAudioFormat>
#include <QAudioOutput>
#include <QAudioDeviceInfo>
#include <QMediaMetaData>
#include <QBuffer>

#include "chronotime.h"

/**
 * @brief The Player class is layer for QMediaPlayer.
 */

class Player : public QObject
{
    Q_OBJECT
private:
    QString buffer;
    milliseconds playDelay;
    bool fbuffered;
    QBuffer buf;
    QList<QAudioDeviceInfo> availableDevices;
    QAudioDecoder *decoder;
    QAudioOutput *output;
    QAudioBuffer buffer;

protected:

    /**
     * @brief rescanDevices
     * @return count of findet devices.
     */
    int rescanDevices();
private slots:
    /**
     * @brief decodeComplit
     */
    void decodeComplit();
public:

    Player(const QString& bufferFile, QObject *parent = Q_NULLPTR);

    /**
     * @brief setMediaFromBytes
     * @param array of song data
     * @return true if all done
     */
    bool setMediaFromBytes(const QByteArray& array);

    /**
     * @brief play curent song;
     * @param waiting : time of waiting for buffering source data.
     * @return true if all done.
     */
    bool play(int waiting = DEFAULT_BUFFERING_TIME);

    /**
     * @brief stop curent song.
     * @return trye if all done.
     */
    bool stop();

    /**
     * @brief seek move playing song to point
     * @param point - new point playng.
     * @return true uf all done.
     */
    bool seek(const milliseconds &point);

    /**
     * @brief setNewsUpdataeinterval - set a new updating interval
     * @param interval on milisecunds
     */
    void setNewsUpdataeInterval(const milliseconds &interval);

    /**
     * @brief UpdataeInterval return interval of emiting signals.
     * @return
     */
    milliseconds UpdataeInterval()const;

    /**
     * @brief getPlayDelay
     * @return delay of playing song.
     * @warning: working only with setMediaFromBytes.
     */
    milliseconds getPlayDelay();
    ~Player();

signals:

    /**
     * @brief positionChanged
     * Signal the position of the content has changed to position, expressed in milliseconds.
     */
    void positionChanged(const milliseconds &pos);

    /**
     * @brief progressChanged signals when progress changed. see
     * @param progress range : from 0 to 1 where 0 is 0% 1 is 100%.
     */
    void progressChanged(const float& progress);

    /**
     * @brief stateChanged emited when the paying statte of song changed.
     */
    void stateChanged(const QAudio::State);

};


#endif // PLAYER_H
