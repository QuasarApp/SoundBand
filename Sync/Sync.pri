#
# Copyright (C) 2018 QuasarApp.
# Distributed under the lgplv3 software license, see the accompanying
# Everyone is permitted to copy and distribute verbatim copies
# of this license document, but changing it is not allowed.
#

!isEmpty(SYNC_LIB):error("Sync.pri already included")
SYNC_LIB = 1
include($$PWD/../installer/deploy/deployFiles.pri)

#DEPENDS

LIBS += -L"$$DESTDIR/" -lSync

INCLUDEPATH += "$$PWD/"
