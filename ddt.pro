QT += widgets quick

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
# CONFIG += mobility
# MOBILITY +=

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

unix: TARGET = ddtexe
win32: TARGET = ddt

installPath = $$PWD/../ddt_install

copyother.path = $$installPath
copyother.files += misc/options.xml
copyother.files += misc/LICENSE

win32: copyother.files += misc/windeploy/*
unix: copyother.files += misc/unixdeploy/*

unix: {
    qtLibPath = $$(QTDIR)/lib
    copyqtlibs.path = $$installPath/lib
    copyqtlibs.files+=$$qtLibPath/libQt5Core.so.5.1.1
    copyqtlibs.files+=$$qtLibPath/libQt5Gui.so.5.1.1
    copyqtlibs.files+=$$qtLibPath/libQt5Widgets.so.5.1.1
}

win32: {
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
                        $$qtLibPath/Qt5Svg.dll \
}

unix: {
    copyorlibs.path=$$copyqtlibs.path
    copyorlibs.files+=$$PWD/deps/or-tools-read-only/lib/*
    copyorlibs.files+=$$PWD/deps/or-tools-read-only/dependencies/install/lib/*
}

copylang.path = $$installPath/languages
copylang.files = languages/*.ts

copyqml.path = $$installPath
copyqml.files = qml

copyexecutable.path = $$installPath

unix: copyexecutable.files = $$TARGET
win32: copyexecutable.files = $$PWD/release/$$sprintf($$TARGET%1, .exe)

INSTALLS += copymisc copyqtlibs copyqml copyexecutable copylang
unix: INSTALLS += copyorlibs



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

lupdate_only {
SOURCES = qml/ddt/*.qml
}

TRANSLATIONS = \
    languages/en.ts \
    languages/de.ts

QMAKE_CXXFLAGS += -std=c++11

OTHER_FILES += \
    options.xml \
    ddt.qmlproject \
    README.md \
    LICENSE \

INCLUDEPATH += \
    $$PWD/deps/or-tools-read-only/src \
    $$PWD/deps/or-tools-read-only/dependencies/install/include \

OBJECTS_DIR = ./obj

MOC_DIR = ./moc

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

