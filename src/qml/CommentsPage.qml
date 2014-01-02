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
    id: container

    function load() {
        model.load()
    }

    onStatusChanged: {
        if (status == PageStatus.Active) {
            pageStack.pushAttached(menuPage)
        }
    }

    property string identifier
    property Component headerComponent
    property var headerProperties
    property var item

    SocialNetworkModel {
        id: model
        socialNetwork: facebook
        filter: FacebookRelatedDataFilter {
            identifier: container.identifier
            connection: Facebook.Comments
            fields: "from,message,created_time"
        }
    }

    SilicaListView {
        id: view
        anchors.fill: parent
        model: model
        spacing: Theme.paddingLarge
        header: Column {
            width: view.width
            PageHeader {
                title: qsTr("Comments")
            }

            Item {
                id: header
                property Item object
                width: parent.width
                height: childrenRect.height

                Component.onCompleted: {
                    header.object = container.headerComponent.createObject(header, headerProperties)
                }
                Component.onDestruction: object.destroy()
            }
        }
        footer: Item {
            id: footer
            property bool posting: false
            property bool displayMargins: model.count > 0
            enabled: item.status == SocialNetwork.Idle
                     && item.actionStatus == SocialNetwork.Idle
                     && model.status == SocialNetwork.Idle
            anchors.left: parent.left
            anchors.right: parent.right
            height: childrenRect.height  + Theme.paddingMedium
                    + (displayMargins ? Theme.paddingLarge : 0)

            Connections {
                target: item
                onActionStatusChanged: {
                    if (item.actionStatus == SocialNetwork.Idle && footer.posting) {
                        model.load()
                        textField.text = ""
                        footer.posting = false
                    }
                }
            }

            FacebookPicture {
                id: commentAvatar
                anchors.left: parent.left; anchors.leftMargin: Theme.paddingMedium
                anchors.top: parent.top; anchors.topMargin: footer.displayMargins ? Theme.paddingLarge : 0
                identifier: facebook.currentUserIdentifier
                imageWidth: Theme.iconSizeMedium
                imageHeight: Theme.iconSizeMedium

            }

            TextField {
                id: textField
                anchors {
                    top: parent.top
                    topMargin: footer.displayMargins ? Theme.paddingLarge : 0
                    left: commentAvatar.right
                    right: parent.right
                }

                EnterKey.highlighted: text.length > 0
                EnterKey.iconSource: "image://theme/icon-m-enter-" + (text.length > 0 ? "accept" : "close")
                EnterKey.onClicked: {
                    if (textField.text.length > 0) {
                        item.uploadComment(textField.text)
                        footer.posting = true
                    }
                }
            }
        }

        delegate: Item {
            width: view.width
            height: Math.max(message.height + itemFooter.height, avatar.height)
            opacity: 0
            Component.onCompleted: {
                opacity = 1
            }
            Behavior on opacity { FadeAnimation {} }

            FacebookPicture {
                id: avatar
                anchors.left: parent.left; anchors.leftMargin: Theme.paddingMedium
                identifier: model.contentItem.from.objectIdentifier
                imageWidth: Theme.iconSizeMedium
                imageHeight: Theme.iconSizeMedium
            }

            Label {
                id: message
                anchors.left: avatar.right; anchors.leftMargin: Theme.paddingMedium
                anchors.top: avatar.top
                anchors.right: parent.right; anchors.rightMargin: Theme.paddingLarge
                text: model.contentItem.message
                font.pixelSize: Theme.fontSizeSmall
                horizontalAlignment: Text.AlignLeft
                wrapMode: Text.Wrap
                color: Theme.highlightColor
            }

            Label {
                id: itemFooter
                anchors.left: avatar.right; anchors.leftMargin: Theme.paddingMedium
                anchors.top: message.bottom
                anchors.right: parent.right;anchors.rightMargin: Theme.paddingLarge
                opacity: 0.6
                color: Theme.highlightColor
                width: parent.width
                font.pixelSize: Theme.fontSizeExtraSmall
                text: model.contentItem.from.objectName + " \u2022 "
                      + Format.formatDate(DateHelper.fromString(model.contentItem.createdTime),
                                          Formatter.DurationElapsed)
            }
        }

        ScrollDecorator {}
    }
}
