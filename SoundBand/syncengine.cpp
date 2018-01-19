#include "syncengine.h"

/*QPixmap ColorImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize){

    QStringList params;

    int index = 0;
    int oldIndex;
    while((index = id.indexOf('/', index + 1)) != -1){
        index ++;
        params.push_back(id.mid(oldIndex, index));
        oldIndex = index;
    }

    QString comand = params.first();

    if(comand == CURENT_SONG){

    }

    if(comand == SONG_BY_ID && params.size() > ){

    }
}

*/
SyncEngine::SyncEngine()
{

}
const QString& SyncEngine::curentSong()const{
    return "";
}
