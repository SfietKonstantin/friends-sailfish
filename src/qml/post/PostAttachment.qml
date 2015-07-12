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
import ".."

Item {
    id: container
    signal clicked()
    property QtObject helper
    anchors.left: parent.left; anchors.right: parent.right
    height: childrenRect.height
    visible: privateData.item !== null

    MouseArea {
        id: attachmentMouseArea
        anchors.fill: parent
        onClicked: container.clicked()
    }

    QtObject {
        id: privateData
        property Item item: null
    }

    Connections {
        target: helper
        onLayoutTypeChanged: {
            if (privateData.item !== null) {
                privateData.item.destroy()
            }

            switch (helper.layoutType) {
            case PostHelper.Share:
                privateData.item = shareAttachmentComponent.createObject(container)
                break
            case PostHelper.Photo:
                privateData.item = photoAttachmentComponent.createObject(container)
                break
            case PostHelper.Album:
                privateData.item = albumAttachmentComponent.createObject(container)
                break
            case PostHelper.Video:
                privateData.item = shareAttachmentComponent.createObject(container, {video: true})
                break
            case PostHelper.None:
                break
            default:
                privateData.item = unknownComponent.createObject(container)
                break
            }
        }
    }

    Component {
        id: shareAttachmentComponent

        Rectangle {
            id: share
            property bool video: false
            anchors.left: parent.left; anchors.right: parent.right
            height: column.height + Theme.paddingMedium + (image.visible ? 0 : Theme.paddingMedium)
            color: !attachmentMouseArea.pressed ? Theme.rgba(Theme.highlightBackgroundColor, 0.2)
                                                : Theme.rgba(Theme.highlightBackgroundColor,
                                                             Theme.highlightBackgroundOpacity)

            Column {
                id: column
                anchors.top: parent.top
                anchors.left: parent.left; anchors.right: parent.right
                spacing: Theme.paddingSmall
                anchors.topMargin: image.visible ? 0 : Theme.paddingMedium

                FacebookImage {
                    id: image
                    anchors.left: parent.left; anchors.right: parent.right
                    visible: container.helper.attachment.length > 0
                    height: container.helper.attachment.length > 0  ? (width / container.helper.attachmentSize.width * container.helper.attachmentSize.height ) : 0
                    url: container.helper.attachment

                    Image {
                        id: videoImage
                        visible: share.video
                        anchors.centerIn: parent
                        source: "image://theme/icon-m-play"
                    }
                }

                Label {
                    text: container.helper.attachmentTitle
                    visible: container.helper.attachmentTitle.length > 0
                    anchors.left: parent.left; anchors.leftMargin: Theme.paddingMedium
                    anchors.right: parent.right; anchors.rightMargin: Theme.paddingMedium
                    font.pixelSize: Theme.fontSizeSmall
                    wrapMode: Text.WordWrap
                    color: Theme.highlightColor
                }

                Label {
                    text: container.helper.attachmentDescription
                    visible: container.helper.attachmentDescription.length > 0
                    anchors.left: parent.left; anchors.leftMargin: Theme.paddingMedium
                    anchors.right: parent.right; anchors.rightMargin: Theme.paddingMedium
                    font.pixelSize: Theme.fontSizeExtraSmall
                    wrapMode: Text.WordWrap
                    maximumLineCount: 3
                    color: Theme.highlightColor
                }

                Label {
                    text: container.helper.attachmentSource
                    visible: container.helper.attachmentSource.length > 0
                    anchors.left: parent.left; anchors.leftMargin: Theme.paddingMedium
                    anchors.right: parent.right; anchors.rightMargin: Theme.paddingMedium
                    font.pixelSize: Theme.fontSizeExtraSmall
                    truncationMode: TruncationMode.Fade
                    color: Theme.secondaryHighlightColor
                }
            }
        }
    }

    Component {
        id: photoAttachmentComponent

        Rectangle {
            anchors.left: parent.left; anchors.right: parent.right
            height: width / container.helper.attachmentSize.width * container.helper.attachmentSize.height
            color: Theme.rgba(Theme.highlightBackgroundColor, 0.2)

            FacebookImage {
                id: image
                anchors.fill: parent
                url: container.helper.attachment
            }
        }
    }

    Component {
        id: albumAttachmentComponent

        Rectangle {
            anchors.left: parent.left; anchors.right: parent.right
            height: childrenRect.height
            color: Theme.rgba(Theme.highlightBackgroundColor, 0.2)

            Grid {
                id: grid
                anchors.left: parent.left; anchors.right: parent.right
                height: childrenRect.height
                columns: 3
                rows: 2
                Repeater {
                    model: container.helper.subAttachments
                    delegate: FacebookImage {
                        url: modelData
                        width: grid.width / 3
                        height: width
                    }
                }
            }
        }
    }

    Component {
        id: unknownComponent

        Rectangle {
            anchors.left: parent.left; anchors.right: parent.right
            height: Theme.itemSizeSmall
            color: Theme.rgba(Theme.highlightBackgroundColor, 0.2)

            Label {
                id: unknownLabel
                anchors.centerIn: parent
                //% "Unknown component"
                //: Text indicating that the component is not displayed because it is unknown
                text: qsTrId("friends_postdelegate_unknowncomponent")
            }
        }
    }
}
