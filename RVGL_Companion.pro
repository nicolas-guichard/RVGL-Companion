QT += qml quick network quickcontrols2

CONFIG += c++11

RC_ICONS = icon.ico

include(singleapplication/singleapplication.pri)
DEFINES += QAPPLICATION_CLASS=QGuiApplication

SOURCES += main.cpp \
    assetsmanager.cpp \
    filedownloader.cpp \
    rvgllauncher.cpp \
    portmanager.cpp

HEADERS += \
    assetsmanager.h \
    filedownloader.h \
    rvgllauncher.h \
    portmanager.h

RESOURCES += qml.qrc

win32 {
    DEFINES += MINIUPNP_STATICLIB STATICLIB
    LIBS += -lws2_32 -liphlpapi

    !contains(QMAKE_TARGET.arch, x86_64) {
        RESOURCES += win32.qrc
    } else {
        RESOURCES += win64.qrc
    }
}

DISTFILES += \
    README.md \
    LICENSE \
    .travis.yml \
    .appveyor.yml \
    AUTHORS \
    travis/linux/build.sh \
    travis/linux/requirements.sh \
    travis/windows/build.sh \
    travis/windows/requirements.sh \
    travis/windows/package.sh \
    travis/linux/package.sh

DEFINES += QT_DEPRECATED_WARNINGS

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

LIBS += -L$${PWD} -lminiupnpc
