TEMPLATE = subdirs
CONFIG += ordered

include($$PWD/installer/deploy/deployFiles.pri)
message( DEPLOY_FILES_MASTER = $$DEPLOY_FILES)


SUBDIRS += SoundBand \
	tests


CONFIG(release, debug|release): {

    SUBDIRS += installer

} else {
    message( Selected Debug mode. The installer will not be created )

}
