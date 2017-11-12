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

/*
 * information about chrono
 * https://stackoverflow.com/questions/31255486/c-how-do-i-convert-a-stdchronotime-point-to-long-and-back
 */

microseconds Sync::now(){
    auto tim = std::chrono::system_clock::now();
    auto mc = std::chrono::time_point_cast<std::chrono::microseconds>(tim);
    auto epoh = mc.time_since_epoch();
#ifdef QT_DEBUG
    qDebug() << epoh.count();
#endif
    return epoh.count();
}

Clock Sync::from(const microseconds& mc){
    std::chrono::duration<long> dur(mc);
    return Clock(dur);
}

bool Sync::Play(Song& song, Syncer *syncdata){
    QBuffer buffer(&song.source);
    player->setMedia(QMediaContent(), &buffer);
    if(syncdata){
        microseconds sync_time  = syncdata->run - now();
        if(sync_time > MAX_SYNC_TIME && sync_time <= 0)
            return false;
        Clock run_time = from(syncdata->run);
        do {
            std::this_thread::yield();
        } while (std::chrono::high_resolution_clock::now() < run_time);
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

