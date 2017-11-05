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


syncLib::Song::Song():
    syncLib::SongHeader()
{
    source.clear();
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
