include(social.pri)
include(socialextra/socialextra.pri)
include(login/login.pri)
include(imageloader/imageloader.pri)
include(../version.pri)

TEMPLATE = app
TARGET = harbour-friends
TARGETPATH = /usr/bin
DEPLOYMENT_PATH = /usr/share/$$TARGET
DEFINES *= 'VERSION=\'\"$${VERSION}\"\''
DEFINES *= 'CLIENT_ID_PLUGIN=\'\"$${DEPLOYMENT_PATH}/lib/libharbour-friends-clientidplugin.so\"\''
include(data/data.pri)
include(translations.pri)

QT += qml quick

CONFIG += link_pkgconfig

HEADERS += clientidplugininterface.h \
    tokenmanager.h \
    settingsmanager.h \
    posthelper.h \
    footerhelper.h \
    notificationshelper.h
SOURCES += main.cpp \
    tokenmanager.cpp \
    settingsmanager.cpp \
    posthelper.cpp \
    footerhelper.cpp \
    notificationshelper.cpp

OTHER_FILES += qml/friends.qml \
    qml/UiConstants.js \
    qml/CoverHeader.qml \
    qml/CoverPage.qml \
    qml/MenuPage.qml \
    qml/NewsPage.qml \
    qml/LoginDialog.qml \
    qml/FacebookImage.qml \
    qml/FacebookPicture.qml \
    qml/FriendsPage.qml \
    qml/StateIndicator.qml \
    qml/AlbumsPage.qml \
    qml/UserPage.qml \
    qml/PostDelegate.qml \
    qml/AboutPage.qml \
    qml/DevelopersPage.qml \
    qml/PhotosPage.qml \
    qml/PhotoPage.qml \
    qml/SplitSocialPanel.qml \
    qml/CommentsPage.qml \
    qml/PostCommentHeaderComponent.qml \
    qml/WelcomeDialog.qml \
    qml/SocialButtons.qml \
    qml/PostDialog.qml \
    qml/LikesPage.qml \
    qml/NotificationsPage.qml \
    qml/TypeSolverPage.qml

target.path = $$TARGETPATH

desktop.path = /usr/share/applications
desktop.files = harbour-friends.desktop

icon.path = /usr/share/icons/hicolor/86x86/apps/
icon.files = harbour-friends.png

DEFINES *= DEPLOYMENT_PATH=\"\\\"\"$${DEPLOYMENT_PATH}/\"\\\"\"
qml.path = $$DEPLOYMENT_PATH/qml
qml.files = $$OTHER_FILES

INSTALLS += target desktop icon qml

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
