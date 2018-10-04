#include "app.h"
#include <QFileInfo>
#include <QDir>
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
#include <exaptions.h>

App::App(QObject* ptr):
    QObject(ptr)
{

    if (!initAppDir()) {
        throw initNodeError();
    }

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

bool App::run() {

    qmlEngine->addImageProvider(QLatin1String("collection"), imageProvider);

    QQmlContext *ctxt = qmlEngine->rootContext();
    ctxt->setContextProperty("syncEngine", syncEngine);
    ctxt->setContextProperty("serverListModel", serverListModel);
    ctxt->setContextProperty("playListsModel", playListsModel);
    ctxt->setContextProperty("currentPlayListModel", currentPlayListModel);
    ctxt->setContextProperty("playListModel", playListModel);


    qmlEngine->load(QUrl(QStringLiteral("qrc:/QML/main.qml")));
    if (qmlEngine->rootObjects().isEmpty())
        return false;

    setIcon();

    return syncEngine->init();
}

bool App::initAppDir() const {

    QDir dir(QDir::rootPath());

    if (!QFileInfo::exists(MAIN_FOLDER) &&
        !dir.mkpath(MAIN_FOLDER)) {

        return false;
    }

    return true;
}

App::~App(){
    delete playListModel;
    delete playListsModel;
    delete currentPlayListModel;
    delete serverListModel;
    delete syncEngine;
    delete qmlEngine;
}

