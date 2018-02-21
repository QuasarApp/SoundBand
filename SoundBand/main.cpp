#include <QApplication>
#include <QQmlApplicationEngine>
#include "syncengine.h"
#include "filedialog.h"

int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    qmlRegisterType<SyncEngine>("SyncEngine",1,0,"SyncEngine");
    qmlRegisterType<FileDialog>("FileDialog",1,0,"FileDialog");

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;
    return app.exec();
}
