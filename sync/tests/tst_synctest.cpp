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
    void test_case1();

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


}

QTEST_APPLESS_MAIN(SyncTest)

#include "tst_synctest.moc"
