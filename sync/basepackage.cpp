#include "basepackage.h"

package::package()
{
    clear();
}

package::package( QByteArray &array):
    package::package(){
    parseFrom(array);
}

const SongHeader& package::getHeader() const{
    return header;
}

const Song& package::getSong() const{
    return source;
}

const Syncer& package::getPlayData() const{
    return playdata;
}

const Type& package::getType() const{
    return type;
}

const milliseconds& package::getTime() const{
    return time;
}

bool package::isValid() const{

    bool ret = true;
    if(type == TypePackage::t_void){
        return false;

    }

    if(type & TypePackage::t_sync && type & t_brodcaster){
        ret = ret && playdata.seek > 0 && playdata.timeOn > 0;

    }

    if(type & TypePackage::t_song_h && type & t_brodcaster){
        ret = ret && header.size > 0;

    }

    if(type & TypePackage::t_song && type & t_brodcaster){
        ret = ret && source.size > 0;

    }

    return ret;

}

void package::clear(){
    type = TypePackage::t_void;
    source.clear();
    playdata.seek = 0;
    time = -1;
}

QByteArray package::parseTo(){
    QByteArray temp;
    QDataStream stream(&temp, QIODevice::WriteOnly);
    temp.clear();
    if(isValid()){
        stream <<  static_cast<unsigned char>(type);

        if(type & TypePackage::t_sync && type & t_brodcaster){
            stream << playdata.seek;
            stream << playdata.timeOn;

        }

        if(type & TypePackage::t_song_h && type & t_brodcaster){
            stream << header;

        }

        if(type & TypePackage::t_song && type & t_brodcaster){
            stream << source;

        }

        if(type & TypePackage::t_syncTime){
            stream << time;
        }

    }
    return temp;
}

bool package::parseFrom(QByteArray &array){
    type = TypePackage::t_void;
    QDataStream stream(&array, QIODevice::ReadOnly);

    unsigned char temp_type;
    stream >> temp_type;
    type = static_cast<TypePackage> (temp_type);

    if(type & TypePackage::t_sync){
        stream >> playdata.seek;
        stream >> playdata.timeOn;


    }

    if(type & TypePackage::t_song_h){
        stream >> header;

    }

    if(type & TypePackage::t_song){
        stream >> source;

    }

    if(type & TypePackage::t_syncTime){
        stream >> time;
    }

    return isValid();
}

package::~package(){}

