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

import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.friends 1.0

Dialog {
    id: container
    onRejected: Qt.quit()
    canAccept: false
    signal connected()

    DialogHeader {
        id: header
        //: Action displayed on the cancel button for the login dialog
        //% "Quit"
        cancelText: qsTrId("friends_login_quit")
        //: Action displayed on the accept button for the login dialog. This button is never active, and should display "Ignore". It cannot be triggered.
        //% "Ignore"
        acceptText: qsTrId("friends_login_ignore")
    }

    LoginManager {
        id: loginManager
        uiType: LoginManager.Mobile
        clientId: CLIENT_ID
        friendsPermissions: LoginManager.FriendsAboutMe
                            + LoginManager.FriendsActivities
                            + LoginManager.FriendsBirthday
                            + LoginManager.FriendsEducationHistory
                            + LoginManager.FriendsEvents
                            + LoginManager.FriendsGroups
                            + LoginManager.FriendsHometowm
                            + LoginManager.FriendsInterests
                            + LoginManager.FriendsLikes
                            + LoginManager.FriendsLocation
                            + LoginManager.FriendsNotes
                            + LoginManager.FriendsPhotos
                            + LoginManager.FriendsQuestions
                            + LoginManager.FriendsRelationships
                            + LoginManager.FriendsRelationshipDetails
                            + LoginManager.FriendsReligionPolitics
                            + LoginManager.FriendsStatus
                            + LoginManager.FriendsSubscriptions
                            + LoginManager.FriendsVideos
                            + LoginManager.FriendsWebsite
                            + LoginManager.FriendsWorkHistory
        userPermissions: LoginManager.UserAboutMe
                         + LoginManager.UserActivities
                         + LoginManager.UserBirthday
                         + LoginManager.UserEducationHistory
                         + LoginManager.UserEvents
                         + LoginManager.UserGroups
                         + LoginManager.UserHometown
                         + LoginManager.UserInterests
                         + LoginManager.UserLikes
                         + LoginManager.UserLocation
                         + LoginManager.UserNotes
                         + LoginManager.UserPhotos
                         + LoginManager.UserQuestions
                         + LoginManager.UserRelationships
                         + LoginManager.UserRelationshipDetails
                         + LoginManager.UserReligionPolitics
                         + LoginManager.UserStatus
                         + LoginManager.UserSubscriptions
                         + LoginManager.UserVideos
                         + LoginManager.UserWebsite
                         + LoginManager.UserWorkHistory
                         + LoginManager.Email
        extendedPermissions: LoginManager.ReadFriendList
                             + LoginManager.ReadInsights
                             + LoginManager.ReadMailbox
                             + LoginManager.ReadRequests
                             + LoginManager.ReadStream
                             + LoginManager.XmppLogin
                             + LoginManager.AdsManagement
                             + LoginManager.CreateEvent
                             + LoginManager.ManageFriendList
                             + LoginManager.ManageNotifications
                             + LoginManager.UserOnlinePresence
                             + LoginManager.FriendsOnlinePresence
                             + LoginManager.PublishCheckins
                             + LoginManager.PublishStream
                             + LoginManager.RsvpEvent

        Component.onCompleted: login()
        onUrlRequested: webView.url = url
        onLoginSucceeded: {
            tokenManager.token = token
            container.connected()
        }
    }

    SilicaWebView {
        id: webView
        anchors.top: header.bottom; anchors.bottom: parent.bottom
        anchors.left: parent.left; anchors.right: parent.right
        onUrlChanged: loginManager.checkUrl(url)
    }
}
