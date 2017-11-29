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
    /**
     * @brief test_case1 - check player and locale database;
     */
    void test_case1();
    /**
     * @brief test_case2 - check network
     */
    void test_case2();

};

SyncTest::SyncTest()
{

}

SyncTest::~SyncTest()
{

}

void SyncTest::test_case1()
{


    syncLib::Sync sync;

    QVERIFY(sync.play(":/song/test_song"));

    sync.stop();

    QVERIFY(sync.play(1));

    sync.stop();

    QVERIFY(!sync.play(2));

    QVERIFY(QFile(DATABASE_NAME).size() == 2068480);

}

void SyncTest::test_case2()
{


    syncLib::Sync sync(LOCAL_HOST, 1994);

    syncLib::Sync sync2(LOCAL_HOST, 1998,"test2.dat");

    QVERIFY(sync.play(1));

    sync.stop();

    QVERIFY(sync2.addNode(LOCAL_HOST, 1994));


}

QTEST_APPLESS_MAIN(SyncTest)

#include "tst_synctest.moc"
