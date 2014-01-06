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

QT += qml quick

CONFIG += link_pkgconfig

HEADERS += clientidplugininterface.h \
    tokenmanager.h \
    settingsmanager.h \
    posthelper.h \
    footerhelper.h
SOURCES += main.cpp \
    tokenmanager.cpp \
    settingsmanager.cpp \
    posthelper.cpp \
    footerhelper.cpp

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
    qml/LikesPage.qml

target.path = $$TARGETPATH

desktop.path = /usr/share/applications
desktop.files = harbour-friends.desktop

icon.path = /usr/share/icons/hicolor/86x86/apps/
icon.files = harbour-friends.png

DEFINES *= DEPLOYMENT_PATH=\"\\\"\"$${DEPLOYMENT_PATH}/\"\\\"\"
qml.path = $$DEPLOYMENT_PATH/qml
qml.files = $$OTHER_FILES

# translations
TS_FILE = $$OUT_PWD/friends.ts
EE_QM = $$OUT_PWD/friends_eng_en.qm

ts.commands += lupdate $$PWD -ts $$TS_FILE
ts.CONFIG += no_check_exist
ts.output = $$TS_FILE
ts.input = .

ts_install.files = $$TS_FILE
ts_install.path = /usr/share/translations/source
ts_install.CONFIG += no_check_exist

# should add -markuntranslated "-" when proper translations are in place (or for testing)
engineering_english.commands += lrelease -idbased $$TS_FILE -qm $$EE_QM
engineering_english.CONFIG += no_check_exist
engineering_english.depends = ts
engineering_english.input = $$TS_FILE
engineering_english.output = $$EE_QM

engineering_english_install.path = /usr/share/translations
engineering_english_install.files = $$EE_QM
engineering_english_install.CONFIG += no_check_exist

QMAKE_EXTRA_TARGETS += ts engineering_english

PRE_TARGETDEPS += ts engineering_english

INSTALLS += target desktop icon qml
#INSTALLS += ts_install engineering_english_install

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
