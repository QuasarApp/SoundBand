#ifndef CONFIG_H
#define CONFIG_H

// general otions

#define REALTIMESYNC                1 // realtime synced mod (1 if enabled, 0 disabled)

// LIB VERSION
#define MAJOR_VERSION               0
#define MINOR_VERSION               1
#define REVISION_VERSION            1

// sqlite config
#define DATABASE_NAME               "songdata.dat"
#define BUFFER_NAME                 "buffer"

// network config
#define DEFAULT_ADRESS              "DEFAULT_ADRESS"
#define DEFAULT_NRTWORK             0         // the network number from which the address will be taken.
#define LOCAL_HOST                  "127.0.0.1"
#define DEFAULT_PORT                1994
#define MAX_SYNC_TIME               20 * 1000 // 10 sec on millisec
#define SYNC_TIME                   5 * 1000  // 5 sec on millisec
#define DEEP_SCANER_INTERVAL        10000     // 10 sec
#define CHECK_PING_INTERVAL         5 * 60 *1000    // 5 minutes

// sync
#define MIN_DIFFERENCE              10       // millisec

#endif // CONFIG_H
