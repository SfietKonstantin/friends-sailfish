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
import harbour.friends.social 1.0

Item {
    id: container
    property var item
    signal showComments()
    anchors.left: parent.left; anchors.right: parent.right
    height: childrenRect.height

    QtObject {
        id: privateObject
        property bool loading: container.item.status != SocialNetwork.Idle
                               || container.item.actionStatus != SocialNetwork.Idle
    }


    BackgroundItem {
        id: likeItem
        opacity: privateObject.loading ? 0.5 : 1
        enabled: !privateObject.loading
        Behavior on opacity { FadeAnimation {} }

        anchors.left: parent.left; anchors.right: parent.horizontalCenter

        Image {
            id: likeIcon
            source: "image://theme/icon-s-like"
                    + (likeItem.highlighted ? "?" + Theme.highlightColor : "")
            anchors.left: parent.left; anchors.leftMargin: Theme.paddingLarge
            anchors.verticalCenter: parent.verticalCenter
        }

        Label {
            //: A button to cancel the Facebook "like"
            //% "Unlike"
            text: container.item === null ? "" : (container.item.liked ? qsTrId("friends_button_unlike")
            //: A button to perform the Facebook "like"
            //% "Like"
                                                                       : qsTrId("friends_button_like"))
            anchors.left: likeIcon.right; anchors.leftMargin: Theme.paddingLarge
            anchors.right: parent.right; anchors.rightMargin: Theme.paddingLarge
            anchors.verticalCenter: parent.verticalCenter
            truncationMode: TruncationMode.Fade
            font.pixelSize: Theme.fontSizeExtraSmall
            color: likeItem.highlighted ? Theme.highlightColor : Theme.primaryColor
        }
        onClicked: {
            if (container.item === null) {
                return
            }

            container.item.liked ? container.item.unlike() : container.item.like()
        }
    }

    BackgroundItem {
        id: commentItem
        opacity: privateObject.loading ? 0.5 : 1
        enabled: !privateObject.loading
        Behavior on opacity { FadeAnimation {} }

        anchors.left: parent.horizontalCenter; anchors.right: parent.right

        Image {
            id: commentsIcon
            source: "image://theme/icon-s-chat"
                    + (commentItem.highlighted ? "?" + Theme.highlightColor : "")
            anchors.left: parent.left; anchors.leftMargin: Theme.paddingLarge
            anchors.verticalCenter: parent.verticalCenter
        }

        Label {
            //: A button to show or perform a comment
            //% "Comment"
            text: qsTrId("friends_button_comment")
            anchors.left: commentsIcon.right; anchors.leftMargin: Theme.paddingLarge
            anchors.right: parent.right; anchors.rightMargin: Theme.paddingLarge
            anchors.verticalCenter: parent.verticalCenter
            truncationMode: TruncationMode.Fade
            font.pixelSize: Theme.fontSizeExtraSmall
            color: commentItem.highlighted ? Theme.highlightColor : Theme.primaryColor
        }
        onClicked: container.showComments()
    }
}
