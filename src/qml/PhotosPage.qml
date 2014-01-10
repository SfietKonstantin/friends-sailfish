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
            fields: "id,name,description,likes,comments,updated_time"
        }
    }

    onStatusChanged: {
        if (status == PageStatus.Active) {
            pageStack.pushAttached(menuPage)
        }
    }

    Drawer {
        id: drawer
        property int imageSize: Math.max(Screen.width, Screen.height)
        anchors.fill: parent
        dock: container.orientation == Orientation.Portrait ? Dock.Bottom: Dock.Left
        background: SplitSocialPanel {
            item: album
            description: album.description
            onShowComments: {
                var headerProperties = {"model": model, "album": item}
                var page = pageStack.push(Qt.resolvedUrl("CommentsPage.qml"),
                                          {"identifier": item.identifier,
                                           "item": item,
                                           "headerComponent": headerComponent,
                                           "headerProperties": headerProperties})
                page.load()
            }
        }

        foreground: Item {
            anchors.fill: parent
            StateIndicator {
                item: album
                model: model
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
                        fields: "id,name,updated_time,likes,comments"
                        limit: 21
                    }
                }

                header: PageHeader {
                    title: container.name.length > 0 ? container.name : album.name
                }

                delegate: Item {
                    width: view.gridSize
                    height: view.gridSize

                    BackgroundItem {
                        id: background
                        anchors.fill: parent
                        onClicked: {
                            var page = pageStack.push(Qt.resolvedUrl("PhotoPage.qml"),
                                                      {"model": view.model, "currentIndex": model.index})
                            page.load()
                        }

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
                                width: parent.width
                                height: parent.height
                                pictureWidth: drawer.imageSize
                                pictureHeight: drawer.imageSize
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
                    //: Text shown on the placeholder, where there is no photos to be displayed
                    //% "No photos"
                    text: qsTrId("friends_photos_placeholder")
                }

                PullDownMenu {
                    id: menu
                    visible: !container.isUserPhotos
                    MenuItem {
                        //: Action that shows the album informations
                        //% "Album informations"
                        text: qsTrId("friends_photos_action_informations")
                        onClicked: !drawer.open ? drawer.show() : drawer.hide()
                    }
                }
            }
        }
    }

    // Used to comment an album
    Component {
        id: headerComponent
        Item {
            property alias model: repeater.model
            property FacebookAlbum album
            anchors.left: parent.left; anchors.right: parent.right
            height: columnContainer.height + 2 * Theme.paddingMedium

            Rectangle {
                id: columnContainer
                anchors.left: parent.left; anchors.leftMargin: Theme.paddingMedium
                anchors.right: parent.right; anchors.rightMargin: Theme.paddingMedium
                anchors.verticalCenter: parent.verticalCenter
                height: childrenRect.height + 2 * Theme.paddingMedium
                color: Theme.rgba(Theme.highlightBackgroundColor, 0.2)

                Rectangle {
                    id: albumContainer
                    anchors.top: parent.top; anchors.topMargin: Theme.paddingMedium
                    anchors.left: parent.left; anchors.leftMargin: Theme.paddingMedium
                    anchors.right: parent.right; anchors.rightMargin: Theme.paddingMedium
                    color: Theme.rgba(Theme.highlightBackgroundColor, 0.2)
                    height: column.height + (label.visible ? Theme.paddingMedium : 0)

                    Column {
                        id: column
                        property int imageSize: Math.max(Screen.width, Screen.height)
                        spacing: Theme.paddingMedium
                        anchors.left: parent.left; anchors.right: parent.right

                        Grid {
                            id: grid
                            columns: 3
                            rows: repeater.count < 3 ? 1 : 2
                            anchors.left: parent.left; anchors.right: parent.right
                            Repeater {
                                id: repeater
                                delegate: FacebookPicture {
                                    identifier: model.contentItem.identifier
                                    width: grid.width / 3
                                    height: grid.width / 3
                                    pictureWidth: column.imageSize
                                    pictureHeight: column.imageSize
                                }
                            }
                        }

                        Label {
                            id: label
                            anchors.left: parent.left; anchors.leftMargin: Theme.paddingMedium
                            anchors.right: parent.right; anchors.rightMargin: Theme.paddingMedium
                            font.pixelSize: Theme.fontSizeSmall
                            text: album.name
                            visible: album.name.length > 0
                            wrapMode: Text.Wrap
                        }
                    }
                }
                Label {
                    id: footer
                    anchors.top: albumContainer.bottom; anchors.topMargin: Theme.paddingMedium
                    anchors.left: parent.left; anchors.leftMargin: Theme.paddingMedium
                    anchors.right: parent.right; anchors.rightMargin: Theme.paddingMedium
                    font.pixelSize: Theme.fontSizeExtraSmall
                    color: Theme.secondaryColor
                    text: FooterHelper.makeFooter(album.likesCount, album.commentsCount)
                }
            }
        }
    }
}
