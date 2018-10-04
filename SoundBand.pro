
TEMPLATE = subdirs
CONFIG += ordered

message( DEPLOY_FILES_MASTER = $$DEPLOY_FILES)

ENABLE_SNAP = 0 #only linux
ENABLE_INSTALLER = 1 #only desctop

SUBDIRS += QuasarAppLib/QuasarApp.pro \
        Sync \
        SoundBand \
        tests

unix:!android:{
    SUBDIRS += CQtDeployer
}

SoundBand.depends = Sync QuasarAppLib/QuasarApp.pro

equals( ENABLE_INSTALLER, 1) {
    !android:{
        message(desktopVersion: enabled)
        CONFIG(release, debug|release): {

            SUBDIRS += installer

        } else {
            message( Selected Debug mode. The installer will not be created )

        }
    }
}

equals( ENABLE_SNAP, 1) {
    !android:{
        message(desktopVersion: enabled)
        CONFIG(release, debug|release): {

            SUBDIRS += installer/packages/SoundBand/snap.pro

        } else {
            message( Selected Debug mode. The snap will not be created )

        }
    }
}
