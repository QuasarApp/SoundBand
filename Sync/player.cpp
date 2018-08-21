#include "player.h"
#include <stdio.h>
#include "exaptions.h"
#include <QFile>
#include <QDir>


Player::Player(const QString &bufferFile, QObject *parent, Flags flags):
    QMediaPlayer(parent, flags)
{
   buffer = bufferFile;
   bufferVolume = 0;
}

void Player::syncBegin(){

    bufferVolume = volume();
    setVolume(0);

    play();

}

void Player::syncEnd(){

    if(bufferVolume){
        setVolume(bufferVolume);
        bufferVolume = 0;
    }

}

bool Player::isSynced()const{
    return !bufferVolume;
}

Player::~Player(){

}
