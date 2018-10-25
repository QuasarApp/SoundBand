#
# Copyright (C) 2018 QuasarApp.
# Distributed under the lgplv3 software license, see the accompanying
# Everyone is permitted to copy and distribute verbatim copies
# of this license document, but changing it is not allowed.
#

!isEmpty(SYNC_LIB):error("Sync.pri already included")
SYNC_LIB = 1

#DEPENDS

CONFIG(release, debug|release): {
    LIBS += -L"$$PWD/build/release" -lSync

} else {
    LIBS += -L"$$PWD/build/debug" -lSync
}

INCLUDEPATH += "$$PWD/"
