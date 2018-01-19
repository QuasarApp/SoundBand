#ifndef SYNCENGINE_H
#define SYNCENGINE_H
#include "../sync/sync.h"
#include <QObject>
#include <QPixmap>


/**
 * @brief The SyncEngine class - this class is interface between syncLine and qml application.
 */
class SyncEngine : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString lastError READ lastError NOTIFY error)
    Q_PROPERTY(double pos READ pos NOTIFY posChanged)
    Q_PROPERTY(int repeat READ repeat WRITE setRepeat NOTIFY repeatChanged)

    enum Repeat{noRepeat, oneMusic, allPlayList};

private:
    syncLib::Sync *sync;
    syncLib::MySql * sqlApi;
    QString _curentPlayListName;
    QStringList tempList;
public:
    SyncEngine();
    ~SyncEngine();
public slots:
    /**
     * @brief curentSong
     * @return  curent song name
     */
    const QString& curentSong()const;

    /**
     * @brief curentPlayList
     * @return return curent Play List
     */
    const QStringList &curentPlayList()const;

    /**
     * @brief curentPlayListName
     * @return name of curent play list
     */
    const QString& curentPlayListName()const;

    /**
     * @brief allPlayLists
     * @return names of all play lists
     */
    const QStringList &allPlayLists()const;

    /**
     * @brief curentSongImage
     * @return Image of curent song
     */
    const QPixmap& curentSongImage()const;

    /**
     * @brief songImageById
     * @param id - id of playingSong;
     * @return
     */
    const QPixmap& songImageById(int id)const;

    /**
     * @brief play - play curent music
     * @return true if all done.
     */
    bool play();

    /**
     * @brief pause - set pause for curent song
     * @return true if all done.
     */
    bool pause();

    /**
     * @brief next - switch to next music
     * @return true if all done.
     */
    bool next();

    /**
     * @brief prev - switch to prev music
     * @return true if all done.
     */
    bool prev();

    /**
     * @brief listen corent server
     * @param host of server
     * @return true if all done
     */
    bool listen(const QString &host);

    /**
     * @brief repeat
     * @return flag of Repeat
     */
    int repeat()const;

    /**
     * @brief setRepeat
     * @param flag
     */
    void setRepeat(int flag);

    /**
     * @brief setPlayList - select new play list.
     * @param id of selected play list
     * @return true if all done
     */
    bool setPlayList(int id);

    /**
     * @brief setPlayList - select new play list.
     * @param name of selected play list
     * @return true if all done
     */
    bool setPlayList(QString name);

    /**
     * @brief lastError - message of last error
     * @return  message of last error
     */
    const QString& lastError()const;

    /**
     * @brief pos
     * @return posistion of curent song on double
     */
    double pos()const;

signals:

    /**
     * @brief curentSongChanged
     * This signal can be emitted when curent Music changed
     */
    void curentSongChanged();

    /**
     * @brief error
     * This signal can be emitted when error has been detected
     */
    void error();

    /**
     * @brief posChanged
     * This signal can be emitted when position of curent song changed
     */
    void posChanged();

    /**
     * @brief repeatChanged
     * This signal can be emitted when repeat state changed
     */
    void repeatChanged();
};

#endif // SYNCENGINE_H
