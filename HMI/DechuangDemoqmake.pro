QT += quick

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TARGET = DechCluster

SOURCES += src/main.cpp

HEADERS += src/datamodel.h

win32 {
    SOURCES += src/win/baseinfodatamodel.cpp \
               src/win/gaugedatamodel.cpp \
               src/win/telltaledatamodel.cpp

    HEADERS += src/win/baseinfodatamodel.h \
               src/win/gaugedatamodel.h \
               src/win/telltaledatamodel.h

    DEFINES += WINDOWS
}
else {
    SOURCES += src/baseinfodatamodel.cpp \
               src/gaugedatamodel.cpp \
               src/telltaledatamodel.cpp

    HEADERS += src/baseinfodatamodel.h \
               src/gaugedatamodel.h \
               src/telltaledatamodel.h

    INCLUDEPATH += ../HMIControllers/inc ../HMIControllers/src/sharedmemory ../
    SOURCES += ../HMIControllers/src/sharedmemory/sm_gauge.cpp \
               ../HMIControllers/src/sharedmemory/sm_domain.cpp

    LIBS += -lrt -L../bld/Log -llog
}


RESOURCES += qml.qrc\
             resource.qrc

INCLUDEPATH +=src\

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target

target.path=../release/usr/local/bin/hmi

config.path=../release/usr/share/fonts
config.files += \
    ./content/fonts/*

INSTALLS += target config


