#include "sync.h"
#include <QtSql>
#include <QMultimedia>
#include <QSqlQuery>
#include "exaptions.h"
#include <ctime>
#include <thread>
#include <cmath>
#include "config.h"

#ifdef QT_DEBUG
#include <QDebug>
#include <iostream>
#endif

namespace syncLib{

Sync::Sync(const QString address, int port, const QString &datadir):
    node(nullptr),
    db(nullptr),
    player(nullptr),
    qyery(nullptr),
    buffer(nullptr),
    curentSong(nullptr)
{
    node = new Node(address , this->port = port);

    player = new QMediaPlayer(nullptr,QMediaPlayer::LowLatency);
    buffer = new QBuffer;
    if(!player->isAvailable()){
        throw MediaException();
    }

    fbroadcaster = false;

    initDB(datadir);

    connect(node, SIGNAL(Message(ETcpSocket*)), SLOT(packageRender(ETcpSocket*)));
    connect(&deepScaner, SIGNAL(scaned(QList<ETcpSocket*>*)), SLOT(deepScaned(QList<ETcpSocket*>*)));
    connect(player, SIGNAL(positionChanged(qint64)), SIGNAL(seekChanged(qint64)));
    connect(player, SIGNAL(stateChanged(QMediaPlayer::State)), SLOT(endPlay(QMediaPlayer::State)));
}

int Sync::abs(int number) const{
    return (number << 1) >> 1;
}

bool Sync::findHeader(const Song &song){

    for(SongHeader & header: playList){
        if(header == static_cast<SongHeader>(song)){
            curentSong = &header;
            return true;
        }
    }

    return false;
}

void Sync::sqlErrorLog(const QString &qyery){
#ifdef QT_DEBUG
            qDebug()<< qyery << ": fail:\n " <<this->qyery->lastError();
#endif
}

void Sync::initDB(const QString &database){
    if(db) return;
    dataBaseName = database;
    db = new QSqlDatabase();
    *db = QSqlDatabase::addDatabase("QSQLITE", database);
    QDir d(QString("./%0").arg(dataBaseName));
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
            delete db;
            return;
        }

        qyer = QString("CREATE UNIQUE INDEX IF NOT EXISTS i%0 ON %0(name,size)").arg(DATATABLE_NAME);
        if(!qyery->exec(qyer)){
            sqlErrorLog(qyer);
            throw InitDBError();
            delete db;
            return;
        }
    }
    updateAvailableSongs();
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

    int result = qyery->value(0).toInt();
    updateAvailableSongs();
    return result;
}

bool Sync::updateAvailableSongs(){
    QString qyer = QString("SELECT id,name,size from %0").arg(DATATABLE_NAME);
    if(!qyery->exec(qyer)){
        sqlErrorLog(qyer);
        return false;
    }

    playList.clear();

    while(qyery->next()){
        SongHeader song;
        song.id = qyery->value(0).toInt();
        song.name = qyery->value(1).toString();
        song.size = qyery->value(2).toInt();
        playList.push_back(song);
    }

    return true;
}

bool Sync::load(const SongHeader &song,Song &result){
    result.clear();
    if(song.id > -1){
        QString qyer = QString("SELECT * from %0 where id=%1").arg(DATATABLE_NAME).arg(song.id);
        if(!qyery->exec(qyer)){
            return false;
        }
    }else if(!song.name.isEmpty() && song.size > 0){
        QString qyer = QString("SELECT * from %0 where name='%1' and size=%2").arg(DATATABLE_NAME).arg(song.name).arg(song.size);
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

    if(!header.isValid()){
        return false;
    }

    QString qyer = QString("SELECT * from %0 where name='%1' and size=%2").arg(DATATABLE_NAME).arg(header.name).arg(header.size);
    if(!qyery->exec(qyer)){
        sqlErrorLog(qyer);
        return false;
    }

    if(!qyery->next()){
        return false;
    }

    Song song;
    song.id = qyery->value(0).toInt();
    song.name = qyery->value(1).toString();
    song.size = qyery->value(2).toInt();
    song.source = qyery->value(3).toByteArray();
    return Sync::play(song, syncdata);
}

bool Sync::play(const Song &song, const Syncer *syncdata){

    if(!song.isValid()){
        return false;
    }

    buffer->close();
    buffer->setData(song.source);
    buffer->open(QIODevice::ReadOnly);
    player->setMedia(QMediaContent(), buffer);

    fbroadcaster = !bool(syncdata);
    if(!findHeader(song) && save(song) > -1 && !findHeader(song)){
        return false;
    }

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
    return Sync::play(song ,syncdata);
}

bool Sync::play(QString url){
    int id = addNewSong(url);
    if(id < 0){
        return false;
    }
    return Sync::play(id);
}

void Sync::pause(bool state){
    if(state){
        player->pause();
    }else{
        player->play();
    }
}

void Sync::stop(){
    buffer->close();
    player->stop();
}

void Sync::jump(const qint64 seek){
    player->setPosition(seek);
}

bool Sync::sync(const Syncer &sync){
    milliseconds sync_time  = sync.run - now();
    if(sync_time > MAX_SYNC_TIME && sync_time <= 0)
        return false;
    do {
        std::this_thread::yield();
    } while (now() < sync.run);
    player->setPosition(sync.seek);
    return true;
}

bool Sync::addNode(const QString ip, int port){
     if(!node->addNode(ip, port))
         return false;

     rescan();
     return true;
}

void Sync::scan(){

    rescan(true);
}

const QList<ETcpSocket*>& Sync::getServersList() const{
    return servers;
}

bool Sync::listen(ETcpSocket *server){
    if(!server){
        return false;
    }

    if(!server->getSource()->isOpen() && server->getSource()->open(QIODevice::ReadWrite)){
        return false;
    }
    package pac;

    if(!createPackage(t_sync,pac)){
        return false;
    }

    return server->Write(pac.parseTo());
}

bool Sync::createPackage(Type type, package &pac){
    pac.clear();

    pac.type = type;

    if(type & TypePackage::t_sync  && fbroadcaster){

        pac.playdata.run = now() + SYNC_TIME;
        pac.playdata.seek = player->position() + SYNC_TIME;

    }

    if( type & TypePackage::t_feedback && !fbroadcaster){

        pac.playdata.run = now();
        pac.playdata.seek = player->position();

    }

    if(type & TypePackage::t_song_h && fbroadcaster){
        if(!curentSong)
            return false;

        pac.header = *curentSong;

    }

    if(type & TypePackage::t_song && fbroadcaster){
        if(!curentSong)
            return false;

        if(!load(*curentSong, pac.source))
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
            emit networkStateChange();
        }

        if(!(pkg.getType() & t_brodcaster) && servers.indexOf(socket) != -1){
            servers.removeOne(socket);
            emit networkStateChange();
        }

        if(fbroadcaster == (pkg.getType() & t_brodcaster)){
            throw BrodcastConflict();
            return;
        }

        if(pkg.getType() & t_brodcaster){

//            if requst from server

            if(pkg.getType() & t_sync){
                if(!play(pkg.getHeader(), &pkg.getPlayData()) && !play(pkg.getSong(), &pkg.getPlayData())){

                    Type requestType = t_song_h;

                    if(pkg.getType() & t_song_h)
                        requestType = t_song;

                    package answer;
                    if(!createPackage(requestType | t_sync, answer)){
                        throw CreatePackageExaption();
                    }
                    socket->Write(answer.parseTo());
                }else{
                    package feedback;

                    if(!createPackage(t_feedback, feedback)){
                        throw feedbackError();
                    }
                    socket->Write(feedback.parseTo());
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


        }else{

            if(pkg.getType() & t_sync ){
                if(!curentSong){
                    return ;
                }
            }

            Type responceType = pkg.getType();
            if(pkg.getType() & t_feedback){
                if(!curentSong){
                    return ;
                }

                unsigned int diff = abs(static_cast<unsigned int>(player->position() - (pkg.getPlayData().seek + (now() - pkg.getPlayData().run))));

#ifdef QT_DEBUG
                qDebug() << "diff " << socket->name() <<": " << diff;
#endif

                if(diff > 1){
                    responceType = responceType | t_sync;
                }else{
                    std::cout<<"Synced!!"<<std::endl;
                }
            }

            package answer;
            if(!createPackage(responceType & ~t_what & ~t_feedback  & ~t_stop & ~t_brodcaster, answer)){
                throw CreatePackageExaption();
            }
            socket->Write(answer.parseTo());

            if(pkg.getType() & t_close){
                socket->getSource()->close();
                node->getClients()->removeOne(socket);
                delete socket;
            }

        }

        socket->nextItem();
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
       deepScaner.scane(port);
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
        node->addNode(i);
        i->Write(array);
    }
}

void Sync::endPlay(QMediaPlayer::State state){
    if(state == QMediaPlayer::StoppedState){
        curentSong = nullptr;
        fbroadcaster = false;
    }
}

QString Sync::getVersion(){
    return QString(tr("Version") + "%0.%1.%2").arg(MAJOR_VERSION).arg(MINOR_VERSION).arg(REVISION_VERSION);
}

bool Sync::setValume(unsigned int valume){
    if(valume > 100)
        return false;

    player->setVolume(valume);

    return true;
}

unsigned int Sync::getValume() const{
    return player->volume();
}

unsigned int Sync::seek() const{
    return player->position();
}

const QList<SongHeader>* Sync::getPlayList() const{
    return &playList;
}

const SongHeader* Sync::getCurentSong() const{
    return curentSong;
}

int Sync::addNewSong(const QString &url){
    QFile f(url);
    if(!f.open(QIODevice::ReadOnly)){
        return -1;
    }
    QByteArray bytes = f.readAll();
    f.close();
    QString name = url.right(url.lastIndexOf(QRegularExpression("[\\/]"))); // meby [[\\\/]]
    Song song;
    song.name = name;
    song.size = bytes.size();
    song.source = bytes;

    return Sync::save(song);
}

qint64 Sync::getEndPoint() const {
    return player->duration();
}

Sync::~Sync(){
    delete node;
    delete db;
    delete player;
    servers.clear();
    QSqlDatabase::removeDatabase(dataBaseName);

}

}


