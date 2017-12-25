#include "sync.h"
#include <QtSql>
#include <QMultimedia>
#include <QSqlQuery>
#include "exaptions.h"
#include "thread"
#include "chronotime.h"

#ifdef QT_DEBUG
#include <QDebug>
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

    player = new Player(BUFFER_NAME,nullptr,QMediaPlayer::LowLatency);
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

    if(!player->setMediaFromBytes(song.source)){
        return false;
    }

    fbroadcaster = !bool(syncdata);
    if(!findHeader(song) && save(song) > -1 && !findHeader(song)){
        return false;
    }

    if(fbroadcaster){
        package pac;
        if(!createPackage(t_play, pac)){
            throw CreatePackageExaption();
        }
        node->WriteAll(pac.parseTo());
    }

    if(syncdata){
        return sync(*syncdata);
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
    milliseconds sync_time  = sync.run - ChronoTime::now();
    if(sync_time > MAX_SYNC_TIME && sync_time <= 0)
        return false;

    milliseconds delay = player->getPlayDelay();
    if(delay < 0)
        return false;

    player->setPosition(sync.seek + delay);

    QTimer::singleShot(sync_time, [=]() {
        player->play();
    });

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

    if(!createPackage(t_sync, pac)){
        return false;
    }

    return server->Write(pac.parseTo());
}

bool Sync::createPackage(Type type, package &pac, const ETcpSocket *for_){
    pac.clear();

    pac.type = type;

    if(type & TypePackage::t_sync && fbroadcaster){

        if(!for_  || !for_->isSynced()){
            return false;
        }

        pac.playdata.run = ChronoTime::now(for_->getDifferenceTime()) + SYNC_TIME;
        pac.playdata.seek = player->position() + SYNC_TIME;

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
            socket->nextItem();
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
            socket->nextItem();
            continue;
        }

        if(pkg.getType() & t_brodcaster){

//            if requst from server

            if(pkg.getType() & t_play){

                package answer;
                if(!createPackage(t_sync, answer)){
                    throw CreatePackageExaption();
                }
                socket->Write(answer.parseTo());
            }

            if(pkg.getType() & t_sync && !play(pkg.getHeader(), &pkg.getPlayData()) && !play(pkg.getSong(), &pkg.getPlayData())){

                Type requestType = t_song_h;

                if(pkg.getType() & t_song_h)
                    requestType = t_song;

                package answer;
                if(!createPackage(requestType | t_sync, answer)){
                    throw CreatePackageExaption();
                    socket->nextItem();
                    continue;
                }
                socket->Write(answer.parseTo());
            }

            if(pkg.getType() & t_close){
                socket->getSource()->close();
                node->getClients()->removeOne(socket);
                delete socket;
                return;
            }

            if(pkg.getType() & t_what){
                package answer;
                if(!createPackage(t_void, answer)){
                    throw CreatePackageExaption();
                    socket->nextItem();
                    continue;
                }
                socket->Write(answer.parseTo());
            }


        }else{

            if(pkg.getType() & t_sync){
                if(!curentSong){
                    throw SyncError();
                    socket->nextItem();
                    continue;
                }

                if(!socket->isSynced()){
                    socket->calibration();
                }
            }

            package answer;
            if(!createPackage(pkg.getType() & ~t_what & ~t_play & ~t_stop & ~t_brodcaster, answer, socket)){
                throw CreatePackageExaption();
                socket->nextItem();
                continue;
            }
            socket->Write(answer.parseTo());

            if(pkg.getType() & t_close){
                socket->getSource()->close();
                node->getClients()->removeOne(socket);
                delete socket;
                return;
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


