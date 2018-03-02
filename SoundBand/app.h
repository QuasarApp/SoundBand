#ifndef APP_H
#define APP_H

#include <QObject>
#include "syncengine.h"

class SyncEngine;
class QQmlApplicationEngine;

/**
 * @brief The App class
 * general application object
 */
class App : public QObject
{
private:
    SyncEngine *syncEngine;
    QQmlApplicationEngine *qmlEngine;
public:
    explicit App(QObject *ptr = nullptr);

    /**
     * @brief run app
     * @return  false if app not running, true if all done
     */
    bool run();
    ~App();
};

#endif // APP_H
