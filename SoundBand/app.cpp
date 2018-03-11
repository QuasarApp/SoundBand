#include "app.h"
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "syncengine.h"
#include "imageprovider.h"
#include "serverlistmodel.h"
#include "playlistsmodel.h"
#include "playlistmodel.h"
#include "currentplaylistmodel.h"
#include <QIcon>
#include <QSystemTrayIcon>

App::App(QObject* ptr):
    QObject(ptr)
{
    qmlEngine = new QQmlApplicationEngine();
    syncEngine = new SyncEngine();
    imageProvider = new ImageProvider(syncEngine);
    serverListModel = new ServerListModel();
    serverListModel->setSource(syncEngine);

    playListsModel = new PlayListsModel();
    playListsModel->setSource(syncEngine);

    currentPlayListModel = new CurrentPlayListModel();
    currentPlayListModel->setSource(syncEngine);

    playListModel = new PlayListModel();
    playListModel->setSource(syncEngine);

    trayIcon = new QSystemTrayIcon(this);

}

void App::setIcon()
{
    QIcon icon = QIcon(":/image/res/logo.png");
    trayIcon->setIcon(icon);

    trayIcon->setToolTip(tr("SoundBand"));
}

bool App::run(){

    qmlEngine->addImageProvider(QLatin1String("collection"), imageProvider);

    QQmlContext *ctxt = qmlEngine->rootContext();
    ctxt->setContextProperty("syncEngine", syncEngine);
    ctxt->setContextProperty("serverListModel", serverListModel);
    ctxt->setContextProperty("playListsModel", playListsModel);
    ctxt->setContextProperty("currentPlayListModel", currentPlayListModel);
    ctxt->setContextProperty("playListModel", playListModel);


    qmlEngine->load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (qmlEngine->rootObjects().isEmpty())
        return false;

    setIcon();

    return syncEngine->init();
}

App::~App(){
    delete playListModel;
    delete playListsModel;
    delete currentPlayListModel;
    delete serverListModel;
    delete syncEngine;
    delete qmlEngine;
}

