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

    return true;
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
