QT += network multimedia sql testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_synctest.cpp \
    ../ETcpSocket.cpp \
    ../LocalScanner.cpp \
    ../node.cpp \
    ../song.cpp \
    ../sync.cpp \
    ../player.cpp

RESOURCES += \
    res.qrc

HEADERS += \
    ../config.h \
    ../ETcpSocket.h \
    ../exaptions.h \
    ../LocalScanner.h \
    ../node.h \
    ../song.h \
    ../sync.h \
    ../player.h
