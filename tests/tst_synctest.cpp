#include <QtTest>
#include "sync.h"
#include <thread>
#include <QMediaPlayer>
#include "Log.h"

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

    void database_tests();

    void network_tests();

    void log_tests();

};

SyncTest::SyncTest()
{

}

SyncTest::~SyncTest()
{

}

void SyncTest::sycn_tests()
{


    Sync *sync = new Sync;

    QVERIFY(sync != nullptr);

    QVERIFY(!sync->play(":/song/test_song"));

    QVERIFY(sync->play(":/song/test_song.mp3"));


    sync->stop();
    QVERIFY(sync->play(1));

    sync->stop();
    QVERIFY(!sync->play(2));

    QVERIFY(QFile(DATABASE_NAME).size() == 2084864);

    delete sync;

    sync = new Sync(LOCAL_HOST, 1994);
    Sync sync2(LOCAL_HOST, 1998,"test2.dat");

    QVERIFY(sync->play(1));

    sync->stop();
    QVERIFY(sync2.addNode(LOCAL_HOST, 1994));

    delete sync;

}

void SyncTest::player_tests()
{
}

void SyncTest::database_tests()
{
    MySql sql("test1");
    SongHeader header;
    SongStorage song;

    QVERIFY(!sql.load(header, song));

    QVERIFY(!sql.addToPlayList(header, "none"));

    header.id = sql.save(":/song/test_song.mp3");
    QVERIFY(header.id > 0);

    QVERIFY(sql.load(header ,song));

    header = static_cast<SongHeader>(song);
    QVERIFY(sql.load(header, song));

    QVERIFY(sql.addPlayList("play", "desc of play"));

    QVERIFY(sql.addToPlayList(header, "play"));

    QVERIFY(!sql.addToPlayList(header, "play"));

    QVERIFY(sql.removeFromPlayList(header, "play"));

    header.id = -1;
    QVERIFY(sql.addToPlayList(header, "play"));

    QVERIFY(!sql.addToPlayList(header, "play"));

    PlayList list;
    sql.updateAvailableSongs(list);

    QVERIFY(list.size() == 1);
    list.clear();
    sql.updateAvailableSongs(list, "play");

    QVERIFY(list.size() == 1);

    sql.updateAvailableSongs(list, "play2");
    QVERIFY(list.size() == 0);

    QVERIFY(sql.removeFromPlayList(header, "play"));

    QVERIFY(sql.removeSong(header));

    sql.updateAvailableSongs(list);
    QVERIFY(list.size() == 0);

    QStringList playlists;
    sql.getPlayLists(playlists);
    QVERIFY(playlists.size() == 1);

    QVERIFY(sql.removePlayList("play"));

    sql.getPlayLists(playlists);
    QVERIFY(playlists.size() == 0);

    sql.clear();
    QVERIFY(QFile("test1").size() == 32768);

}

void SyncTest::network_tests(){

    Node node1("127.0.0.1", 1994);

    QVERIFY(node1.getClients()->size() == 0);

    ETcpSocket socket("127.0.0.1", 1994);

    node1.waitForNewConnection(500);
    QVERIFY(node1.getClients()->size() > 0);

    QByteArray array(10, 'c');
    QVERIFY(node1.getClients()->first()->Write(array));

    node1.close();
    socket.getSource()->close();

}

void SyncTest::log_tests(){
    Log log("test.log");
    log.setShowDateTime(false);

    log.write("test");
    QVERIFY(QFile("test.log").size() == 18);

    log.setShowDateTime(true);
    log.write("test2");
    QVERIFY(QFile("test.log").size() == 57);

    QFile("test.log").remove();
}

QTEST_APPLESS_MAIN(SyncTest)

#include "tst_synctest.moc"
