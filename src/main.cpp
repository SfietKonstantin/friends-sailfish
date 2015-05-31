/*
 * Copyright (C) 2013 Lucien XU <sfietkonstantin@free.fr>
 *
 * You may use this file under the terms of the BSD license as follows:
 *
 * "Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * The names of its contributors may not be used to endorse or promote
 *     products derived from this software without specific prior written
 *     permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 */

#include <QtCore/QLocale>
#include <QtCore/QTranslator>
#include <QtCore/QStandardPaths>
#include <QtCore/QDir>
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlContext>
#include <QtQml/QQmlEngine>
#include <QtQuick/QQuickView>
#include <QtCore/QPluginLoader>
#include <QtCore/QDebug>


#ifdef HAS_BOOSTER
#include <MDeclarativeCache>
#endif

#include "defines_p.h"
#include "tokenmanager.h"
#include "settingsmanager.h"
#include "posthelper.h"
#include "footerhelper.h"
//#include "notificationshelper.h"
#include "imagehelper.h"
#include "imagemanager.h"
#include "changelogmodel.h"
//#include "threadhelper.h"
//#include "userinfohelper.h"
#include "datehelper.h"
#include "plugin.h"
#include "friendsproxymodel.h"
#include "newsfeedproxymodel.h"

// Login manager headers
#include "login/loginmanager.h"

// Image loader headers
//#include "imageloader/imageloader.h"

#include "dbus/friendsdbusinterface.h"

static const char *URI = "harbour.friends";
static const char *URI_MICROF = "harbour.friends.microf";
static const char *REASON = "Cannot be created";
static const char *FRIENDS_QT = "friendsqt";
static const char *PAYPAL_DONATE = "https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&"
                                   "hosted_button_id=R6AJV4U2G33XG";

static QObject *imagemanager_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    return new ImageManager();
}

static QObject * datehelper_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    return new DateHelper();
}

static QObject * footerhelper_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    return new FooterHelper();
}

//static QObject * notificationshelper_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
//{
//    Q_UNUSED(engine)
//    Q_UNUSED(scriptEngine)
//    return new NotificationsHelper();
//}

void importMisc()
{
    // @uri harbour.friends
    qmlRegisterType<TokenManager>(URI, 1, 0, "TokenManager");
    qmlRegisterType<SettingsManager>(URI, 1, 0, "SettingsManager");
    qmlRegisterType<QFB::LoginManager>(URI, 1, 0, "LoginManager");
    // qmlRegisterSingletonType<ImageLoader>(URI, 1, 0, "ImageLoader", imageloader_provider);
    qmlRegisterSingletonType<DateHelper>(URI, 1, 0, "DateHelper", datehelper_provider);
    qmlRegisterSingletonType<FooterHelper>(URI, 1, 0, "FooterHelper", footerhelper_provider);
//    qmlRegisterSingletonType<NotificationsHelper>(URI, 1, 0, "NotificationsHelper",
//                                           notificationshelper_provider);
    qmlRegisterType<PostHelper>(URI, 1, 0, "PostHelper");
    qmlRegisterType<ImageHelper>(URI, 1, 0, "ImageHelper");
    qmlRegisterSingletonType<ImageManager>(URI, 1, 0, "ImageManager", imagemanager_provider);
    qmlRegisterUncreatableType<QQuickImageBase>(URI, 1, 0, "QQuickImageBase", REASON);
    qmlRegisterType<ChangeLogModel>(URI, 1, 0, "ChangeLogModel");
//    qmlRegisterType<ThreadHelper>(URI, 1, 0, "ThreadHelper");
//    qmlRegisterType<UserInfoHelper>(URI, 1, 0, "UserInfoHelper");
    qmlRegisterType<FriendsProxyModel>(URI, 1, 0, "FriendsProxyModel");
    qmlRegisterType<NewsFeedProxyModel>(URI, 1, 0, "NewsFeedProxyModel");
}

void importMicroF()
{
    // @uri harbour.friends.microf
    qmlRegisterUncreatableType<SocialNetwork>(URI_MICROF, 1, 0, "SocialNetwork", REASON);
    qmlRegisterUncreatableType<SocialNetworkStatus>(URI_MICROF, 1, 0, "SocialNetworkStatus", REASON);
    qmlRegisterUncreatableType<SocialNetworkError>(URI_MICROF, 1, 0, "SocialNetworkError", REASON);
    qmlRegisterUncreatableType<SocialRequest>(URI_MICROF, 1, 0, "SocialRequest", REASON);
    qmlRegisterUncreatableType<SocialContentItemBuilder>(URI_MICROF, 1, 0, "SocialContentItemBuilder", REASON);
    qmlRegisterUncreatableType<SocialContentModelBuilder>(URI_MICROF, 1, 0, "SocialContentModelBuilder", REASON);
    qmlRegisterType<SocialContentItem>(URI_MICROF, 1, 0, "SocialContentItem");
    qmlRegisterType<SocialContentModel>(URI_MICROF, 1, 0, "SocialContentModel");
    qmlRegisterType<Facebook>(URI_MICROF, 1, 0, "Facebook");
    qmlRegisterType<FacebookLoginRequest>(URI_MICROF, 1, 0, "FacebookLoginRequest");
    qmlRegisterType<FacebookLoginContentBuilder>(URI_MICROF, 1, 0, "FacebookLoginContentBuilder");
    qmlRegisterType<FacebookLogoutRequest>(URI_MICROF, 1, 0, "FacebookLogoutRequest");
    qmlRegisterType<FacebookConfirmationContentBuilder>(URI_MICROF, 1, 0, "FacebookConfirmationContentBuilder");
    qmlRegisterType<FacebookProperty>(URI_MICROF, 1, 0, "FacebookProperty");
    qmlRegisterType<FacebookListProperty>(URI_MICROF, 1, 0, "FacebookListProperty");
    qmlRegisterType<FacebookItemBuilder>(URI_MICROF, 1, 0, "FacebookItemBuilder");
    qmlRegisterType<FacebookModelBuilder>(URI_MICROF, 1, 0, "FacebookModelBuilder");
    qmlRegisterType<FacebookNewsFeedModelBuilder>(URI_MICROF, 1, 0, "FacebookNewsFeedModelBuilder");
    qmlRegisterType<FacebookFriendListRequest>(URI_MICROF, 1, 0, "FacebookFriendListRequest");
    qmlRegisterType<FacebookUserSummaryRequest>(URI_MICROF, 1, 0, "FacebookUserSummaryRequest");
    qmlRegisterType<FacebookNewsFeedRequest>(URI_MICROF, 1, 0, "FacebookNewsFeedRequest");
}

Q_DECL_EXPORT int main(int argc, char *argv[])
{
#ifdef HAS_BOOSTER
    QScopedPointer<QGuiApplication> app(MDeclarativeCache::qApplication(argc, argv));
    QScopedPointer<QQuickView> view(MDeclarativeCache::qQuickView());
#else
    QScopedPointer<QGuiApplication> app(new QGuiApplication(argc, argv));
    QScopedPointer<QQuickView> view(new QQuickView);
#endif
    app->setOrganizationName(ORGANIZATION_NAME);
    app->setApplicationName(APPLICATION_NAME);

    QObject::connect(view->engine(), SIGNAL(quit()), QCoreApplication::instance(), SLOT(quit()));

    view->engine()->rootContext()->setContextProperty("VERSION", VERSION);
    view->engine()->rootContext()->setContextProperty("FRIENDS_QT", FRIENDS_QT);
    view->engine()->rootContext()->setContextProperty("PAYPAL_DONATE", PAYPAL_DONATE);

    QString requestedFacebookId;
    if (app->arguments().count() == 2) {
        requestedFacebookId = app->arguments().at(1);
    }
    view->engine()->rootContext()->setContextProperty("REQUESTED_FACEBOOK_ID", requestedFacebookId);

    importMisc();
    importMicroF();
    FriendsDBusInterface dbusInterface;
    dbusInterface.registerView(view.data());
    view->engine()->rootContext()->setContextProperty("FriendsDBusInterface", &dbusInterface);

    // Translations
    QScopedPointer<QTranslator> engineeringEnglish(new QTranslator);
    QScopedPointer<QTranslator> translator(new QTranslator);
#ifdef DESKTOP
    QString translationPath = QLatin1String(":/translations/");
#else
    QString translationPath = QString(DEPLOYMENT_PATH) + QLatin1String("translations");
#endif
    engineeringEnglish->load("friends-engineering-english", translationPath);
    translator->load(QLocale(), "friends", "_", translationPath);

    app->installTranslator(engineeringEnglish.data());
    app->installTranslator(translator.data());

    QString path = QString(DEPLOYMENT_PATH) + QLatin1String("qml/friends.qml");
#ifdef DESKTOP
    path = "qrc:/qml/friends.qml";
#endif
    view->setSource(path);
    view->show();

    int result = app->exec();
    app->removeTranslator(translator.data());
    app->removeTranslator(engineeringEnglish.data());
    return result;
}
