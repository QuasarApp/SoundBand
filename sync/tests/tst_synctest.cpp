#include <QtTest>
#include "../sync.h"
#include <thread>
#include <QMediaPlayer>
// add necessary includes here

class SyncTest : public QObject
{
    Q_OBJECT

public:
    SyncTest();
    ~SyncTest();

private slots:

    void sycn_tests();

    void package_tests();

};

SyncTest::SyncTest()
{

}

SyncTest::~SyncTest()
{

}

void SyncTest::sycn_tests()
{


    syncLib::Sync *sync = new syncLib::Sync;

    QVERIFY(sync != nullptr);

    QVERIFY(sync->play(":/song/test_song"));

    sync->stop();
    QVERIFY(sync->play(1));

    sync->stop();
    QVERIFY(!sync->play(2));

    QVERIFY(QFile(DATABASE_NAME).size() == 2068480);

    delete sync;

    sync = new syncLib::Sync(LOCAL_HOST, 1994);
    syncLib::Sync sync2(LOCAL_HOST, 1998,"test2.dat");

    QVERIFY(sync->play(1));

    sync->stop();
    QVERIFY(sync2.addNode(LOCAL_HOST, 1994));

    delete sync;

}

void SyncTest::package_tests()
{


}

QTEST_APPLESS_MAIN(SyncTest)

#include "tst_synctest.moc"
