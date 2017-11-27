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

    QVERIFY(sync.play(1));

    QVERIFY(!sync.play(2));

    QVERIFY(QFile(DATABASE_NAME).size() == 2068480);

}

void SyncTest::test_case2()
{


    syncLib::Sync sync(1994);

  //  syncLib::Sync sync2(1998);

    QVERIFY(sync.play(1));

   // QVERIFY(sync2.addNode("localhost",1994));

    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

   // QVERIFY(sync2.getServersList().size() > 0);

 //   QVERIFY(sync2.listen(sync2.getServersList().front()));

}

QTEST_APPLESS_MAIN(SyncTest)

#include "tst_synctest.moc"
