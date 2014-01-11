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
import harbour.friends.social 1.0

Page {
    function load() {
        if (model.status == SocialNetwork.Idle || model.status == SocialNetwork.Error) {
            model.load()
        }
    }

    onStatusChanged: {
        if (status == PageStatus.Active) {
            pageStack.pushAttached(menuPage)
        }
    }

    StateIndicator {
        model: model
    }

    SilicaListView {
        id: view
        anchors.fill: parent
        visible: model.status == SocialNetwork.Idle || model.count > 0
        model: SocialNetworkModel {
            id: model
            socialNetwork: facebook
            filter: FacebookRelatedDataFilter {
                identifier: facebook.currentUserIdentifier
                connection: Facebook.Notifications
                fields: "from,title,link,created_time,unread,object.id"
                limit: 20
            }
        }

        header: PageHeader {
            //: Title of the page showing the list of notifications
            //% "Notifications"
            title: qsTrId("friends_notifications_title")
        }

        delegate: BackgroundItem {
            id: background
            property string identifier: NotificationsHelper.getObject(model.contentItem.data)
            height: Math.max(message.height + itemFooter.height, avatar.height) + Theme.paddingLarge
            enabled: identifier.length > 0

            GlassItem {
                id: glass
                anchors.verticalCenter: avatar.verticalCenter
                anchors.horizontalCenter: parent.left
                visible: model.contentItem.unread !== 0
            }

            FacebookPicture {
                id: avatar
                anchors.top: parent.top; anchors.topMargin: Theme.paddingLarge / 2
                anchors.left: glass.right
                identifier: model.contentItem.from.objectIdentifier
                pictureWidth: Theme.iconSizeMedium
                pictureHeight: Theme.iconSizeMedium
            }

            Label {
                id: message
                anchors.left: avatar.right; anchors.leftMargin: Theme.paddingMedium
                anchors.top: parent.top; anchors.topMargin: Theme.paddingLarge / 2
                anchors.right: parent.right; anchors.rightMargin: Theme.paddingLarge
                text: model.contentItem.title
                font.pixelSize: Theme.fontSizeSmall
                horizontalAlignment: Text.AlignLeft
                wrapMode: Text.Wrap
            }

            Label {
                id: itemFooter
                anchors.left: avatar.right; anchors.leftMargin: Theme.paddingMedium
                anchors.top: message.bottom
                anchors.right: parent.right;anchors.rightMargin: Theme.paddingLarge
                opacity: 0.6
                width: parent.width
                font.pixelSize: Theme.fontSizeExtraSmall
                text: Format.formatDate(DateHelper.fromString(model.contentItem.createdTime),
                                        Formatter.DurationElapsed)
            }

            onClicked: {
                model.contentItem.markAsRead()
                var page = pageStack.push(Qt.resolvedUrl("TypeSolverPage.qml"),
                                          {"identifier": background.identifier})
                page.load()
            }
        }

        onAtYEndChanged: {
            if (atYEnd && model.hasNext) {
                model.loadNext()
            }
        }

        VerticalScrollDecorator {}

        ViewPlaceholder {
            enabled: model.status == SocialNetwork.Idle && model.count == 0
            //: Text shown on the placeholder, where there is no notifications to be displayed
            //% "No notifications"
            text: qsTrId("friends_notifications_placeholder")
        }
    }
}
