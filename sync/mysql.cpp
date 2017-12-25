#include "mysql.h"
#include <QSqlQuery>
#include <QtSql>
#include "exaptions.h"

namespace syncLib{

MySql::MySql(const QString &databasename):
    db(nullptr),
    qyery(nullptr)
{
   initDB(databasename);
}

bool MySql::exec(QSqlQuery *sq,const QString& sqlFile){
    QFile f(sqlFile);
    bool result=true;
    if(f.open(QIODevice::ReadOnly)){
        QString temp,delimiter=";";
        QTextStream stream(&f);
        stream.setCodec("UTF8");
        while(!stream.atEnd()){
            temp+=stream.readLine();
            if(temp.lastIndexOf("delimiter",-1,Qt::CaseInsensitive)>-1){
                temp.remove("delimiter",Qt::CaseInsensitive);
                int last=temp.indexOf(QRegularExpression("[^ \f\n\r\t\v]"))+1;
                int begin=temp.lastIndexOf(QRegularExpression("[^ \f\n\r\t\v]"));
                delimiter=temp.mid(begin,last-begin);
                temp="";
            }else{
                if(temp.lastIndexOf(delimiter)>-1){
                    temp.remove(delimiter);
                    (result=result&&sq->exec(temp));
                    temp="";
                }
            }
        }
        return result;
    }
    return false;

}

void MySql::initDB(const QString &database){
    if(db) return;
    dataBaseName = database;
    db = new QSqlDatabase();
    *db = QSqlDatabase::addDatabase("QSQLITE", database);
    QDir d(QString("./%0").arg(dataBaseName));
    db->setDatabaseName(d.absolutePath());
    if(db->open()){
        qyery = new QSqlQuery(*db);
        QString qyer = QString("CREATE TABLE IF NOT EXISTS songs("
                     "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                     "name VARCHAR(100), "
                     "size INT NOT NULL, "
                     "data BLOB NOT NULL "
                     ")");
        if(!qyery->exec(qyer)){
            sqlErrorLog(qyer);
            throw InitDBError();
            delete db;
            return;
        }

        qyer = QString("CREATE UNIQUE INDEX IF NOT EXISTS isongs ON songs(name,size)");
        if(!qyery->exec(qyer)){
            sqlErrorLog(qyer);
            throw InitDBError();
            delete db;
            return;
        }

        qyer = QString("CREATE TABLE IF NOT EXISTS playlists("
                     "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                     "name VARCHAR(50) NOT NULL UNIQUE, "
                     "description VARCHAR(1000) DEFAULT 'without description', "
                     "image BLOB "
                     ")");
        if(!qyery->exec(qyer)){
            sqlErrorLog(qyer);
            throw InitDBError();
            delete db;
            return;
        }

        qyer = QString("CREATE TABLE IF NOT EXISTS playlistsdata("
                     "playlist INT NOT NULL,"
                     "song INT NOT NULL,"
                     "FOREIGN KEY(playlist) REFERENCES playlists(id)"
                        " ON UPDATE CASCADE"
                        " ON DELETE CASCADE,"
                     "FOREIGN KEY(song) REFERENCES songs(id)"
                        " ON UPDATE CASCADE"
                        " ON DELETE CASCADE"
                     ")");
        if(!qyery->exec(qyer)){
            sqlErrorLog(qyer);
            throw InitDBError();
            delete db;
            return;
        }

        qyer = QString("CREATE UNIQUE INDEX IF NOT EXISTS iplaylistsdata ON playlistsdata(playlist,song)");
        if(!qyery->exec(qyer)){
            sqlErrorLog(qyer);
            throw InitDBError();
            delete db;
            return;
        }
    }
}

void MySql::sqlErrorLog(const QString &qyery){
#ifdef QT_DEBUG
            qDebug()<< qyery << ": fail:\n " <<this->qyery->lastError();
#endif
}

int MySql::save(const Song &song){
    QString qyer = QString("SELECT id from songs where name='%0' and size=%1").arg(song.name,
                                                 QString::number(song.size));
    if(!qyery->exec(qyer)){
        sqlErrorLog(qyer);
        return -1;
    }

    if(qyery->next()){
        return qyery->value(0).toInt();
    }

    qyer = QString("INSERT INTO songs (name,size,data) VALUES"
                           "('%0',%1,:val)").arg(song.name,
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
    if(!qyery->exec(QString("SELECT MAX(id) from songs"))){
        sqlErrorLog(qyer);
        return -1;
    }
    if(!qyery->next())
        return -1;

    int result = qyery->value(0).toInt();
    return result;
}

int MySql::save(const QString &url){
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

    return save(song);

}

bool MySql::load(const SongHeader &song,Song &result){
    result.clear();
    if(song.id > -1){
        QString qyer = QString("SELECT * from songs where id=%0").arg(song.id);
        if(!qyery->exec(qyer)){
            return false;
        }
    }else if(!song.name.isEmpty() && song.size > 0){
        QString qyer = QString("SELECT * from songs where name='%0' and size=%1").arg(song.name).arg(song.size);
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

bool MySql::updateAvailableSongs(QList<SongHeader>& list, const QString& playList){
    QString qyer;
    if(playList.isEmpty()){
        qyer = QString("SELECT id,name,size from songs");
    }else{
        qyer = QString("SELECT id,name,size from songs where "
                       "id = (select song from playlistsdata where playlist='%0')").arg(playList);
    }

    if(!qyery->exec(qyer)){
        sqlErrorLog(qyer);
        return false;
    }

    list.clear();

    while(qyery->next()){
        SongHeader song;
        song.id = qyery->value(0).toInt();
        song.name = qyery->value(1).toString();
        song.size = qyery->value(2).toInt();
        list.push_back(song);
    }

    return true;
}

MySql::~MySql(){
    delete db;

    QSqlDatabase::removeDatabase(dataBaseName);

}
}

