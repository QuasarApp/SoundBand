#include "player.h"
#include <stdio.h>
#include "exaptions.h"
#include <QFile>
#include <QDir>
#include <thread>

Player::Player(const QString &bufferFile, QObject *parent):
    QObject(parent)
{
   if(!rescanDevices()){
       throw AudioDevicesError();
       delete this;
       return;
   }
   buffer = bufferFile;
   playDelay = 0;
   fbuffered = false;

   decoder = new QAudioDecoder(this);

   output = new QAudioOutput(availableDevices.first(), this);

   connect(decoder, SIGNAL(finished()), this , SLOT(decodeComplit()));

}

int Player::rescanDevices(){

    availableDevices = QAudioDeviceInfo::availableDevices(QAudio::MusicRole);

    return availableDevices.size();
}

void Player::decodeComplit(){
    if(decoder->error() == QAudioDecoder::NoError){
        buffer = decoder->read();
        fbuffered = true;
        buf.close();
    }else{
        throw AudioDecodeError(decoder->errorString());
    }
}

bool Player::setMediaFromBytes(const QByteArray &array){
    decoder->setAudioFormat(void());

    buf.setData(array);

    if(!buf.open(QIODevice::ReadOnly)){
        return false;
    }

    fbuffered = false;

    decoder->setSourceDevice(buf);

    decoder->start();

    return true;

}

bool Player::play(int waiting){
    milliseconds now = ChronoTime::now();
    while(!fbuffered && (ChronoTime::now() - now) < waiting ){
        std::this_thread::yield();
    }

    if(!fbuffered){
        return false;
    }

}

void Player::_stateChanged(QMediaPlayer::State state){
    if(!fSynced && state == QMediaPlayer::PlayingState && predState == QMediaPlayer::PausedState){
        playDelay = ChronoTime::now() - playDelay;
        pause();
        fSynced = true;
    }
    predState = state;
}

milliseconds Player::getPlayDelay(){
    if(fbuffered)
        return playDelay;
    return -1;

}

Player::~Player(){

    availableDevices->clear();
    delete decoder;
    delete output;
}
