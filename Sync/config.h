#ifndef CONFIG_H
#define CONFIG_H

// general otions
#define CURRENT_PLAYLIST_KEY        "currentPlayList"
#define MAIN_FOLDER_KEY             "songsfolder"
#define MAIN_FOLDER                 QDir::homePath() + "/soundBand"

// LIB VERSION
#define MAJOR_VERSION               0
#define MINOR_VERSION               1
#define REVISION_VERSION            1

// sqlite config
#define DATABASE_NAME               "songdata.dat"
#define ALL_SONGS_LIST              "all songs"

// network config
#define DEFAULT_ADRESS              "DEFAULT_ADRESS"
#define DEFAULT_NRTWORK             0           // the network number from which the address will be taken.
#define LOCAL_HOST                  "127.0.0.1"
#define DEFAULT_PORT                1994
#define RESYNC_TIME                 1000        // 1 sec on millisec
#define MAX_RESYNC_COUNT            3
#define SYNC_TIME                   5 * 1000    // 5 sec on millisec
#define SYNC_COUNT                  20
#define DEEP_SCANER_INTERVAL        1000        // 1 sec

// sync
#define MIN_DIFFERENCE              10          // millisec

//debug
#ifndef  QT_DEBUG
    #define LOGER
#endif

#define LOGER                                   // define this macros if want use logs
#define LOG_FILE                    MAIN_FOLDER + "/logs.log"
#endif // CONFIG_H
