TEMPLATE = aux

SNAPCRAFT = snapcraft

DESTDIR = $$PWD/packages/SoundBand

INPUT = $$PWD/snap/snapcraft.yaml
snapApp.input = INPUT
snapApp.output = $$SNAPCRAFT

unix {
    message($$DESTDIR);

    snapApp.commands += $$SNAPCRAFT
    snapApp.CONFIG += target_predeps no_link combine

    message( snapComands = "cd $$DESTDIR && $$snapApp.commands")
}


QMAKE_EXTRA_COMPILERS += snapApp

DISTFILES += \

unix:extraclean.commands = $$SNAPCRAFT clean soundband

distclean.depends = extraclean
QMAKE_EXTRA_TARGETS += distclean extraclean
