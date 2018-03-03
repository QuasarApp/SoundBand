#include "app.h"
#include <QQmlApplicationEngine>
#include "syncengine.h"
#include "imageprovider.h"

App::App(QObject* ptr):
    QObject(ptr)
{
    qmlEngine = new QQmlApplicationEngine();
    syncEngine = new SyncEngine();
    imageProvider = new ImageProvider(syncEngine);
}

bool App::run(){
    qmlRegisterType<SyncEngine>("SyncEngine",1,0,"SyncEngine");

    qmlEngine->load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (qmlEngine->rootObjects().isEmpty())
        return false;

    return true;
}

App::~App(){
    delete syncEngine;
    delete qmlEngine;
}

