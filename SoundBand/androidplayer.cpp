#include "androidplayer.h"
#ifdef Q_OS_ANDROID

#include <QAndroidJniObject>

AndroidPlayer::AndroidPlayer(QObject *parent)
    : QObject(parent) {

    connect(this, SIGNAL(notificationChanged()),
            this, SLOT(updateAndroidNotification()));
}

QString AndroidPlayer::notification() const {
    return m_notification;
}

void AndroidPlayer::setNotification(QString notification) {
    if (m_notification == notification)
        return;

    m_notification = notification;
    emit notificationChanged();
}

void AndroidPlayer::updateAndroidNotification() {
    QAndroidJniObject javaNotification = QAndroidJniObject::fromString(m_notification);
    QAndroidJniObject::callStaticMethod<void>("org.quasarapp.sounband.NotificationClient",
                                       "notify",
                                       "(Ljava/lang/String;)V",
                                       javaNotification.object<jstring>());
}

#endif
