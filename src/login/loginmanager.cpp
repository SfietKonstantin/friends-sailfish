/****************************************************************************************
 * Copyright (C) 2012 Lucien XU <sfietkonstantin@free.fr>                               *
 *                                                                                      *
 * This program is free software; you can redistribute it and/or modify it under        *
 * the terms of the GNU General Public License as published by the Free Software        *
 * Foundation; either version 3 of the License, or (at your option) any later           *
 * version.                                                                             *
 *                                                                                      *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY      *
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A      *
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.             *
 *                                                                                      *
 * You should have received a copy of the GNU General Public License along with         *
 * this program.  If not, see <http://www.gnu.org/licenses/>.                           *
 ****************************************************************************************/

/**
 * @file loginmanager.cpp
 * @short Implementation of QFB::LoginManager
 */

#include "loginmanager.h"

#include <QtCore/QDebug>
#include <QtCore/QRegExp>
#include <QtCore/QStringList>
#include <QtCore/QUrl>

namespace QFB
{

/**
 * @internal
 * @brief Private class for QFB::LoginManager
 */
class LoginManagerPrivate
{
public:
    /**
     * @internal
     * @brief Translate extended permissions
     * @param extendedPermissions extended permissions.
     * @return a string list providing the list of permissions.
     */
    static QStringList translateExtendedPermissions(
            LoginManager::ExtendedPermissions extendedPermissions);
    /**
     * @internal
     * @brief Translate user permissions
     * @param userPermissions user permissions.
     * @return a string list providing the list of permissions.
     */
    static QStringList translateUserPermissions(LoginManager::UserPermissions userPermissions);
    /**
     * @internal
     * @brief Translate friends permissions
     * @param friendsPermissions friends permissions.
     * @return a string list providing the list of permissions.
     */
    static QStringList translateFriendsPermissions(
            LoginManager::FriendsPermissions friendsPermissions);
    /**
     * @internal
     * @brief Client ID
     */
    QString clientId;
    /**
     * @internal
     * @brief Extended permissions
     */
    LoginManager::ExtendedPermissions extendedPermissions;
    /**
     * @internal
     * @brief User permissions
     */
    LoginManager::UserPermissions userPermissions;
    /**
     * @internal
     * @brief Friends permissions
     */
    LoginManager::FriendsPermissions friendsPermissions;
    /**
     * @internal
     * @brief UI type
     */
    LoginManager::UiType uiType;
};

QStringList LoginManagerPrivate::translateExtendedPermissions(
        LoginManager::ExtendedPermissions extendedPermissions)
{
    QStringList permissionsList;
    if (extendedPermissions.testFlag(LoginManager::ReadFriendList)) {
        permissionsList.append("read_friendlists");
    }
    if (extendedPermissions.testFlag(LoginManager::ReadInsights)) {
        permissionsList.append("read_insights");
    }
    if (extendedPermissions.testFlag(LoginManager::ReadMailbox)) {
        permissionsList.append("read_mailbox");
    }
    if (extendedPermissions.testFlag(LoginManager::ReadRequests)) {
        permissionsList.append("read_requests");
    }
    if (extendedPermissions.testFlag(LoginManager::ReadStream)) {
        permissionsList.append("read_stream");
    }
    if (extendedPermissions.testFlag(LoginManager::XmppLogin)) {
        permissionsList.append("xmpp_login");
    }
    if (extendedPermissions.testFlag(LoginManager::CreateEvent)) {
        permissionsList.append("create_event");
    }
    if (extendedPermissions.testFlag(LoginManager::ManageFriendList)) {
        permissionsList.append("manage_friendlists");
    }
    if (extendedPermissions.testFlag(LoginManager::ManageNotifications)) {
        permissionsList.append("manage_notifications");
    }
    if (extendedPermissions.testFlag(LoginManager::UserOnlinePresence)) {
        permissionsList.append("user_online_presence");
    }
    if (extendedPermissions.testFlag(LoginManager::FriendsOnlinePresence)) {
        permissionsList.append("friends_online_presence");
    }
    if (extendedPermissions.testFlag(LoginManager::PublishCheckins)) {
        permissionsList.append("publish_checkins");
    }
    if (extendedPermissions.testFlag(LoginManager::PublishStream)) {
        permissionsList.append("publish_stream");
    }
    if (extendedPermissions.testFlag(LoginManager::RsvpEvent)) {
        permissionsList.append("rsvp_event");
    }

    return permissionsList;
}

QStringList LoginManagerPrivate::translateUserPermissions(
        LoginManager::UserPermissions userPermissions)
{
    QStringList permissionsList;
    if (userPermissions.testFlag(LoginManager::UserAboutMe)) {
        permissionsList.append("user_about_me");
    }
    if (userPermissions.testFlag(LoginManager::UserActivities)) {
        permissionsList.append("user_activities");
    }
    if (userPermissions.testFlag(LoginManager::UserBirthday)) {
        permissionsList.append("user_birthday");
    }
    if (userPermissions.testFlag(LoginManager::UserEducationHistory)) {
        permissionsList.append("user_education_history");
    }
    if (userPermissions.testFlag(LoginManager::UserEvents)) {
        permissionsList.append("user_events");
    }
    if (userPermissions.testFlag(LoginManager::UserGroups)) {
        permissionsList.append("user_groups");
    }
    if (userPermissions.testFlag(LoginManager::UserHometown)) {
        permissionsList.append("user_hometown");
    }
    if (userPermissions.testFlag(LoginManager::UserInterests)) {
        permissionsList.append("user_interests");
    }
    if (userPermissions.testFlag(LoginManager::UserLikes)) {
        permissionsList.append("user_likes");
    }
    if (userPermissions.testFlag(LoginManager::UserLocation)) {
        permissionsList.append("user_location");
    }
    if (userPermissions.testFlag(LoginManager::UserNotes)) {
        permissionsList.append("user_notes");
    }
    if (userPermissions.testFlag(LoginManager::UserPhotos)) {
        permissionsList.append("user_photos");
    }
    if (userPermissions.testFlag(LoginManager::UserQuestions)) {
        permissionsList.append("user_questions");
    }
    if (userPermissions.testFlag(LoginManager::UserRelationships)) {
        permissionsList.append("user_relationships");
    }
    if (userPermissions.testFlag(LoginManager::UserRelationshipDetails)) {
        permissionsList.append("user_relationship_details");
    }
    if (userPermissions.testFlag(LoginManager::UserReligionPolitics)) {
        permissionsList.append("user_religion_politics");
    }
    if (userPermissions.testFlag(LoginManager::UserStatus)) {
        permissionsList.append("user_status");
    }
    if (userPermissions.testFlag(LoginManager::UserSubscriptions)) {
        permissionsList.append("user_subscriptions");
    }
    if (userPermissions.testFlag(LoginManager::UserVideos)) {
        permissionsList.append("user_videos");
    }
    if (userPermissions.testFlag(LoginManager::UserWebsite)) {
        permissionsList.append("user_website");
    }
    if (userPermissions.testFlag(LoginManager::UserWorkHistory)) {
        permissionsList.append("user_work_history");
    }
    if (userPermissions.testFlag(LoginManager::Email)) {
        permissionsList.append("email");
    }

    return permissionsList;
}

QStringList LoginManagerPrivate::translateFriendsPermissions(
        LoginManager::FriendsPermissions friendsPermissions)
{
    QStringList permissionsList;
    if (friendsPermissions.testFlag(LoginManager::FriendsAboutMe)) {
        permissionsList.append("friends_about_me");
    }
    if (friendsPermissions.testFlag(LoginManager::FriendsActivities)) {
        permissionsList.append("friends_activities");
    }
    if (friendsPermissions.testFlag(LoginManager::FriendsBirthday)) {
        permissionsList.append("friends_birthday");
    }
    if (friendsPermissions.testFlag(LoginManager::FriendsEducationHistory)) {
        permissionsList.append("friends_education_history");
    }
    if (friendsPermissions.testFlag(LoginManager::FriendsEvents)) {
        permissionsList.append("friends_events");
    }
    if (friendsPermissions.testFlag(LoginManager::FriendsGroups)) {
        permissionsList.append("friends_groups");
    }
    if (friendsPermissions.testFlag(LoginManager::FriendsHometowm)) {
        permissionsList.append("friends_hometown");
    }
    if (friendsPermissions.testFlag(LoginManager::FriendsInterests)) {
        permissionsList.append("friends_interests");
    }
    if (friendsPermissions.testFlag(LoginManager::FriendsLikes)) {
        permissionsList.append("friends_likes");
    }
    if (friendsPermissions.testFlag(LoginManager::FriendsLocation)) {
        permissionsList.append("friends_location");
    }
    if (friendsPermissions.testFlag(LoginManager::FriendsNotes)) {
        permissionsList.append("friends_notes");
    }
    if (friendsPermissions.testFlag(LoginManager::FriendsPhotos)) {
        permissionsList.append("friends_photos");
    }
    if (friendsPermissions.testFlag(LoginManager::FriendsQuestions)) {
        permissionsList.append("friends_questions");
    }
    if (friendsPermissions.testFlag(LoginManager::FriendsRelationships)) {
        permissionsList.append("friends_relationships");
    }
    if (friendsPermissions.testFlag(LoginManager::FriendsRelationshipDetails)) {
        permissionsList.append("friends_relationship_details");
    }
    if (friendsPermissions.testFlag(LoginManager::FriendsReligionPolitics)) {
        permissionsList.append("friends_religion_politics");
    }
    if (friendsPermissions.testFlag(LoginManager::FriendsStatus)) {
        permissionsList.append("friends_status");
    }
    if (friendsPermissions.testFlag(LoginManager::FriendsSubscriptions)) {
        permissionsList.append("friends_subscriptions");
    }
    if (friendsPermissions.testFlag(LoginManager::FriendsVideos)) {
        permissionsList.append("friends_videos");
    }
    if (friendsPermissions.testFlag(LoginManager::FriendsWebsite)) {
        permissionsList.append("friends_website");
    }
    if (friendsPermissions.testFlag(LoginManager::FriendsWorkHistory)) {
        permissionsList.append("friends_work_history");
    }
    return permissionsList;
}

////// End of private class //////

LoginManager::LoginManager(QObject *parent):
    QObject(parent), d_ptr(new LoginManagerPrivate)
{
    Q_D(LoginManager);
    d->extendedPermissions = ExtendedPermissions();
    d->userPermissions = UserPermissions();
    d->uiType = Desktop;
}

LoginManager::LoginManager(const QString &clientId, QObject *parent):
    QObject(parent), d_ptr(new LoginManagerPrivate)
{
    Q_D(LoginManager);
    d->clientId = clientId;
    d->extendedPermissions = ExtendedPermissions();
    d->userPermissions = UserPermissions();
    d->uiType = Desktop;
}

LoginManager::LoginManager(const QString &clientId, ExtendedPermissions extendedPermissions,
                           UserPermissions userPermissions, FriendsPermission friendsPermission,
                           QObject *parent):
    QObject(parent), d_ptr(new LoginManagerPrivate)
{
    Q_D(LoginManager);
    d->clientId = clientId;
    d->extendedPermissions = extendedPermissions;
    d->userPermissions = userPermissions;
    d->friendsPermissions = friendsPermission;
    d->uiType = Desktop;
}

LoginManager::LoginManager(const QString &clientId, UiType uiType, QObject *parent):
    QObject(parent), d_ptr(new LoginManagerPrivate)
{
    Q_D(LoginManager);
    d->clientId = clientId;
    d->extendedPermissions = ExtendedPermissions();
    d->userPermissions = UserPermissions();
    d->uiType = uiType;
}

LoginManager::LoginManager(const QString &clientId, ExtendedPermissions extendedPermissions,
                           UserPermissions userPermissions, FriendsPermission friendsPermission,
                           UiType uiType, QObject *parent):
    QObject(parent), d_ptr(new LoginManagerPrivate)
{
    Q_D(LoginManager);
    d->clientId = clientId;
    d->extendedPermissions = extendedPermissions;
    d->userPermissions = userPermissions;
    d->friendsPermissions = friendsPermission;
    d->uiType = uiType;
}

LoginManager::~LoginManager()
{
}

QString LoginManager::clientId() const
{
    Q_D(const LoginManager);
    return d->clientId;
}

LoginManager::ExtendedPermissions LoginManager::extendedPermissions() const
{
    Q_D(const LoginManager);
    return d->extendedPermissions;
}

LoginManager::UserPermissions LoginManager::userPermissions() const
{
    Q_D(const LoginManager);
    return d->userPermissions;
}

LoginManager::FriendsPermissions LoginManager::friendsPermissions() const
{
    Q_D(const LoginManager);
    return d->friendsPermissions;
}

LoginManager::UiType LoginManager::uiType() const
{
    Q_D(const LoginManager);
    return d->uiType;
}

void LoginManager::setClientId(const QString &clientId)
{
    Q_D(LoginManager);
    if (d->clientId != clientId) {
        d->clientId = clientId;
        emit clientIdChanged();
    }
}

void LoginManager::setExtendedPermissions(ExtendedPermissions extendedPermissions)
{
    Q_D(LoginManager);
    if (d->extendedPermissions != extendedPermissions) {
        d->extendedPermissions = extendedPermissions;
        emit extendedPermissionsChanged();
    }
}

void LoginManager::setUserPermissions(UserPermissions userPermissions)
{
    Q_D(LoginManager);
    if (d->userPermissions != userPermissions) {
        d->userPermissions = userPermissions;
        emit userPermissionsChanged();
    }
}

void LoginManager::setFriendsPermissions(FriendsPermissions friendsPermissions)
{
    Q_D(LoginManager);
    if (d->friendsPermissions != friendsPermissions) {
        d->friendsPermissions = friendsPermissions;
        emit friendsPermissionsChanged();
    }
}

void LoginManager::setUiType(UiType uiType)
{
    Q_D(LoginManager);
    if (d->uiType != uiType) {
        d->uiType = uiType;
        emit uiTypeChanged();
    }
}

void LoginManager::login()
{
    Q_D(LoginManager);
    QStringList extendedPermissions = d->translateExtendedPermissions(d->extendedPermissions);
    QStringList userPermissions = d->translateUserPermissions(d->userPermissions);
    QStringList friendsPermissions = d->translateFriendsPermissions(d->friendsPermissions);
    QStringList permissionsList;
    permissionsList.append(extendedPermissions);
    permissionsList.append(userPermissions);
    permissionsList.append(friendsPermissions);
    QString permissions = permissionsList.join(",");

    qDebug() << "Asked permissions" << permissions;

    QString url ("https://%1.facebook.com/dialog/oauth?client_id=%2&\
redirect_uri=https://www.facebook.com/connect/login_success.html&scope=%3&response_type=token");

    switch (d->uiType) {
        case Mobile:
            url = url.arg("m", d->clientId, permissions);
            break;
        default:
            url = url.arg("www", d->clientId, permissions);
            break;
    }

    emit urlRequested(QUrl(url));
}

void LoginManager::checkUrl(const QUrl &url)
{
    Q_D(LoginManager);
    QString urlToString = url.toString();
    // Check if the URL is not good
    if (!urlToString.contains(d->clientId)) {
        if (urlToString.contains("https://www.facebook.com/connect/login_success.html")) {
            // Get the token
            QRegExp tokenRegExp ("access_token=([^&]*)&");
            if (url.fragment().indexOf(tokenRegExp) != -1) {
                QString token = tokenRegExp.cap(1);

                qDebug() << "Login succeded";
                emit loginSucceeded(token);
                return;
            } else {
                qDebug() << "Login failed";
                emit loginFailed();
                return;
            }
        }
        qDebug() << "The user clicked on a wrong link !";
        login();
    }
}

}
