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

    void player_tests();


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

void SyncTest::player_tests()
{
    QFile f(":/song/test_song");

    QVERIFY(f.open(QIODevice::ReadOnly));

    Player pl(BUFFER_NAME);

    QByteArray array = f.readAll();
    f.close();
    QVERIFY(pl.setMediaFromBytes(array));

    f.setFileName(BUFFER_NAME);

    QVERIFY(f.open(QIODevice::ReadOnly));

    QByteArray array2 = f.readAll();
    f.close();

    QVERIFY(array.length() == array2.length());

    QVERIFY(pl.setMediaFromBytes(array));

    QVERIFY(f.open(QIODevice::ReadOnly));

    array2 = f.readAll();

    QVERIFY(array.length() == array2.length());




}

QTEST_APPLESS_MAIN(SyncTest)

#include "tst_synctest.moc"
