#include <QApplication>
#include <QQmlApplicationEngine>
#include "app.h"

int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QApplication app(argc, argv);
    App soundBand;
    if(!soundBand.run())
        return -1;

    return app.exec();
}
