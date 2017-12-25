#include "player.h"
#include <stdio.h>
#include "exaptions.h"
#include <QFile>
#include <QDir>


Player::Player(const QString &bufferFile, QObject *parent, Flags flags):
    QMediaPlayer(parent, flags)
{
   buffer = bufferFile;
   playDelay = 0;
   predState = state();
   fSynced = false;
   connect(this, SIGNAL(stateChanged(QMediaPlayer::State)), SLOT(_stateChanged(QMediaPlayer::State)));
}

bool Player::setMediaFromBytes(const QByteArray &array){
    QFile f(buffer);
    if(!f.open(QIODevice::WriteOnly | QIODevice::Truncate)){
        return false;
    }

    if(array.length() != f.write(array.data(),array.length())){

        f.close();
        return false;
    }



    f.close();

    setMedia(QUrl::fromLocalFile(QDir("./").absoluteFilePath(buffer)));
    play();
    pause();
    playDelay = ChronoTime::now();
    play();


    return true;

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
    if(fSynced)
        return playDelay;
    return -1;

}

Player::~Player(){

}
