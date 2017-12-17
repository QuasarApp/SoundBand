QT += network multimedia sql testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    tst_synctest.cpp \
    ../chronotime.cpp \
    ../ETcpSocket.cpp \
    ../LocalScanner.cpp \
    ../node.cpp \
    ../player.cpp \
    ../song.cpp \
    ../sync.cpp


RESOURCES += \
    res.qrc

HEADERS += \
    ../chronotime.h \
    ../config.h \
    ../ETcpSocket.h \
    ../exaptions.h \
    ../LocalScanner.h \
    ../node.h \
    ../player.h \
    ../song.h \
    ../sync.h


