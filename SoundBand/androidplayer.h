
#ifndef ANDROIDPLAYER_H
#define ANDROIDPLAYER_H

#include <QObject>
#ifdef Q_OS_ANDROID

class AndroidPlayer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString notification READ notification WRITE setNotification NOTIFY notificationChanged)

private:
    QString m_notification;

private slots:
    void updateAndroidNotification();

public:
    explicit AndroidPlayer(QObject *parent = nullptr);

QString notification() const;

public slots:
    void setNotification(QString notification);

signals:
    void notificationChanged();
};

#endif // OS_ANDROID

#endif // ANDROIDPLAYER_H

