
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

Page {
    id: container
    property string identifier
    function load() {
        model.load()
    }

    onStatusChanged: {
        if (status == PageStatus.Active) {
            pageStack.pushAttached(menuPage)
        }
    }

    StateIndicator {
        busy: model.status == SocialNetwork.Busy && model.count == 0
        error: model.status == SocialNetwork.Error && model.count == 0
        onReload: container.load()
    }

    SilicaListView {
        anchors.fill: parent
        visible: model.status == SocialNetwork.Idle || model.count > 0
        model: SocialNetworkModel {
            id: model
            socialNetwork: facebook
            filter: FacebookRelatedDataFilter {
                identifier: container.identifier
                connection: Facebook.Albums
                limit: 10
            }
        }

        header: PageHeader {
            title: qsTr("Albums")
        }

        delegate: Item {
            width: parent.width
            height: 2 * Theme.itemSizeExtraLarge + Theme.paddingMedium

            BackgroundItem {
                id: background
                anchors.left: parent.left; anchors.leftMargin: Theme.paddingMedium
                anchors.right: parent.right; anchors.rightMargin: Theme.paddingMedium
                anchors.top: parent.top; anchors.topMargin: Theme.paddingMedium / 2
                anchors.bottom: parent.bottom; anchors.bottomMargin: Theme.paddingMedium / 2

                Item {
                    id: image
                    opacity: background.down ? Theme.highlightBackgroundOpacity : 1
                    anchors.left: parent.left; anchors.right: parent.right
                    height: childrenRect.height
                    Rectangle {
                        visible: !background.down
                        anchors.fill: parent
                        color: Theme.rgba(Theme.highlightBackgroundColor, 0.2)
                    }
                    FacebookPicture {
                        identifier: model.contentItem.coverPhoto
                        anchors.left: parent.left; anchors.right: parent.right
                        imageWidth: Screen.width - 2 * Theme.paddingMedium
                        imageHeight: 2 * Theme.itemSizeExtraLarge
                        height: imageHeight
                    }
                }

                Rectangle {
                    anchors.bottom: image.bottom
                    anchors.left: image.left; anchors.right: image.right
                    height: Theme.paddingLarge + Theme.fontSizeMedium + Theme.paddingMedium
                    opacity: 0.8
                    gradient: Gradient {
                        GradientStop {position: 0; color: "#00000000"}
                        GradientStop {position: 1; color: "black"}
                    }
                }

                Label {
                    anchors.left: parent.left; anchors.leftMargin: Theme.paddingMedium
                    anchors.right: parent.right; anchors.rightMargin: Theme.paddingMedium
                    anchors.bottom: image.bottom; anchors.bottomMargin: Theme.paddingMedium
                    text: model.contentItem.name
                    truncationMode: TruncationMode.Fade
                }
            }

            MouseArea {
                id: mouseArea
                anchors.fill: parent
            }
        }

        onAtYEndChanged: {
            if (atYEnd && model.hasNext) {
                model.loadNext()
            }
        }

        VerticalScrollDecorator {}
    }
}
