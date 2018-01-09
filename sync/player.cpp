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

bool Player::syncBegin(){

    if(!isAudioAvailable()){
        return false;
    }
    bufferVolume = volume();
    setVolume(0);

    play();

    return true;

}

bool Player::syncEnd(){

    if(!bufferVolume){
        return false;
    }
    setVolume(bufferVolume);
    bufferVolume = 0;

    return true;

}

bool Player::isSynced()const{
    return !bufferVolume;
}

Player::~Player(){

}
