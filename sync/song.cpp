#include "song.h"
#include <QString>
#include <QByteArray>

syncLib::SongHeader::SongHeader()
{
    this->id = 0;
    this->name = "";
    this->size = 0;
}

syncLib::SongHeader& syncLib::SongHeader::operator =(const syncLib::SongHeader& right){
    this->id = right.id;
    this->name = right.name;
    this->size = right.size;
    return *this;
}

bool syncLib::SongHeader::operator ==(const syncLib::SongHeader& right){
    return this->name == right.name && this->size == right.size;
}

syncLib::SongHeader::getSize(){
    QByteArray size;
    QDataStream stream(&size);
    stream << id << name << this->size;
    return size.size();
}

syncLib::Song::Song():
    syncLib::SongHeader()
{
    source.clear();
}

friend QDataStream& operator << (QDataStream& stream, const syncLib::SongHeader& song){
    stream << song.id;
    stream << song.name;
    stream << song.size;
    return stream;
}
friend QDataStream& operator >> (QDataStream& stream, syncLib::SongHeader& song){
    stream >> song.id;
    stream >> song.name;
    stream >> song.size;
    return stream;
}

syncLib::Song::Song(const SongHeader& from)
    :syncLib::Song::Song()
{
    this->id = from.id;
    this->name = from.name;
    this->size = from.size;
}

syncLib::Song::clear(){
    source.clear();
}

syncLib::Song::~Song(){
    source.clear();
}

syncLib::Song::getSize(){
    return syncLib::SongHeader::getSize() + source.size();
}

QDataStream operator << (QDataStream& stream,const syncLib::Song& song){
    stream << (syncLib::SongHeader)(*this);
    stream << song.source;
}

QDataStream operator << (QDataStream& stream, syncLib::Song& song){
    stream >> (syncLib::SongHeader)(*this);
    stream >> song.source;
}
