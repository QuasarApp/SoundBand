#include "sync.h"
#include <QtSql>
#include <QMultimedia>
#include <QMediaPlayer>
#include <QSqlQuery>
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

bool Sync::load(const SongHeader &song,Song &result){
    result.clear();
    if(song.id > -1){
        QString qyer = QString("SELECT * from %0 where id=%1").arg(DATATABLE_NAME).arg(song.id);
        if(!qyery->exec(qyer)){
            return false;
        }
    }else if(!song.name.isEmpty() && song.size > 0){
        QString qyer = QString("SELECT * from %0 where name=%1 and size=%2").arg(DATATABLE_NAME).arg(song.name).arg(song.size);
        if(!qyery->exec(qyer)){
            return false;
        }
    }else {
        return false;
    }

    result.id = qyery->value(0).toInt();
    result.name = qyery->value(1).toString();
    result.size = qyery->value(2).toInt();
    result.source = qyery->value(3).toByteArray();
    return true;
}

/*
 * information about chrono
 * https://stackoverflow.com/questions/31255486/c-how-do-i-convert-a-stdchronotime-point-to-long-and-back
 */

milliseconds Sync::now(){
    auto tim = std::chrono::system_clock::now();
    auto mc = std::chrono::time_point_cast<std::chrono::milliseconds>(tim);
    auto epoh = mc.time_since_epoch();
#ifdef QT_DEBUG
    qDebug() << epoh.count();
#endif
    return epoh.count();
}

Clock Sync::from(const milliseconds& mc){
    std::chrono::milliseconds dur(mc);
    return Clock(dur);
}

bool Sync::Play(SongHeader &header, Syncer *syncdata){
    QString qyer = QString("SELECT * from %0 where name=%1 and size=%2").arg(DATATABLE_NAME).arg(header.name).arg(header.size);
    if(!qyery->exec(qyer)){
        return false;
    }
    Song song;
    song.id = qyery->value(0).toInt();
    song.name = qyery->value(1).toString();
    song.size = qyery->value(2).toInt();
    song.source = qyery->value(3).toByteArray();
    return Sync::Play(song,syncdata);
}

bool Sync::Play(Song& song, Syncer *syncdata){
    QBuffer buffer(&song.source);
    player->setMedia(QMediaContent(), &buffer);
    if(syncdata && !sync(*syncdata)){
        return false;
    }
    fbroadcaster = !bool(syncdata);
//    if(fbroadcaster){
//    }
    player->play();
    playList->push_front(static_cast<SongHeader>(song));
    return true;
}

bool Sync::Play(int id_song, Syncer *syncdata){

    QString qyer = QString("SELECT * from %0 where id=%1").arg(DATATABLE_NAME).arg(id_song);
    if(!qyery->exec(qyer)){
        return false;
    }
    Song song;
    song.id = qyery->value(0).toInt();
    song.name = qyery->value(1).toString();
    song.size = qyery->value(2).toInt();
    song.source = qyery->value(3).toByteArray();
    return Sync::Play(song,syncdata);
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

bool Sync::sync(const Syncer &sync){
    milliseconds sync_time  = sync.run - now();
    if(sync_time > MAX_SYNC_TIME && sync_time <= 0)
        return false;
    Clock run_time = from(sync.run);
    do {
        std::this_thread::yield();
    } while (std::chrono::high_resolution_clock::now() < run_time);
    player->setPosition(sync.seek);
    return true;
}

bool Sync::createPackage(TypePackage type, package &pac){
    pac.clear();

    pac.type = type;

    if(type & TypePackage::t_sync){
        if(!fbroadcaster)
            return false;

        pac.type = type;
        pac.playdata.run = now() + SYNC_TIME;
        pac.playdata.seek = player->position() + SYNC_TIME;

    }

    if(type & TypePackage::t_song_h){
        if(!fbroadcaster || playList->isEmpty())
            return false;

        pac.type = type;
        pac.header = playList->front();

    }

    if(type & TypePackage::t_song){
        if(!fbroadcaster || playList->isEmpty())
            return false;

        pac.type = type;
        if(!load(playList->front(), pac.source))
            return false;

    }

    return pac.isValid();
}

Sync::~Sync(){
    delete node;
    delete db;
    delete player;
}

}


