#include "playlist.h"

PlayList::PlayList(QMediaPlaylist *playList)
{
    playListInfo.clear();
    this->playList = playList;
}

QList<SongStorage>* PlayList::getInfo(){
    return &playListInfo;
}

QMediaPlaylist* PlayList::getList(){
    return playList;
}

void PlayList::clear(){
    playList->clear();
    playListInfo.clear();
}

bool PlayList::addMedia(const SongStorage &song){
    if(!song.isValid())
        return false;

    if(!playList->addMedia(song.toMedia()))
        return false;

    playListInfo.push_back(song);
    return true;
}

bool PlayList::isValid()const{
    return playListInfo.size() == playList->mediaCount();
}

bool PlayList::isEmpty()const{
    return playList->isEmpty() && playListInfo.isEmpty();
}

const SongHeader& PlayList::currentHeader()const{
    return static_cast<const SongHeader&>(playListInfo[playList->currentIndex()]);
}

const SongStorage& PlayList::currentSong()const{
    return playListInfo[playList->currentIndex()];
}

int PlayList::size()const{
    return playList->mediaCount();
}

PlayList::~PlayList(){

}
