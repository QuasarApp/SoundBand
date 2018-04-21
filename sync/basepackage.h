#ifndef BASEPACKAGE_H
#define BASEPACKAGE_H

#include "song.h"
#include "config.h"
#include "abstractpackage.h"

typedef unsigned char Type;

/**
 * @brief The TypePackage enum
 * t_void         =   this package empty and not valid.
 * t_play         =   play current audio file.
 * t_song_h       =   the header of playing audio file.
 * t_song         =   the package with this type is necessary for translite media data on network.
 * t_sync         =   the infomation about sync playning media file on network.
 * t_close        =   the information about close channel.
 * t_syncTime     =   getLocalTime of socket
 * t_what         =   request for information about the node
 * t_brodcaster   =   information about the node
*/

enum TypePackage{
     t_void         =   0x00,
     t_play         =   0x01,
     t_song_h       =   0x02,
     t_song         =   0x04,
     t_sync         =   0x08,
     t_close        =   0x10,
     t_syncTime     =   0x20,
     t_what         =   0x40,
     t_brodcaster   =   0x80
};

/**
 * @brief The package class. Package for translite media data on network
 *
 * parse map:
 * 1 byle - type
 * data
 */
class package : public AbstractPackage
{

private:
    Type type;
    Song source;
    SongHeader header;
    bool fbroadcaster;
    Syncer playdata;
    milliseconds time;
public:
    package();
    package(QByteArray &array);
    ~package();

    /**
     * @brief getHeader
     * @return Header of the song
     */
    const SongHeader& getHeader() const;

    /**
     * @brief getSong
     * @return Song
     */
    const Song& getSong() const;

    /**
     * @brief getPlayTime
     * @return time of playning media data
     */
    const Syncer &getPlayData() const;

    /**
     * @brief getType
     * @return type of package
     */
    const Type& getType() const;

    /**
     * @brief getTime
     * @return time of sended package pc
     */
    const milliseconds& getTime()const;

    /**
     * @brief isValid
     * @return true if package is valid
     */
    bool isValid() const;

    /**
     * @brief clear all date of package
     */
    void clear();

    /**
     * @brief parseTo parse this package to byte array
     * @return byte array
     */
    QByteArray parseTo();

    /**
     * @brief parseFrom create a package from bytes
     * @param array of bytes
     * @return true if package valid
     */
    bool parseFrom(QByteArray& array);
    friend class Sync;
};

#endif // BASEPACKAGE_H
