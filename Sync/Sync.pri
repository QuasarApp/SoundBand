#
# Copyright (C) 2018 QuasarApp.
# Distributed under the lgplv3 software license, see the accompanying
# Everyone is permitted to copy and distribute verbatim copies
# of this license document, but changing it is not allowed.
#

!isEmpty(SYNC_LIB):error("Sync.pri already included")
SYNC_LIB = 1

#DEPENDS
OLD_DEST_DIR=$$DESTDIR
OLD_SOUND=$$SOUND_BAND_MODULE;

SOUND_BAND_MODULE = sync
include($$PWD/../deploy.pri)

LIBS += -L"$$DESTDIR/" -lSync

SOUND_BAND_MODULE = $$OLD_SOUND
DESTDIR = $$OLD_DEST_DIR

INCLUDEPATH += "$$PWD/"
