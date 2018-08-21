#include <QApplication>
#include <QQmlApplicationEngine>
#include "app.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);
    App soundBand;
    if(!soundBand.run())
        return -1;

    return app.exec();
}
