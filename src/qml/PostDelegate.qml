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

Item {
    id: container
    signal clicked()
    property alias object: helper.object
    property alias fancy: helper.fancy
    width: parent.width
    height: background.height

    Rectangle {
        anchors.fill: background
        color: Theme.rgba(Theme.highlightBackgroundColor, 0.2)
    }

    BackgroundItem {
        id: background
        enabled: false // TODO
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
            primaryColor: Theme.primaryColor
            highlightColor: Theme.highlightColor
        }

        // Header
        Item {
            anchors.left: parent.left; anchors.right: parent.right
            height: childrenRect.height
            FacebookImage {
                id: picture
                visible: helper.fullHeader
                width: helper.fullHeader ? Theme.iconSizeMedium : 0
                height: helper.fullHeader ? Theme.iconSizeMedium : 0
                url: helper.profilePicture
            }

            Column {
                anchors.left: picture.right
                anchors.leftMargin: helper.fullHeader ? Theme.paddingMedium : 0
                anchors.right: parent.right; anchors.rightMargin: Theme.paddingMedium
                spacing: Theme.paddingSmall

                Label {
                    id: header
                    anchors.left: parent.left; anchors.right: parent.right
                    textFormat: Text.RichText
                    text: helper.header
                    font.pixelSize: Theme.fontSizeSmall
                    wrapMode: Text.WordWrap
//                    onLinkActivated: background.solveLink(link)
                }

                Label {
                    anchors.left: parent.left; anchors.right: parent.right
                    visible: helper.fullHeader
                    text: Format.formatDate(helper.timestamp, Formatter.DurationElapsed)
                    font.pixelSize: Theme.fontSizeExtraSmall
                    color: Theme.secondaryHighlightColor
                }
            }
        }

        // Content
        Label {
            visible: helper.message !== ""
            anchors.left: parent.left; anchors.right: parent.right
            textFormat: Text.RichText
            text: helper.message
            wrapMode: Text.Wrap
            font.pixelSize: Theme.fontSizeSmall
        }

        // Attachment
        Rectangle {
            id: attachmentContainer

            anchors.left: parent.left; anchors.right: parent.right
            height: attachment.height + Theme.paddingMedium + (image.visible ? 0 : Theme.paddingMedium)
            color: !attachmentMouseArea.pressed ? Theme.rgba(Theme.highlightBackgroundColor, 0.2)
                                                : Theme.rgba(Theme.highlightBackgroundColor,
                                                             Theme.highlightBackgroundOpacity)
            visible: helper.attachment.length > 0 || helper.attachmentTitle.length > 0
                     || helper.attachmentDescription.length > 0 || helper.attachmentSource.length > 0

            Column {
                id: attachment
                anchors.top: parent.top
                anchors.left: parent.left; anchors.right: parent.right
                spacing: Theme.paddingSmall
                anchors.topMargin: image.visible ? 0 : Theme.paddingMedium

                FacebookImage {
                    id: image
                    anchors.left: parent.left; anchors.right: parent.right
                    visible: helper.attachment.length > 0
                    height: helper.attachment.length > 0  ? (width / helper.attachmentSize.width * helper.attachmentSize.height ) : 0
                    source: helper.attachment
                }

                Label {
                    text: helper.attachmentTitle
                    visible: helper.attachmentTitle.length > 0
                    anchors.left: parent.left; anchors.leftMargin: Theme.paddingMedium
                    anchors.right: parent.right; anchors.rightMargin: Theme.paddingMedium
                    font.pixelSize: Theme.fontSizeSmall
                    wrapMode: Text.WordWrap
                    maximumLineCount: 2
                    color: Theme.highlightColor
                }

                Label {
                    text: helper.attachmentDescription
                    visible: helper.attachmentDescription.length > 0
                    anchors.left: parent.left; anchors.leftMargin: Theme.paddingMedium
                    anchors.right: parent.right; anchors.rightMargin: Theme.paddingMedium
                    font.pixelSize: Theme.fontSizeExtraSmall
                    wrapMode: Text.WordWrap
                    maximumLineCount: 3
                    color: Theme.highlightColor
                }

                Label {
                    text: helper.attachmentSource
                    visible: helper.attachmentSource.length > 0
                    anchors.left: parent.left; anchors.leftMargin: Theme.paddingMedium
                    anchors.right: parent.right; anchors.rightMargin: Theme.paddingMedium
                    font.pixelSize: Theme.fontSizeExtraSmall
                    truncationMode: TruncationMode.Fade
                    color: Theme.secondaryHighlightColor
                }
            }

            MouseArea {
                id: attachmentMouseArea
                enabled: false
//                enabled: post.source != "" || post.facebookObjectId != "" || post.objectIdentifier != ""
                anchors.fill: parent
//                onClicked: {
//                    if (post.source != "") {
//                        Qt.openUrlExternally(post.source)
//                    } else {
//                        var objectId = post.facebookObjectId
//                        var fql = true
//                        if (objectId == "") {
//                            objectId = post.objectIdentifier
//                            fql = false
//                        }

//                        var page = pageStack.push(Qt.resolvedUrl("TypeSolverPage.qml"),
//                                                  {"identifier": objectId,
//                                                   "type": post.facebookObjectType,
//                                                   "fql": fql})
//                        page.load()
//                    }
//                }
            }
        }

        // Likes comments
        Label {
            visible: helper.hasFooter
            anchors.left: parent.left; anchors.right: parent.right
            font.pixelSize: Theme.fontSizeExtraSmall
            color: Theme.secondaryHighlightColor
            text: helper.footer
        }
    }
}

