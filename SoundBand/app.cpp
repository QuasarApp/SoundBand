#include "app.h"
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "syncengine.h"
#include "imageprovider.h"
#include "serverlistmodel.h"
#include "playlistsmodel.h"
#include "playlistmodel.h"


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

    playListModel = new PlayListModel();
    playListModel->setSource(syncEngine);

    currentPlayListModel = new PlayListModel();
    currentPlayListModel->setSource(syncEngine);
}

bool App::run(){

    qmlEngine->addImageProvider(QLatin1String("collection"), imageProvider);

    QQmlContext *ctxt = qmlEngine->rootContext();
    ctxt->setContextProperty("syncEngine", syncEngine);
    ctxt->setContextProperty("serverListModel", serverListModel);
    ctxt->setContextProperty("playListsModel", playListsModel);
    ctxt->setContextProperty("playListModel", playListModel);
    ctxt->setContextProperty("currentPlayListModel", currentPlayListModel);

    qmlEngine->load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (qmlEngine->rootObjects().isEmpty())
        return false;

    return true;
}

App::~App(){
    delete playListsModel;
    delete playListModel;
    delete currentPlayListModel;
    delete serverListModel;
    delete syncEngine;
    delete qmlEngine;
}

