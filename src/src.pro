include(microf.pri)
include(dbus/dbus.pri)
include(../version.pri)

TEMPLATE = app
TARGET = harbour-friends
TARGETPATH = /usr/bin
DEPLOYMENT_PATH = /usr/share/$$TARGET
DEFINES *= 'VERSION=\'\"$${VERSION}\"\''
include(data/data.pri)
include(translations/translations.pri)

QT += core core-private gui qml quick quick-private dbus

CONFIG += c++11
CONFIG += link_pkgconfig

HEADERS += cachehelper_p.h \
    clientidplugininterface.h \
    tokenmanager.h \
    settingsmanager.h \
    posthelper.h \
    footerhelper.h \
#    notificationshelper.h \
    imagehelper.h \
    imagemanager.h \
    changelogmodel.h \
#    threadhelper.h \
    objecthelper_p.h \
    abstractdisplayhelper.h \
#    userinfohelper.h \
    datehelper.h \
    defines_p.h \
    abstractproxymodel.h \
    friendsproxymodel.h \
    newsfeedproxymodel.h

SOURCES += main.cpp \
    tokenmanager.cpp \
    settingsmanager.cpp \
    posthelper.cpp \
    footerhelper.cpp \
#    notificationshelper.cpp \
    imagehelper.cpp \
    imagemanager.cpp \
    changelogmodel.cpp \
#    threadhelper.cpp \
    abstractdisplayhelper.cpp \
#    userinfohelper.cpp \
    datehelper.cpp \
    abstractproxymodel.cpp \
    friendsproxymodel.cpp \
    newsfeedproxymodel.cpp

target.path = $$TARGETPATH

desktop.path = /usr/share/applications
desktop.files = harbour-friends.desktop

icon.path = /usr/share/icons/hicolor/86x86/apps/
icon.files = harbour-friends.png

DEFINES *= DEPLOYMENT_PATH=\"\\\"\"$${DEPLOYMENT_PATH}/\"\\\"\"

INSTALLS += target desktop icon

CONFIG(desktop):{
RESOURCES += friends.qrc
DEFINES += DESKTOP
}

packagesExist(qdeclarative5-boostable) {
    message("Building with qdeclarative-boostable support")
    DEFINES += HAS_BOOSTER
    PKGCONFIG += qdeclarative5-boostable
} else {
   warning("qdeclarative-boostable not available; startup times will be slower")
}

include(qml/qml.pri)
