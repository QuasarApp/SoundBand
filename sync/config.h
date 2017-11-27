#ifndef CONFIG_H
#define CONFIG_H

// LIB VERSION
#define MAJOR_VERSION               0
#define MINOR_VERSION               0
#define REVISION_VERSION            0

// sqlite config
#define DATABASE_NAME               "songdata.dat"
#define DATATABLE_NAME              "songs"

// network config
#define DEFAULT_PORT                1239
#define MAX_SYNC_TIME               20 * 1000 // 10 sec on microsec
#define SYNC_TIME                   5 * 1000  // 5 sec on microsec
#define DEEP_SCANER_INTERVAL        10000     // 10 sec

#endif // CONFIG_H
