#ifndef SYNC_GLOBAL_H
#define SYNC_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SYNC_LIBRARY)
#  define SYNCSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SYNCSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // SYNC_GLOBAL_H
