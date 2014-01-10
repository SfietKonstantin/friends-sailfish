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
    property int currentIndex
    property var model
    property bool isFacebookModel: true
    allowedOrientations: Orientation.All

    function load() {
        view.positionViewAtIndex(container.currentIndex, ListView.Center)
    }

    onStatusChanged: {
        if (status == PageStatus.Active) {
            pageStack.pushAttached(menuPage)
        }
    }

    Drawer {
        id: drawer
        anchors.fill: parent
        dock: container.orientation == Orientation.Portrait ? Dock.Top: Dock.Left

        background: SplitSocialPanel {
            property int realIndex: view.currentIndex < 0 || view.currentIndex >= container.model.count ? 0 : view.currentIndex
            item: container.isFacebookModel ? model.relatedItem(realIndex) : model.get(realIndex).contentItem
            onItemChanged: console.debug(item)
            description: item.name
            onShowComments: {
                var headerProperties = {"identifier": item.identifier,
                                        "photo": item}
                var page = pageStack.push(Qt.resolvedUrl("CommentsPage.qml"),
                                          {"identifier": item.identifier,
                                           "item": item,
                                           "headerComponent": headerComponent,
                                           "headerProperties": headerProperties})
                page.load()
            }
        }

        foreground: ListView {
            id: view
            onVisibleChanged: {
                if (visible) {
                    view.positionViewAtIndex(container.currentIndex, ListView.Contain)
                }
            }
            property int imageSize: Math.max(Screen.width, Screen.height)
            anchors.fill: parent
            model: visible ? container.model : null
            interactive: visible ? model.count > 1 : false
            orientation: Qt.Horizontal
            highlightRangeMode: ListView.StrictlyEnforceRange
            snapMode: ListView.SnapOneItem
            onAtXEndChanged: {
                if (view.model === null) {
                    return
                }

                if (container.isFacebookModel) {
                    if (atXEnd && view.model.hasNext) {
                        view.model.loadNext()
                    }
                }
            }

            delegate: Item {
                width: view.width
                height: view.height
                FacebookPicture {
                    identifier: model.contentItem.identifier
                    anchors.fill: parent
                    pictureWidth: view.imageSize
                    pictureHeight: view.imageSize
                    fillMode: Image.PreserveAspectFit
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: !drawer.open ? drawer.show() : drawer.hide()
                }
            }
        }
    }

    // Used to comment a photo
    Component {
        id: headerComponent
        Item {
            property alias identifier: picture.identifier
            property FacebookPhoto photo
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
                    id: photoContainer
                    anchors.top: parent.top; anchors.topMargin: Theme.paddingMedium
                    anchors.left: parent.left; anchors.leftMargin: Theme.paddingMedium
                    anchors.right: parent.right; anchors.rightMargin: Theme.paddingMedium
                    color: Theme.rgba(Theme.highlightBackgroundColor, 0.2)
                    height: column.height + (label.visible ? Theme.paddingMedium : 0)

                    Column {
                        id: column
                        spacing: Theme.paddingMedium
                        anchors.left: parent.left; anchors.right: parent.right

                        FacebookPicture {
                            id: picture
                            property int imageSize: Math.max(Screen.width, Screen.height)
                            anchors.left: parent.left; anchors.right: parent.right
                            height: width * 2 / 3
                            pictureWidth: imageSize
                            pictureHeight: imageSize
                        }

                        Label {
                            id: label
                            anchors.left: parent.left; anchors.leftMargin: Theme.paddingMedium
                            anchors.right: parent.right; anchors.rightMargin: Theme.paddingMedium
                            font.pixelSize: Theme.fontSizeSmall
                            text: photo.name
                            visible: photo.name.length > 0
                            wrapMode: Text.Wrap
                        }
                    }
                }
                Label {
                    id: footer
                    anchors.top: photoContainer.bottom; anchors.topMargin: Theme.paddingMedium
                    anchors.left: parent.left; anchors.leftMargin: Theme.paddingMedium
                    anchors.right: parent.right; anchors.rightMargin: Theme.paddingMedium
                    font.pixelSize: Theme.fontSizeExtraSmall
                    color: Theme.secondaryColor
                    text: FooterHelper.makeFooter(photo.likesCount, photo.commentsCount)
                }
            }
        }
    }
}


