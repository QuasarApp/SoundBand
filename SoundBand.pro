
TEMPLATE = subdirs
CONFIG += ordered

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

