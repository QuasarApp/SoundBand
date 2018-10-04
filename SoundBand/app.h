#ifndef APP_H
#define APP_H

#include <QObject>
#include "syncengine.h"

class SyncEngine;
class QQmlApplicationEngine;
class ImageProvider;
class ServerListModel;
class PlayListsModel;
class PlayListModel;
class CurrentPlayListModel;
class QSystemTrayIcon;

/**
 * @brief The App class
 * general application object
 */
class App : public QObject
{
private:
    SyncEngine *syncEngine;
    QQmlApplicationEngine *qmlEngine;
    ImageProvider *imageProvider;
    ServerListModel *serverListModel;
    PlayListsModel *playListsModel;
    CurrentPlayListModel *currentPlayListModel;
    PlayListModel *playListModel;
    QSystemTrayIcon *trayIcon;
    void setIcon();

public:
    explicit App(QObject *ptr = nullptr);

    /**
     * @brief run app
     * @return  false if app not running, true if all done
     */
    bool run();

    /**
     * @brief initAppDir
     * @return true if all done else false
     */
    bool initAppDir() const;
    ~App();
};

#endif // APP_H
