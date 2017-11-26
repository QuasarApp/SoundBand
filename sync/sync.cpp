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

Sync::Sync():
    node(nullptr),
    db(nullptr),
    player(nullptr),
    qyery(nullptr),
    buffer(nullptr)
{
    node = new Node();
    player = new QMediaPlayer(nullptr,QMediaPlayer::LowLatency);
    buffer = new QBuffer;
    if(!player->isAvailable()){
        throw MediaException();
    }

    initDB();

    connect(node,SIGNAL(Message(ETcpSocket*)),SLOT(packageRender(ETcpSocket*)));
    connect(&deepScaner,SIGNAL(scaned(QList<ETcpSocket*>*)),SLOT(deepScaned(QList<ETcpSocket*>*)));

}

void Sync::sqlErrorLog(const QString &qyery){
#ifdef QT_DEBUG
            qDebug()<< qyery << ": fail:\n " <<this->qyery->lastError();
#endif
}

void Sync::initDB(){
    if(db) return;
    db = new QSqlDatabase();
    *db = QSqlDatabase::addDatabase("QSQLITE");
    QDir d(QString("./%0").arg(DATABASE_NAME));
    db->setDatabaseName(d.absolutePath());
    if(db->open()){
        qyery = new QSqlQuery(*db);
        QString qyer = QString("CREATE TABLE IF NOT EXISTS %0"
                     "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
                     "name VARCHAR(100), "
                     "size INT NOT NULL, "
                     "data BLOB NOT NULL)").arg(DATATABLE_NAME);
        if(!qyery->exec(qyer)){
            sqlErrorLog(qyer);
            throw InitDBError();
        }

        qyer = QString("CREATE UNIQUE INDEX IF NOT EXISTS i%0 ON %0(name,size)").arg(DATATABLE_NAME);
        if(!qyery->exec(qyer)){
            sqlErrorLog(qyer);
            throw InitDBError();
        }
    }
}

int Sync::save(const Song &song){
    QString qyer = QString("SELECT id from %0 where name='%1' and size=%2").arg(DATATABLE_NAME,
                                                 song.name,
                                                 QString::number(song.size));
    if(!qyery->exec(qyer)){
        sqlErrorLog(qyer);
        return -1;
    }
    if(qyery->next()){
        return qyery->value(0).toInt();
    }

    qyer = QString("INSERT INTO %0 (name,size,data) VALUES"
                           "('%1',%2,:val)").arg(DATATABLE_NAME,
                                                 song.name,
                                                 QString::number(song.size));
    if(!qyery->prepare(qyer)){
        sqlErrorLog(qyer + " prepare error");
        return -1;
    }
    qyery->bindValue(":val",song.source);
    if(!qyery->exec()){
        sqlErrorLog(qyer);
        return -1;
    }
    if(!qyery->exec(QString("SELECT MAX(id) from %0").arg(DATATABLE_NAME))){
        sqlErrorLog(qyer);
        return -1;
    }
    if(!qyery->next())
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

    if(!qyery->next()){
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

bool Sync::play(const SongHeader &header, const Syncer *syncdata){
    QString qyer = QString("SELECT * from %0 where name=%1 and size=%2").arg(DATATABLE_NAME).arg(header.name).arg(header.size);
    if(!qyery->exec(qyer)){
        return false;
    }
    Song song;
    song.id = qyery->value(0).toInt();
    song.name = qyery->value(1).toString();
    song.size = qyery->value(2).toInt();
    song.source = qyery->value(3).toByteArray();
    return Sync::play(song,syncdata);
}

bool Sync::play(Song &song, Syncer *syncdata){
    //QBuffer buffer(&song.source);
    buffer->setData(song.source);
    buffer->open(QIODevice::ReadOnly);
    player->setMedia(QMediaContent(), buffer);


    fbroadcaster = !bool(syncdata);
    playList.push_front(static_cast<SongHeader&>(song));
    if(fbroadcaster){
        package pac;
        if(!createPackage(t_song_h | t_sync, pac)){
            throw CreatePackageExaption();
        }
        node->WriteAll(pac.parseTo());
    }

    if(syncdata && !sync(*syncdata)){
        return false;
    }

    player->play();

    return true;
}

bool Sync::play(int id_song, Syncer *syncdata){

    QString qyer = QString("SELECT * from %0 where id=%1").arg(DATATABLE_NAME).arg(id_song);
    if(!qyery->exec(qyer) || !qyery->next()){
        return false;
    }
    Song song;
    song.id = qyery->value(0).toInt();
    song.name = qyery->value(1).toString();
    song.size = qyery->value(2).toInt();
    song.source = qyery->value(3).toByteArray();
    return Sync::play(song,syncdata);
}

bool Sync::play(QString url){
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
    return Sync::play(song);
}

void Sync::pause(){
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

bool Sync::createPackage(Type type, package &pac){
    pac.clear();

    pac.type = type;

    if(type & TypePackage::t_sync && fbroadcaster){

        pac.playdata.run = now() + SYNC_TIME;
        pac.playdata.seek = player->position() + SYNC_TIME;

    }

    if(type & TypePackage::t_song_h && fbroadcaster){
        if(playList.isEmpty())
            return false;

        pac.header = playList.front();

    }

    if(type & TypePackage::t_song && fbroadcaster){
        if(playList.isEmpty())
            return false;

        if(!load(playList.front(), pac.source))
            return false;

    }

    if(fbroadcaster)
        pac.type = TypePackage(pac.type | t_brodcaster);

    return pac.isValid();
}

void Sync::packageRender(ETcpSocket *socket){

    QByteArray *array;
    while((array = socket->topStack())){
        package pkg;
        if(!pkg.parseFrom((*array))){
            throw BadAnswerExaption();
            continue;
        }
//     package answer;

//     scaning servers

        if(pkg.getType() & t_brodcaster && servers.indexOf(socket) == -1){
            servers.append(socket);
        }

        if(!(pkg.getType() & t_brodcaster) && servers.indexOf(socket) != -1){
            servers.removeOne(socket);
        }

        if(fbroadcaster == (pkg.getType() & t_brodcaster)){
            throw BrodcastConflict();
            return;
        }

        if(pkg.getType() & t_brodcaster){

//            if requst from server

            if(pkg.getType() & t_play){
                player->play();
            }

            if((pkg.getType() & t_song_h) && !play(pkg.getHeader(), &pkg.getPlayData())){
                if((pkg.getType() & t_song) && !play(pkg.getSong(), &pkg.getPlayData())){
                    package answer;
                    if(!createPackage(t_song | t_sync, answer)){
                        throw CreatePackageExaption();
                    }
                    socket->Write(answer.parseTo());

                }
            }

            if(pkg.getType() & t_close){
                socket->getSource()->close();
                node->getClients()->removeOne(socket);
                delete socket;
            }

            if(pkg.getType() & t_what){
                package answer;
                if(!createPackage(t_void, answer)){
                    throw CreatePackageExaption();
                }
                socket->Write(answer.parseTo());
            }

            if(pkg.getType() & t_sync){
                if(playList.empty()){
                    throw SyncError();
                }
            }

        }else{

            package answer;
            if(!createPackage(pkg.getType() | ~t_what | ~t_play | ~t_stop | ~t_brodcaster, answer)){
                throw CreatePackageExaption();
            }
            socket->Write(answer.parseTo());

            if(pkg.getType() & t_close){
                socket->getSource()->close();
                node->getClients()->removeOne(socket);
                delete socket;
            }

        }

        array->clear();
        delete array;

    }
}

void Sync::rescan(bool deep){
    package pac;
    if(!createPackage(t_what,pac)){
        throw CreatePackageExaption();
        return;
    }
    node->WriteAll(pac.parseTo());

    if(deep){
       deepScaner.setInterval(DEEP_SCANER_INTERVAL);
       deepScaner.scane();
    }
}

void Sync::deepScaned(QList<ETcpSocket *> * list){
    package pac;
    if(!createPackage(t_what,pac)){
        throw CreatePackageExaption();
        return;
    }
    QByteArray array = pac.parseTo();
    for(ETcpSocket * i: *list){
        i->Write(array);
    }
}

Sync::~Sync(){
    delete node;
    delete db;
    delete player;
    servers.clear();
}

}


