#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "syncengine.h"
#include <QFont>

int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
//    QFont f = app.font();
//    f.setPixelSize(1);
//    QGuiApplication::setFont(f);
    qmlRegisterType<SyncEngine>("SyncEngine",1,0,"SyncEngine");
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;
    return app.exec();
}
