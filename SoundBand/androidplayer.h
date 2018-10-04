#ifdef Q_OS_ANDROID

#ifndef ANDROIDPLAYER_H
#define ANDROIDPLAYER_H

#include <QObject>

class AndroidPlayer : public QObject
{
    Q_OBJECT
public:
    explicit AndroidPlayer(QObject *parent = nullptr);

signals:

public slots:
};

#endif // ANDROIDPLAYER_H

#endif // OS_ANDROID
