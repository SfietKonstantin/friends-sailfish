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


Item {
    id: container
    property alias post: helper.post
    property alias from: helper.from
    property alias to: helper.to
    property alias fancy: helper.fancy
    width: parent.width
    height: background.height + Theme.paddingMedium

    Rectangle {
        anchors.fill: background
        color: Theme.rgba(Theme.highlightBackgroundColor, 0.2)
    }

    BackgroundItem {
        id: background
        anchors.left: parent.left; anchors.leftMargin: Theme.paddingMedium
        anchors.right: parent.right; anchors.rightMargin: Theme.paddingMedium
        anchors.verticalCenter: parent.verticalCenter
        height: column.height + 2 * Theme.paddingMedium
    }

    Column {
        id: column
        anchors.left: background.left; anchors.leftMargin: Theme.paddingMedium
        anchors.right: background.right; anchors.rightMargin: Theme.paddingMedium
        spacing: Theme.paddingMedium
        anchors.verticalCenter: parent.verticalCenter

        PostHelper {
            id: helper
            highlightColor: Theme.highlightColor
        }

        // Header
        Item {
            anchors.left: parent.left; anchors.right: parent.right
            height: childrenRect.height
            FacebookPicture {
                id: picture
                identifier: model.contentItem.from.objectIdentifier
                imageWidth: Theme.iconSizeMedium
                imageHeight: Theme.iconSizeMedium
            }

            Item {
                anchors.left: picture.right; anchors.leftMargin: Theme.paddingMedium
                anchors.right: parent.right; anchors.rightMargin: Theme.paddingMedium
                height: childrenRect.height

                Label {
                    id: header
                    anchors.left: parent.left; anchors.right: parent.right
                    textFormat: Text.RichText
                    text: helper.header
                    font.pixelSize: Theme.fontSizeSmall
                    truncationMode: TruncationMode.Fade
                }

                Label {
                    anchors.top: header.bottom; anchors.topMargin: Theme.paddingSmall
                    anchors.left: parent.left; anchors.right: parent.right
                    text: Format.formatDate(DateHelper.fromString(container.post.createdTime),
                                            Formatter.DurationElapsed)
                    font.pixelSize: Theme.fontSizeExtraSmall
                    color: Theme.secondaryColor
                }
            }
        }

        // Content
        Label {
            anchors.left: parent.left; anchors.right: parent.right
            text: helper.message
            wrapMode: Text.WordWrap
            font.pixelSize: Theme.fontSizeSmall
        }


        //Images (an attempt)
        Rectangle {
            id: imagesContainer

            function preprocess(text) {
                return text.replace(/\n/g, " ")
            }

            visible: post.media.length > 0
            property real cellSize: post.media.length <= 1 ? width : width / 3
            anchors.left: parent.left; anchors.right: parent.right
            color: Theme.rgba(Theme.highlightBackgroundColor, 0.2)
            height: grid.height + (attachment.visible ? Theme.paddingMedium * 2 + attachment.height : 0)

            Grid {
                id: grid
                columns: post.media.length <= 1 ? 1 : 3
                rows: post.media.length <= 1 ? 1 : 2
                anchors.left: parent.left; anchors.right: parent.right
                Repeater {
                    model: post.media
                    delegate: FacebookImage {
                        url: modelData
                        width: imagesContainer.cellSize
                        height: post.media.length <= 1 ? 2 / 3 * imagesContainer.cellSize
                                                       : imagesContainer.cellSize
                    }
                }
            }

            Column {
                id: attachment
                anchors.top: grid.bottom; anchors.topMargin: Theme.paddingMedium
                anchors.left: parent.left; anchors.right: parent.right
                spacing: Theme.paddingSmall
                visible: post.name.length > 0 || post.caption.length > 0 || post.description.length > 0
                Label {
                    text: imagesContainer.preprocess(post.name)
                    visible: post.name.length > 0
                    anchors.left: parent.left; anchors.leftMargin: Theme.paddingMedium
                    anchors.right: parent.right; anchors.rightMargin: Theme.paddingMedium
                    font.pixelSize: Theme.fontSizeSmall
                    truncationMode: TruncationMode.Fade
                }

                Label {
                    text: imagesContainer.preprocess(post.description)
                    visible: post.description.length > 0
                    anchors.left: parent.left; anchors.leftMargin: Theme.paddingMedium
                    anchors.right: parent.right; anchors.rightMargin: Theme.paddingMedium
                    font.pixelSize: Theme.fontSizeExtraSmall
                    truncationMode: TruncationMode.Fade
                }

                Label {
                    text: imagesContainer.preprocess(post.caption)
                    visible: post.caption.length > 0
                    anchors.left: parent.left; anchors.leftMargin: Theme.paddingMedium
                    anchors.right: parent.right; anchors.rightMargin: Theme.paddingMedium
                    font.pixelSize: Theme.fontSizeExtraSmall
                    truncationMode: TruncationMode.Fade
                    color: Theme.secondaryColor
                }
            }
        }

        // Likes comments
        Label {
            anchors.left: parent.left; anchors.right: parent.right
            font.pixelSize: Theme.fontSizeExtraSmall
            color: Theme.secondaryColor
            text: helper.footer
        }
    }
}

