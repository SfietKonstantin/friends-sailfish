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
#include <QtQml/QQmlEngine>
#include <QtQuick/QQuickView>
#include <QtCore/QPluginLoader>
#include "clientidplugininterface.h"
#include <QtCore/QDebug>


#ifdef HAS_BOOSTER
#include <MDeclarativeCache>
#endif

#include "tokenmanager.h"
#include "settingsmanager.h"
#include "posthelper.h"
#include "footerhelper.h"
#include "notificationshelper.h"

// Login manager headers
#include "login/loginmanager.h"

// Image loader headers
#include "imageloader/imageloader.h"

// social plugin headers
#include "socialnetworkinterface.h"
#include "socialnetworkmodelinterface.h"
#include "contentiteminterface.h"
#include "identifiablecontentiteminterface.h"
#include "filterinterface.h"
//#include "sorterinterface.h"
//#include "contentitemtypefilterinterface.h"

// facebook implementation headers
#include "facebook/facebookinterface.h"
#include "facebook/facebookitemfilterinterface.h"
#include "facebook/facebookrelateddatafilterinterface.h"
#include "facebook/facebookobjectreferenceinterface.h"
#include "facebook/facebookalbuminterface.h"
#include "facebook/facebookcommentinterface.h"
#include "facebook/facebooknotificationinterface.h"
#include "facebook/facebookphotointerface.h"
#include "facebook/facebookpostinterface.h"
#include "facebook/facebookuserinterface.h"
#include "facebook/facebooklikeinterface.h"

// Social extra features
#include "socialextra/alphabeticalsorterinterface.h"
#include "socialextra/newsfeedfilterinterface.h"
#include "socialextra/facebookextrapostinterface.h"
#include "socialextra/typesolverinterface.h"

static const char *URI = "harbour.friends";
static const char *URI_SOCIAL = "harbour.friends.social";
static const char *URI_SOCIAL_EXTRA = "harbour.friends.social.extra";
static const char *REASON = "Cannot be created";
static const char *FACEBOOK_PAGE = "https://m.facebook.com/friendsforn9";
static const char *PAYPAL_DONATE = "https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&"
                                   "hosted_button_id=R6AJV4U2G33XG";

class DateHelper : public QObject
{
    Q_OBJECT
public:
    explicit DateHelper(QObject *parent = 0);
    Q_INVOKABLE static QDateTime fromString(const QString &date);
};

DateHelper::DateHelper(QObject *parent)
    : QObject(parent)
{
}

QDateTime DateHelper::fromString(const QString &date)
{
    return QDateTime::fromString(date, Qt::ISODate).toTimeSpec(Qt::LocalTime);
}



static QObject *imageloader_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    return new ImageLoader();
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

static QObject * notificationshelper_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    return new NotificationsHelper();
}

void importMisc()
{
    // @uri harbour.friends
    qmlRegisterType<TokenManager>(URI, 1, 0, "TokenManager");
    qmlRegisterType<SettingsManager>(URI, 1, 0, "SettingsManager");
    qmlRegisterType<QFB::LoginManager>(URI, 1, 0, "LoginManager");
    qmlRegisterSingletonType<ImageLoader>(URI, 1, 0, "ImageLoader", imageloader_provider);
    qmlRegisterSingletonType<DateHelper>(URI, 1, 0, "DateHelper", datehelper_provider);
    qmlRegisterSingletonType<FooterHelper>(URI, 1, 0, "FooterHelper", footerhelper_provider);
    qmlRegisterSingletonType<NotificationsHelper>(URI, 1, 0, "NotificationsHelper",
                                           notificationshelper_provider);
    qmlRegisterType<PostHelper>(URI, 1, 0, "PostHelper");
}

void importSocial()
{
    // @uri harbour.friends.social
    qmlRegisterUncreatableType<SocialNetworkInterface>(URI_SOCIAL, 1, 0, "SocialNetwork", REASON);
    qmlRegisterUncreatableType<ContentItemInterface>(URI_SOCIAL, 1, 0, "ContentItem", REASON);
    qmlRegisterUncreatableType<IdentifiableContentItemInterface>(URI_SOCIAL, 1, 0, "IdentifiableContentItem", REASON);
    qmlRegisterUncreatableType<FilterInterface>(URI_SOCIAL, 1, 0, "Filter", REASON);
    qmlRegisterUncreatableType<SorterInterface>(URI_SOCIAL, 1, 0, "Sorter", REASON);

    // creatable types from the social plugin
    qmlRegisterType<SocialNetworkModelInterface>(URI_SOCIAL, 1, 0, "SocialNetworkModel");

    // creatable types from the facebook implementation
    qmlRegisterType<FacebookInterface>(URI_SOCIAL, 1, 0, "Facebook");
    qmlRegisterType<FacebookItemFilterInterface>(URI_SOCIAL, 1, 0, "FacebookItemFilter");
    qmlRegisterType<FacebookRelatedDataFilterInterface>(URI_SOCIAL, 1, 0, "FacebookRelatedDataFilter");
    qmlRegisterType<FacebookObjectReferenceInterface>(URI_SOCIAL, 1, 0, "FacebookObjectReference");
    qmlRegisterType<FacebookAlbumInterface>(URI_SOCIAL, 1, 0, "FacebookAlbum");
    qmlRegisterType<FacebookCommentInterface>(URI_SOCIAL, 1, 0, "FacebookComment");
    qmlRegisterType<FacebookNotificationInterface>(URI_SOCIAL, 1, 0, "FacebookNotification");
    qmlRegisterType<FacebookPhotoInterface>(URI_SOCIAL, 1, 0, "FacebookPhoto");
    qmlRegisterType<FacebookPostInterface>(URI_SOCIAL, 1, 0, "FacebookPost");
    qmlRegisterType<FacebookUserInterface>(URI_SOCIAL, 1, 0, "FacebookUser");

    qmlRegisterType<FacebookLikeInterface>(URI_SOCIAL, 1, 0, "FacebookLike");
    qmlRegisterType<FacebookNameTagInterface>(URI_SOCIAL, 1, 0, "FacebookNameTag");
    qmlRegisterType<FacebookPhotoImageInterface>(URI_SOCIAL, 1, 0, "FacebookPhotoImage");
    qmlRegisterType<FacebookPhotoTagInterface>(URI_SOCIAL, 1, 0, "FacebookPhotoTag");
    qmlRegisterType<FacebookPostActionInterface>(URI_SOCIAL, 1, 0, "FacebookPostAction");
    qmlRegisterType<FacebookPostPropertyInterface>(URI_SOCIAL, 1, 0, "FacebookPostProperty");
    qmlRegisterType<FacebookUserCoverInterface>(URI_SOCIAL, 1, 0, "FacebookUserCover");
    qmlRegisterType<FacebookUserPictureInterface>(URI_SOCIAL, 1, 0, "FacebookUserPicture");
}

void importSocialExtra()
{
    // @uri harbour.friends.social.extra
    qmlRegisterType<AlphabeticalSorterInterface>(URI_SOCIAL_EXTRA, 1, 0, "AlphabeticalSorter");
    qmlRegisterType<NewsFeedFilterInterface>(URI_SOCIAL_EXTRA, 1, 0, "NewsFeedFilter");
    qmlRegisterType<FacebookExtraPostInterface>(URI_SOCIAL_EXTRA, 1, 0, "FacebookExtraPost");
    qmlRegisterType<TypeSolverInterface>(URI_SOCIAL_EXTRA, 1, 0, "TypeSolver");
    qmlRegisterType<TypeSolverFilterInterface>(URI_SOCIAL_EXTRA, 1, 0, "TypeSolverFilter");
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
    app->setOrganizationName("SfietKonstantin");
    app->setApplicationName("Friends");

    QObject::connect(view->engine(), SIGNAL(quit()), QCoreApplication::instance(), SLOT(quit()));

    // Client id
    QString clientId;
#ifndef DESKTOP
    QPluginLoader pluginLoader (CLIENT_ID_PLUGIN);
    if (pluginLoader.load()) {
        QObject *plugin = pluginLoader.instance();
        ClientIdPluginInterface *castedPlugin = qobject_cast<ClientIdPluginInterface *>(plugin);
        if (castedPlugin) {
            clientId = castedPlugin->clientId();
            qDebug() << "Client id loaded";
        }
    }
#else
    if (app->arguments().count() == 2) {
        clientId = app->arguments().at(1);
        qDebug() << "Client id loaded: " << clientId;
    }
#endif

    view->engine()->rootContext()->setContextProperty("CLIENT_ID", clientId);
    view->engine()->rootContext()->setContextProperty("VERSION", VERSION);
    view->engine()->rootContext()->setContextProperty("FACEBOOK_PAGE", FACEBOOK_PAGE);
    view->engine()->rootContext()->setContextProperty("PAYPAL_DONATE", PAYPAL_DONATE);

    importMisc();
    importSocial();
    importSocialExtra();

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
#ifndef DESKTOP
    view->showFullScreen();
#else
    view->show();
#endif


    int result = app->exec();
    app->removeTranslator(translator.data());
    app->removeTranslator(engineeringEnglish.data());
    return result;
}

#include "main.moc"
