QT += quick core gui network multimedia sql
android: QT += androidextras
mac: QT += macextras
CONFIG += c++14

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp \
    syncengine.cpp \
    ../sync/chronotime.cpp \
    ../sync/ETcpSocket.cpp \
    ../sync/LocalScanner.cpp \
    ../sync/mysql.cpp \
    ../sync/node.cpp \
    ../sync/player.cpp \
    ../sync/song.cpp \
    ../sync/sync.cpp \
    ../sync/Log.cpp\
    ../sync/exaptions.cpp


RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

HEADERS += \
    syncengine.h \
    ../sync/chronotime.h \
    ../sync/config.h \
    ../sync/ETcpSocket.h \
    ../sync/exaptions.h \
    ../sync/LocalScanner.h \
    ../sync/mysql.h \
    ../sync/node.h \
    ../sync/player.h \
    ../sync/song.h \
    ../sync/sync.h \
    ../sync/Log.h

