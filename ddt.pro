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


# Installation path
#target.path = $$PWD/../ddt_install

unix: TARGET = ddtexe
win: TARGET = ddt

installPath = $$PWD/../ddt_install

copymisc.path = $$installPath
copymisc.files+=misc/*


copyqtlibs.path = $$installPath/lib
unix: {
    qtLibPath = $$(QTDIR)/lib
    copyqtlibs.files+=$$qtLibPath/libQt5Core.so.5.1.1
    copyqtlibs.files+=$$qtLibPath/libQt5Gui.so.5.1.1
    copyqtlibs.files+=$$qtLibPath/libQt5Widgets.so.5.1.1
}

copyorlibs.path = $$installPath/lib
copyorlibs.files+=$$PWD/deps/or-tools-read-only/lib/*
copyorlibs.files+=$$PWD/deps/or-tools-read-only/dependencies/install/lib/*

copylang.path = $$installPath/languages
copylang.files = languages/*.ts

copyqml.path = $$installPath
copyqml.files = qml

copyexecutable.path = $$installPath
copyexecutable.files = $$TARGET

INSTALLS += copymisc copyqtlibs copyorlibs copyqml copyexecutable copylang




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


