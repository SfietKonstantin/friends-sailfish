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
    property alias post: helper.object
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

        function solveLink(link) {
            var splitted = link.split("----")
            if (splitted.length !== 2) {
                return
            }

            if (splitted[0] == "item") {
                var page = pageStack.push(Qt.resolvedUrl("TypeSolverPage.qml"),
                                          {"identifier": splitted[1]})
                page.load()
            } else if (splitted[0] == "url") {
                Qt.openUrlExternally(splitted[1])
            }
        }

        anchors.left: parent.left; anchors.leftMargin: Theme.paddingMedium
        anchors.right: parent.right; anchors.rightMargin: Theme.paddingMedium
        anchors.verticalCenter: parent.verticalCenter
        height: column.height + 2 * Theme.paddingMedium
        onClicked: {
            var headerProperties = {"post": container.post}
            var page = pageStack.push(Qt.resolvedUrl("CommentsPage.qml"),
                                      {"identifier": container.post.identifier,
                                       "item": container.post,
                                       "headerComponent": postHeaderComponent,
                                       "headerProperties": headerProperties})
            page.load()
        }
        enabled: !container.fancy
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

        Component.onCompleted: {
            // Add the message tags
            for (var i = 0; i < container.post.messageTags.length; i++) {
                helper.addMessageTag(container.post.messageTags[i])
            }

            // Add the story tags
            for (i = 0; i < container.post.storyTags.length; i++) {
                helper.addStoryTag(container.post.storyTags[i])
            }
        }

        // Header
        Item {
            anchors.left: parent.left; anchors.right: parent.right
            height: childrenRect.height
            FacebookPicture {
                id: picture
                identifier: container.post.from.objectIdentifier
                pictureWidth: Theme.iconSizeMedium
                pictureHeight: Theme.iconSizeMedium
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
                    onLinkActivated: background.solveLink(link)
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
            textFormat: Text.RichText
            text: helper.message
            wrapMode: Text.Wrap
            font.pixelSize: Theme.fontSizeSmall
            onLinkActivated: background.solveLink(link)
        }

        //Images
        Rectangle {
            id: imagesContainer

            function preprocess(text) {
                return text.replace(/\n/g, " ")
            }

            visible: post.media.length > 0
            property real cellWidth: post.media.length <= 1 ? grid.width : grid.width / 3
            property real cellHeight: post.media.length <= 1 ? grid.width * (small ? 1 : 2 / 3)
                                                             : grid.width / 3
            property bool small: false

            anchors.left: parent.left
            width: !(small && !attachment.visible) ? parent.width : grid.width + 2 * Theme.paddingMedium
            color: !imagesContainerMouseArea.pressed ? Theme.rgba(Theme.highlightBackgroundColor, 0.2)
                                                     : Theme.rgba(Theme.highlightBackgroundColor,
                                                                  Theme.highlightBackgroundOpacity)
            height: !small ? grid.height + (attachment.visible ? Theme.paddingMedium * 2 + attachment.height : 0)
                           : Math.max(grid.height, attachment.height) + 2 * Theme.paddingMedium

            MouseArea {
                id: imagesContainerMouseArea
                enabled: post.source != "" || post.facebookObjectId != ""
                anchors.fill: parent
                onClicked: {
                    if (post.source != "") {
                        Qt.openUrlExternally(post.source)
                    } else {
                        var page = pageStack.push(Qt.resolvedUrl("TypeSolverPage.qml"),
                                                  {"identifier": post.facebookObjectId,
                                                   "type": post.facebookObjectType,
                                                   "fql": true})
                        page.load()
                    }
                }
            }


            Grid {
                id: grid
                columns: post.media.length <= 1 ? 1 : 3
                rows: post.media.length <= 1 ? 1 : 2
                anchors.left: parent.left
                anchors.leftMargin: !imagesContainer.small ? 0 : Theme.paddingMedium
                anchors.verticalCenter: !imagesContainer.small ? undefined : parent.verticalCenter
                width: !imagesContainer.small ? imagesContainer.width : Theme.iconSizeLarge
                opacity: imagesContainerMouseArea.pressed ? Theme.highlightBackgroundOpacity : 1

                Repeater {
                    model: post.media
                    delegate: FacebookImage {
                        id: delegate
                        function checkSize() {
                            if (status != Image.Ready) {
                                return
                            }

                            if (post.media.length <= 1) {
                                if (sourceSize.width < column.width / 2 || sourceSize.height < column.width / 3) {
                                    imagesContainer.small = true
                                }
                            }
                        }

                        url: modelData
                        onSourceSizeChanged: checkSize()
                        width: imagesContainer.cellWidth
                        height: imagesContainer.cellHeight

                        Connections {
                            target: column
                            onWidthChanged: delegate.checkSize()
                        }
                    }
                }
            }

            Column {
                id: attachment
                anchors.top: !imagesContainer.small ? grid.bottom : undefined
                anchors.topMargin: !imagesContainer.small ? Theme.paddingMedium : 0
                anchors.verticalCenter: !imagesContainer.small ? undefined : parent.verticalCenter
                anchors.left: !imagesContainer.small ? parent.left : grid.right
                anchors.right: parent.right
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

