#include "sync.h"
#include <QtSql>
#include <QMultimedia>
#include <QMediaPlayer>
#include <QSqlQuery>
#include "song.h"
#include "node.h"
#include "exaptions.h"
#include "time.h"
#include "thread"

#include "config.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

namespace syncLib{
/**
 * @brief Clock system time on nanosecunds
 */
typedef std::chrono::high_resolution_clock Clock;
/**
 * @brief Time_point on nanosecunds (int64)
 */
typedef Clock::time_point Time_point;

Sync::Sync(){
    node = new Node();
    player = new QMediaPlayer(nullptr,QMediaPlayer::LowLatency);
    if(!player->isAvailable()){
        throw MediaException();
    }
}

void Sync::initDB(){
    if(db) return;
    *db = QSqlDatabase::addDatabase("QSQLITE");
    QDir d(QString("./%0").arg(DATABASE_NAME));
    db->setDatabaseName(d.absolutePath());
    if(db->open()){
        qyery = new QSqlQuery(*db);
        QString qyer = QString("CREATE TABLE IF NOT EXISTS %0 "
                     "id int NOT NULL AUTO_INCREMENT,"
                     "name VARCHAR(100),"
                     "size INT NOT NULL,"
                     "data BLOB NOT NULL").arg(DATATABLE_NAME);
        qyery->exec(qyer);
    }
}

int Sync::save(const Song &song){
    QString qyer = QString("INSERT INTO %0 (name, size, data) VALUES"
                           "(%1,%2, :data)").arg(DATATABLE_NAME,
                                                 song.name,
                                                 QString::number(song.size));
    qyery->prepare(qyer);
    qyery->bindValue(":data",song.source);
    if(!qyery->exec())
        return -1;
    if(qyery->exec(QString("SELECT MAAX(id) form %0").arg(DATATABLE_NAME)))
        return -1;
    return qyery->value(0).toInt();
}

bool Sync::Play(const Song& song, Syncer *syncdata){
    QBuffer buffer(&song.source);
    player->setMedia(QMediaContent(), &buffer);
    if(syncdata){
        Time_point sync_time = syncdata->run  - Clock::now();
        auto max_sync = Time_point(MAX_SYNC_TIME * 1000000);
        if(sync_time > max_sync && sync_time <= 0)
            return false;
        do {
            std::this_thread::yield();
        } while (Clock::now() < syncdata->run);
        player->setPosition(syncdata->seek);
    }
    player->play();
    return true;
}

bool Sync::Play(int id_song){

    QString qyer = QString("SELECT * from %0 where id=%1").arg(DATATABLE_NAME).arg(id_song);
    if(!qyery->exec(qyer)){
        return false;
    }
    Song song;
    song.id = qyery->value(0).toInt();
    song.name = qyery->value(1).toString();
    song.size = qyery->value(2).toInt();
    song.source = qyery->value(3).toByteArray();
    return Sync::Play(song);
}

bool Sync::Play(QString url){
    QFile f(url);
    if(!f.open(QIODevice::ReadOnly)){
        return false;
    }
    QByteArray bytes = f.readAll();
    f.close();
    QString name = url.right(url.lastIndexOf(QRegularExpression("[\\/]"))); // meby [[\\\/]]
    Song song;
    song.name = name;
    song.size = bytes.size();
    song.source = bytes;
    song.id = Sync::save(song);
    if(song.id < 0)
        return false;
    return Sync::Play(song);
}

void Sync::Pause(){
    player->pause();
}

void Sync::stop(){
    player->stop();
}

void Sync::jump(const int seek){
    player->setPosition(seek);
}

Sync::~Sync(){
    delete node;
    delete db;
    delete player;
}

}

