#ifndef SYNC_H
#define SYNC_H
/*
data=QSqlDatabase::addDatabase("QSQLITE");
   QDir d("./Inventory");
   data.setDatabaseName(d.absolutePath());
   qDebug()<<d.absolutePath()<<data.open();
*/
class QString;
class QSqlDatabase;
class QMediaPlayer;
class QSqlQuery;
namespace syncLib {

class Song;
class Node;
class Sync
{
private:
    Node *node;
    QSqlDatabase *db;
    QMediaPlayer *player;
    QSqlQuery *qyery;
    void initDB();
    bool save(const Song &song);
public:
    bool Play(QString url) const;
    void Pause() const;
    void stop() const;
    void jump(const int seek) const;
    Sync();
    ~Sync();
};
}


#endif // SYNC_H
