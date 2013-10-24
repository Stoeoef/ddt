QT += widgets quick

######## Source and Header files
SOURCES += src/main.cpp \
    src/speakermodel.cpp \
    src/mainmodel.cpp \
    src/qlistqmlwrapper.cpp \
    src/debate.cpp \
    src/opdebate.cpp \
    src/bpsdebate.cpp \
    src/seattospeakermapper.cpp \
    src/tablenames.cpp \
    src/team.cpp \
    src/prototypeqlistqmlwrapper.cpp \
    src/matchingsolver.cpp \
    src/autocompletemodel.cpp \
    src/ddtconfiguration.cpp \
    src/mixedvaluespeakermodel.cpp \

HEADERS += \
    src/speakermodel.h \
    src/qlistqmlwrapper.h \
    src/opdebate.h \
    src/bpsdebate.h \
    src/debate.h \
    src/seattospeakermapper.h \
    src/tablenames.h \
    src/team.h \
    src/prototypeqlistqmlwrapper.h \
    src/mainmodel.h \
    src/matchingsolver.h \
    src/autocompletemodel.h \
    src/ddtconfiguration.h \
    src/mixedvaluespeakermodel.h \

######## External libraries
INCLUDEPATH += \
    $$PWD/deps/or-tools-read-only/src \
    $$PWD/deps/or-tools-read-only/dependencies/install/include \

unix: LIBS += \
    -L$$PWD/deps/or-tools-read-only/lib/ -lconstraint_solver -lbase -lutil -llinear_solver\
    -L$$PWD/deps/or-tools-read-only/dependencies/install/lib/ -lprotobuf -lCbc -lCbcSolver\
          -lgflags -lClp -lCgl -lCoinUtils -lOsi -lOsiClp\
    -L/lib/x86_64-linux-gnu/ -lz \

win32: LIBS += \
    -L$$PWD/deps/or-tools-read-only/lib/ -lconstraint_solver -lbase -lutil -llinear_solver\
    -L$$PWD/deps/or-tools-read-only/dependencies/install/lib/ -llibprotobuf -llibgflags \
    -L$$PWD/deps/or-tools-read-only/dependencies/install/lib/coin -llibCbc -llibCbcSolver -llibCgl -llibClp -llibCoinUtils -llibOsi -llibOsiClp \
    -L/lib/x86_64-linux-gnu/ -lzlib \
    -LC:/Windows/SysWOW64&/ -lws2_32 \

unix: TARGET = ddtexe
win32: TARGET = ddt

######## Install target
# platform independent definitions
installPath = $$PWD/../ddt_install

copyother.path = $$installPath
copyother.files += misc/options.xml
copyother.files += misc/LICENSE

copyQtQuick.path = $$installPath/qml/
copyQtQuick.files += $$(QTDIR)/qml/QtQuick.2

copyQmlPlugins.path = $$installPath/qml/QtQuick
copyQmlPlugins.files += $$(QTDIR)/qml/QtQuick/Controls \
                        $$(QTDIR)/qml/QtQuick/Layouts \
                        $$(QTDIR)/qml/QtQuick/Window.2

copyPlatform.path = $$installPath/plugins/platforms
copyImageFormats.path = $$installPath/plugins/imageformats

copylang.path = $$installPath/languages
copylang.files = languages/*.ts

copyqml.path = $$installPath
copyqml.files = qml

copyexecutable.path = $$installPath

# unix install definitions
unix: {
    copyother.files += misc/unixdeploy/*
    qtLibPath = $$(QTDIR)/lib
    copyqtlibs.path = $$installPath/lib
    copyqtlibs.files+=  $$qtLibPath/libicudata.so* \
                        $$qtLibPath/libicui18n.so* \
                        $$qtLibPath/libicuuc.so* \
                        $$qtLibPath/libQt5Core.so.5* \
                        $$qtLibPath/libQt5Gui.so.5* \
                        $$qtLibPath/libQt5Widgets.so.5* \
                        $$qtLibPath/libtQt5Network.so.5* \
                        $$qtLibPath/libQt5Qml.so.5* \
                        $$qtLibPath/libQt5Quick.so.5* \
                        $$qtLibPath/libQt5V8.so.5* \
                        $$qtLibPath/libQt5Widgets.so.5* \
                        $$qtLibPath/libQt5Svg.so.5* \
                        $$qtLibPath/libQt5DBus.so.5*

    copyPlatform.files += $$(QTDIR)/plugins/platforms/libqxcb.so
    copyImageFormats.files += $$(QTDIR)/plugins/imageformats/libqsvg.so
    copyexecutable.files = $$TARGET
    copyexecutable.commands += chmod +x $$copyexecutable.path/ddt.sh
}

# windows install definitions
win32: {
    copyother.files += misc/windeploy/*
    qtLibPath = $$(QTDIR)/bin
    copyqtlibs.path = $$installPath/
    copyqtlibs.files+=  $$qtLibPath/d3dcompiler_46.dll \
                        $$qtLibPath/icuin51.dll \
                        $$qtLibPath/icuuc51.dll \
                        $$qtLibPath/icudt51.dll \
                        $$qtLibPath/libEGL.dll \
                        $$qtLibPath/libGLESv2.dll \
                        $$qtLibPath/Qt5Core.dll \
                        $$qtLibPath/Qt5Gui.dll \
                        $$qtLibPath/Qt5Network.dll \
                        $$qtLibPath/Qt5Qml.dll \
                        $$qtLibPath/Qt5Quick.dll \
                        $$qtLibPath/Qt5V8.dll \
                        $$qtLibPath/Qt5Widgets.dll \
                        $$qtLibPath/Qt5Svg.dll

    copyPlatform.files += $$(QTDIR)/plugins/platforms/qwindows.dll
    copyImageFormats.files += $$(QTDIR)/plugins/imageformats/qsvg.dll
    copyexecutable.files = $$PWD/release/$$sprintf($$TARGET%1, .exe)
}

INSTALLS += copyother copyqtlibs copyQmlPlugins copyQtQuick copyqml copyexecutable copylang copyPlatform copyImageFormats

######## Translations
TRANSLATIONS = \
    languages/en.ts \
    languages/de.ts

lupdate_only {
    SOURCES = qml/ddt/*.qml
}

######## Misc
QMAKE_CXXFLAGS += -std=c++11

OTHER_FILES += \
    options.xml \
    ddt.qmlproject \
    README.md \
    LICENSE \


OBJECTS_DIR = ./.obj

MOC_DIR = ./.moc
