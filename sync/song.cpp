#include "song.h"
#include <QStringList>
#include <QRegularExpression>
#include <QFile>

namespace syncLib{

static const QStringList ValidSongs = {".mp3", ".wav", ".ogg"};
SongHeader::SongHeader()
{
    this->isSelected = false;
    this->id = -1;
    this->name = "";
    this->size = 0;
}

bool SongHeader::getName(QString & name, const QUrl &url) const {
    if(url.isLocalFile() && url.isValid()){
        name = url.fileName();
        name = name.right(name.lastIndexOf(QRegularExpression("[\\\/]")));
        return true;
    }

    return false;

}

bool SongHeader::getSize(int & size, const QUrl &url) const {
    if(url.isLocalFile() && url.isValid()){
        QFile f(url.toLocalFile());
        if(!f.exists()){
            return false;
        }
        size = f.size();
        return true;
    }

    return false;

}

SongHeader& SongHeader::operator =(const SongHeader& right){
    this->id = right.id;
    this->name = right.name;
    this->size = right.size;
    return *this;
}

SongHeader& SongHeader::operator =(const QMediaContent& right){
    this->id = -1;
    if(!getName(name, right.canonicalUrl())){
        name.clear();
    }

    if(!getSize(this->size, right.canonicalUrl())){
        this->size = 0;
    }

    return *this;
}

bool SongHeader::operator ==(const SongHeader& right)const{
    return this->name == right.name && this->size == right.size;
}

bool SongHeader::operator ==(const QMediaContent& right)const{
    QString name;
    if(!getName(name, right.canonicalUrl())){
        return false;
    }

    int size;;
    if(!getSize(size, right.canonicalUrl())){
        return false;
    }

    return this->name == name && this->size == size;
}

bool SongHeader::isNameValid() const{
    bool CheckSongs = false;
    for (QString i: ValidSongs){
        CheckSongs = CheckSongs || name.endsWith(i);
    }
    return CheckSongs;
}

bool SongHeader::isValid() const{

    return id > -1 && !name.isEmpty() && size > 0 && isNameValid();

}

SongHeader::~SongHeader(){}

QDataStream& operator << (QDataStream& stream, const SongHeader& song){
    stream << song.id;
    stream << song.name;
    stream << song.size;
    return stream;
}
QDataStream& operator >> (QDataStream& stream, SongHeader& song){
    stream >> song.id;
    stream >> song.name;
    stream >> song.size;
    return stream;
}


SongStorage::SongStorage():
    SongHeader()
{
    url.clear();
}

SongStorage::SongStorage(const SongHeader& from)
    :SongStorage::SongStorage()
{
    this->id = from.id;
    this->name = from.name;
    this->size = from.size;
}

SongStorage::SongStorage(const QUrl& from)
    :SongStorage::SongStorage()
{
    if(!from.isValid() || !from.isLocalFile()){
        return;
    }

    QFile f(from.toLocalFile());
    this->size = f.size();
    f.close();

    this->id = -1;
    url = from;
    if(!getName(name, from)){
        name.clear();
    }
}

SongStorage::SongStorage(const QMediaContent& from)
    :SongStorage::SongStorage(from.canonicalUrl())
{

}

const QUrl& SongStorage::getSource()const{
    return url;
}

bool SongStorage::isValid() const{
    return SongHeader::isValid() && url.isValid() && QFile(url.toLocalFile()).exists();
}

SongStorage::~SongStorage(){
    url.clear();
}

QMediaContent SongStorage::toMedia()const{
    return QMediaContent(url);
}

bool SongStorage::toSong(Song& song)const{
    song = (*((SongHeader*)this));

    QFile f(url.toLocalFile());

    if(!f.open(QIODevice::ReadOnly))
        return false;
    song.source = f.readAll();

    f.close();
    return song.isValid();
}

QDataStream& operator << (QDataStream& stream,const SongStorage& song){
    stream << static_cast<const SongHeader&>(song);
    stream << song.url;
    return stream;
}

QDataStream& operator >> (QDataStream& stream, SongStorage& song){
    stream >> static_cast<SongHeader&>(song);
    stream >> song.url;
    return stream;
}


Song::Song():
    SongHeader()
{
    source.clear();
}

Song::Song(const SongHeader& from)
    :Song::Song()
{
    this->id = from.id;
    this->name = from.name;
    this->size = from.size;
}

void Song::clear(){
    source.clear();
}

const QByteArray& Song::getSource()const{
    return source;
}

bool Song::isValid() const{

    return SongHeader::isValid() && source.size() == size;
}

Song::~Song(){
    source.clear();
}

QDataStream& operator << (QDataStream& stream,const Song& song){
    stream << static_cast<const SongHeader&>(song);
    stream << song.source;
    return stream;
}

QDataStream& operator >> (QDataStream& stream, Song& song){
    stream >> static_cast<SongHeader&>(song);
    stream >> song.source;
    return stream;
}

}
