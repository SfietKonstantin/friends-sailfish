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

Rectangle {
    id: container
    property variant item
    property alias description: description.text
    signal showComments()
    anchors.fill: parent
    color: Theme.rgba(Theme.highlightBackgroundColor, 0.1)

    Column {
        id: column
        property bool loading: container.item.status != SocialNetwork.Idle
                               || container.item.actionStatus != SocialNetwork.Idle
        anchors.left: parent.left; anchors.right: parent.right
        spacing: Theme.paddingMedium

        Row {
            spacing: Theme.paddingLarge
            height: Theme.itemSizeLarge
            anchors.right: parent.right
            anchors.rightMargin: Theme.paddingLarge

            Image {
                opacity: column.loading ? 0.5 : 1
                source: "image://theme/icon-s-like" + "?" + Theme.highlightColor
                anchors.verticalCenter: parent.verticalCenter
            }

            Label {
                opacity: column.loading ? 0.5 : 1
                color: Theme.highlightColor
                text: item === null ? "" : item.likesCount
                anchors.verticalCenter: parent.verticalCenter
            }

            Image {
                opacity: column.loading ? 0.5 : 1
                source: "image://theme/icon-s-chat" + "?" + Theme.highlightColor
                anchors.verticalCenter: parent.verticalCenter
            }

            Label {
                opacity: column.loading ? 0.5 : 1
                color: Theme.highlightColor
                text: item === null ? "" : item.commentsCount
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        Label {
            id: description
            anchors.left: parent.left; anchors.leftMargin: Theme.paddingMedium
            anchors.right: parent.right; anchors.rightMargin: Theme.paddingMedium
            wrapMode: Text.Wrap
            maximumLineCount: 4
            elide: Text.ElideRight
            visible: text.length > 0
        }

        Label {
            anchors.left: parent.left; anchors.leftMargin: Theme.paddingMedium
            anchors.right: parent.right; anchors.rightMargin: Theme.paddingMedium
            wrapMode: Text.WordWrap
            font.pixelSize: Theme.fontSizeSmall
            color: Theme.secondaryColor
            //: Display a message about the last updated time. %1 will be replaced by the elapsed time, like "6 weeks ago"
            //% "Last update %1"
            text: qsTrId("friends_splitsocial_last_update").arg(Format.formatDate(DateHelper.fromString(item === null ? "" : item.updatedTime), Formatter.DurationElapsed))
        }
    }

    SocialButtons {
        anchors.bottom: parent.bottom; anchors.bottomMargin: Theme.paddingMedium
        item: container.item
        onShowComments: container.showComments()
    }
}
