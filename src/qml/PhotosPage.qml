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
    property string identifier
    property string name
    property bool isUserPhotos: false
    Component.onCompleted: menu.visible = !isUserPhotos
    onIsUserPhotosChanged: menu.visible = !isUserPhotos
    function load() {
        if (model.status == SocialNetwork.Idle || model.status == SocialNetwork.Error) {
            model.load()
        }

        if (isUserPhotos) {
            return
        }

        if (album.status == SocialNetwork.Idle || album.status == SocialNetwork.Error) {
            album.load()
        }
    }

    FacebookAlbum {
        id: album
        socialNetwork: facebook
        filter: FacebookItemFilter {
            identifier: container.identifier
            fields: "id,description,likes,comments,updated_time"
        }
    }

    onStatusChanged: {
        if (status == PageStatus.Active) {
            pageStack.pushAttached(menuPage)
        }
    }

    Drawer {
        id: drawer
        property bool loading: album.status != SocialNetwork.Idle
                               || album.actionStatus != SocialNetwork.Idle
        anchors.fill: parent
        dock: container.orientation == Orientation.Portrait ? Dock.Bottom: Dock.Left
        background: Rectangle {
            anchors.fill: parent
            color: Theme.rgba(Theme.highlightBackgroundColor, 0.1)

            Column {
                anchors.left: parent.left; anchors.right: parent.right
                spacing: Theme.paddingMedium

                Row {
                    spacing: Theme.paddingLarge
                    height: Theme.itemSizeLarge
                    anchors.right: parent.right
                    anchors.rightMargin: Theme.paddingLarge

                    Image {
                        opacity: drawer.loading ? 0.5 : 1
                        source: "image://theme/icon-s-like" + "?" + Theme.highlightColor
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    Label {
                        opacity: drawer.loading ? 0.5 : 1
                        color: Theme.highlightColor
                        text: album.likesCount
                        anchors.verticalCenter: parent.verticalCenter
                        width: Theme.paddingLarge
                    }

                    Image {
                        opacity: drawer.loading ? 0.5 : 1
                        source: "image://theme/icon-s-chat" + "?" + Theme.highlightColor
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    Label {
                        opacity: drawer.loading ? 0.5 : 1
                        color: Theme.highlightColor
                        text: album.commentsCount
                        anchors.verticalCenter: parent.verticalCenter
                        width: Theme.paddingLarge
                    }
                }

                Label {
                    anchors.left: parent.left; anchors.leftMargin: Theme.paddingMedium
                    anchors.right: parent.right; anchors.rightMargin: Theme.paddingMedium
                    wrapMode: Text.WordWrap
                    text: album.description
                    visible: album.description.length > 0
                }

                Label {

                    anchors.left: parent.left; anchors.leftMargin: Theme.paddingMedium
                    anchors.right: parent.right; anchors.rightMargin: Theme.paddingMedium
                    wrapMode: Text.WordWrap
                    font.pixelSize: Theme.fontSizeSmall
                    color: Theme.secondaryColor
                    text: qsTr("Last update %1").arg(Format.formatDate(DateHelper.fromString(album.updatedTime),
                                                                       Formatter.DurationElapsed))
                }
            }

            Item {
                anchors.left: parent.left; anchors.right: parent.right
                anchors.bottom: parent.bottom; anchors.bottomMargin: Theme.paddingMedium
                height: childrenRect.height

                BackgroundItem {
                    id: likeItem
                    opacity: drawer.loading ? 0.5 : 1
                    enabled: !drawer.loading
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
                        text: album.liked ? qsTr("Unlike") : qsTr("Like")
                        anchors.left: likeIcon.right; anchors.leftMargin: Theme.paddingLarge
                        anchors.right: parent.right; anchors.rightMargin: Theme.paddingLarge
                        anchors.verticalCenter: parent.verticalCenter
                        truncationMode: TruncationMode.Fade
                        font.pixelSize: Theme.fontSizeExtraSmall
                        color: likeItem.highlighted ? Theme.highlightColor : Theme.primaryColor
                    }
                    onClicked: album.liked ? album.unlike() : album.like()
                }

                BackgroundItem {
                    id: commentItem
                    opacity: drawer.loading ? 0.5 : 1
                    enabled: !drawer.loading
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
                        text: qsTr("Comment")
                        anchors.left: commentsIcon.right; anchors.leftMargin: Theme.paddingLarge
                        anchors.right: parent.right; anchors.rightMargin: Theme.paddingLarge
                        anchors.verticalCenter: parent.verticalCenter
                        truncationMode: TruncationMode.Fade
                        font.pixelSize: Theme.fontSizeExtraSmall
                        color: commentItem.highlighted ? Theme.highlightColor : Theme.primaryColor
                    }
                    // onClicked: <do something>
                }
            }
        }
        foreground: Item {
            anchors.fill: parent
            StateIndicator {
                busy: model.status == SocialNetwork.Busy && model.count == 0
                error: model.status == SocialNetwork.Error && model.count == 0
                onReload: container.load()
            }

            SilicaGridView {
                id: view
                property int gridSize: view.width / 3
                cellWidth: gridSize
                cellHeight: gridSize
                anchors.fill: parent
                visible: model.status == SocialNetwork.Idle || model.count > 0
                model: SocialNetworkModel {
                    id: model
                    socialNetwork: facebook
                    filter: FacebookRelatedDataFilter {
                        identifier: container.identifier
                        connection: Facebook.Photos
                        limit: 21
                    }
                }

                header: PageHeader {
                    title: container.name
                }

                delegate: Item {
                    width: view.gridSize
                    height: view.gridSize

                    BackgroundItem {
                        id: background
                        anchors.fill: parent

                        Item {
                            id: image
                            opacity: background.down ? Theme.highlightBackgroundOpacity : 1
                            anchors.fill: parent
                            Rectangle {
                                visible: !background.down
                                anchors.fill: parent
                                color: Theme.rgba(Theme.highlightBackgroundColor, 0.2)
                            }
                            FacebookPicture {
                                identifier: model.contentItem.identifier
                                anchors.centerIn: parent
                                imageWidth: parent.width
                                imageHeight: parent.height
                            }
                        }

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
                    text: qsTr("No pictures")
                }

                PullDownMenu {
                    id: menu
                    visible: !container.isUserPhotos
                    MenuItem {
                        text: qsTr("Album informations")
                        onClicked: !drawer.open ? drawer.show() : drawer.hide()
                    }
                }
            }
        }
    }
}
