include($$PWD/deploy/targetList.pri)

TEMPLATE = aux

INSTALLER = installerApp

INPUT = $$PWD/config/config.xml $$PWD/packages
installerApp.input = INPUT
installerApp.output = $$INSTALLER

QT_DIR = $$dirname(QMAKE_QMAKE)
LUPDATE = $$QT_DIR/lupdate
LRELEASE = $$QT_DIR/lrelease

OUT_FILE = SoundBandInstaller

QT_DIR = $$dirname(QMAKE_QMAKE)
QML_DIR = $$PWD/../SoundBand


WINDEPLOY = $$QT_DIR/windeployqt.exe
WINDEPLOY_RUN = $$PWD/deploy/deploy.bat

LINUXDEPLOY = $$PWD/../CQtDeployer/build/cqtdeployer

win32 {
    OUT_FILE = SoundBandInstaller.exe
    LUPDATE = $$QT_DIR/lupdate.exe
    LRELEASE = $$QT_DIR/lrelease.exe

}

#prepare start
win32 {
    installerApp.commands += $$WINDEPLOY_RUN $$WINDEPLOY $$QML_DIR $$shell_path($$PWD) &&
}
#prepare end

unix {
    for(command, TARGET_LIST) {
        installerApp.commands += $$LINUXDEPLOY -bin $$DESTDIR/$$command clear -qmlDir $$QML_DIR -qmake $$QMAKE_QMAKE &&
    }
}

installerApp.commands += $$QT_DIR/../../../Tools/QtInstallerFramework/3.0/bin/binarycreator --offline-only -c $$PWD/config/config.xml -p $$PWD/packages $$PWD/$$OUT_FILE --verbose -f
installerApp.CONFIG += target_predeps no_link combine

message( installComands = "$$installerApp.commands")

commands += "$$LUPDATE $$PWD/packages/SoundBand/meta/installscript.js -ts $$PWD/packages/SoundBand/meta/ru.ts"
commands += "$$LRELEASE $$PWD/packages/SoundBand/meta/ru.ts"
commands += "$$LUPDATE $$PWD/packages/SoundBand.Tests/meta/installscript.js -ts $$PWD/packages/SoundBand.Tests/meta/ru.ts"
commands += "$$LRELEASE $$PWD/packages/SoundBand.Tests/meta/ru.ts"

for(command, commands) {
    system($$command)|error("Failed to run: $$command")
}


QMAKE_EXTRA_COMPILERS += installerApp

DISTFILES += \
    config/controlScript.js \
    config/config.xml \
    README.md \
    config/ru.ts \
    packages/SoundBand/meta/installscript.js \
    packages/SoundBand/meta/package.xml \
    packages/SoundBand/meta/ru.ts \
    packages/SoundBand.Tests/meta/installscript.js \
    packages/SoundBand.Tests/meta/package.xml \
    packages/SoundBand.Tests/meta/ru.ts \
    deploy/deploy.bat

