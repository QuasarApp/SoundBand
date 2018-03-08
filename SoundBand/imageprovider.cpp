#include "imageprovider.h"
#include "syncengine.h"

ImageProvider::ImageProvider(SyncEngine *engine):
    QQuickImageProvider(QQuickImageProvider::Pixmap)
{
    syncEngine = engine;
}

QPixmap ImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize){
    short width = 100;
    short height = 50;

    QPixmap result;

    if (size)
        *size = QSize(width, height);

    bool ok;
    int songId = id.toShort(&ok);

    if(!ok)
        return QPixmap(1,1);

    if(!syncEngine->songImageById(songId, result)){
       return QPixmap(":/image/res/logo.png");
    }

    return result.scaled(requestedSize);
}

