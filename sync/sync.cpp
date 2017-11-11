#include "sync.h"
#include <QtSql>
#include <QMultimedia>
#include <QMediaPlayer>
#include <QSqlQuery>
#include "song.h"
#include "node.h"
#include "exaptions.h"

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
    db = QSqlDatabase::addDatabase("QSQLITE");
    QDir d("./" + DATABASE_NAME);
    db.setDatabaseName(d.absolutePath());
    if(db->open()){
        qyery = new QSqlQuery(db);
        QString qyer = QString("CREATE TABLE IF NOT EXISTS %0 "
                     "id int NOT NULL AUTO_INCREMENT,"
                     "name VARCHAR(100),"
                     "size INT NOT NULL,"
                     "data BLOB NOT NULL").arg(DATATABLE_NAME);
        qyery->exec(qyer);
    }
}

bool Sync::save(const Song &song){
    QString qyer = QString("INSERT INTO %0 (name, size, data) VALUES"
                           "(%1,%2, :data)").arg(DATATABLE_NAME,
                                                 song.name,
                                                 QString::number(song.size));
    qyery->prepare(qyer);
    qyery->bindValue(":data",song.source);

    return qyery->exec();
}

bool Sync::Play(const Song& song){
    QBuffer buffer(&song.source);
    player->setMedia(QMediaContent(), &buffer);
    player->play();
}

Sync::~Sync(){
    delete node;
    delete db;
    delete player;
}

}

