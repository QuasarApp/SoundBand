QT += network multimedia sql testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app
TARGET = test

CONFIG(release, debug|release): {
    DESTDIR = $$PWD/build/release

} else {
    DESTDIR = $$PWD/build/debug
}

SOURCES += \
    tst_synctest.cpp


RESOURCES += \
    res.qrc

include($$PWD/../Sync/Sync.pri)
include('$$PWD/../QuasarAppLib/QuasarLib.pri');
