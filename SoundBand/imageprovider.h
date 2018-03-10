#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H
#include <QPixmap>
#include <QQuickImageProvider>

class SyncEngine;

class ImageProvider: public QQuickImageProvider
{
private:
    SyncEngine *syncEngine;
public:
    explicit ImageProvider(SyncEngine * engine);

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);
};

#endif // IMAGEPROVIDER_H
