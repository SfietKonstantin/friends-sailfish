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
        PostHeader { helper: helper }

        // Content
        PostContent { helper: helper }

        // Attachment
        PostAttachment { helper: helper }

        // Attached story
        Item {
            id: attachedStory
            property Item item
            anchors.left: parent.left; anchors.right: parent.right
            height: childrenRect.height
            visible: item !== null

            Connections {
                target: helper
                onHasAttachedStoryChanged: {
                    if (attachedStory.item !== null) {
                        attachedStory.item.destroy()
                    }

                    if (helper.hasAttachedStory) {
                        attachedStory.item = attachedStoryComponent.createObject(attachedStory)
                    }
                }
            }

            Component {
                id: attachedStoryComponent

                PostAttachedStory {
                    object: helper.object
                    fancy: helper.fancy
                }
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

