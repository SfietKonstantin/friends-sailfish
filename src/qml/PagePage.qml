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
import harbour.friends.social.extra 1.0
import "UiConstants.js" as Ui

Page {
    id: container
    property string identifier
    function load() {
        if (page.status == SocialNetwork.Idle || page.status == SocialNetwork.Error) {
            page.load()
        }
        if (model.status == SocialNetwork.Idle || model.status == SocialNetwork.Error) {
            model.load()
        }
    }

    onStatusChanged: {
        if (status == PageStatus.Active) {
            pageStack.pushAttached(menuPage)
        }
    }

    StateIndicator {
        model: model
        item: page
    }

    FacebookPage {
        id: page
        socialNetwork: facebook
        filter: FacebookItemFilter {
            identifier: container.identifier
            fields: "name,cover,about,can_post"
        }
    }

    SilicaListView {
        id: view
        anchors.fill: parent
        visible: (model.status == SocialNetwork.Idle) || model.count > 0
        header: Item {
            width: view.width
            height: childrenRect.height + Theme.paddingMedium
            Item {
                anchors.left: parent.left; anchors.right: parent.right
                height: Math.max(2 * Theme.itemSizeExtraLarge + Theme.itemSizeSmall
                                 + 0.5 * Theme.paddingSmall,
                                 2 * Theme.itemSizeExtraLarge + about.height)

                CoverImage {
                    id: coverImage
                    anchors.left: parent.left; anchors.right: parent.right
                    height: 2 * Theme.itemSizeExtraLarge
                    coverUrl: page.cover.source

                    Label {
                        id: nameText
                        anchors.left: parent.left; anchors.leftMargin: Theme.paddingMedium
                        anchors.right: parent.right
                        anchors.rightMargin: Theme.paddingMedium + Theme.itemSizeSmall * 2
                        anchors.bottom: parent.bottom; anchors.bottomMargin: Theme.paddingMedium
                        opacity: 0
                        wrapMode: Text.WordWrap
                        font.pixelSize: Theme.fontSizeLarge
                        states: [
                            State {
                                name: "visible"; when: page.name != ""
                                PropertyChanges {
                                    target: nameText
                                    opacity: 1
                                    text: page.name
                                }
                            }
                        ]
                        Behavior on opacity {
                            NumberAnimation {duration: Ui.ANIMATION_DURATION_NORMAL}
                        }
                    }
                }

                Label {
                    id: about
                    anchors.left: parent.left; anchors.leftMargin: Theme.paddingMedium
                    anchors.right: pictureContainer.left; anchors.rightMargin: Theme.paddingMedium
                    anchors.top: coverImage.bottom; anchors.topMargin: Theme.paddingMedium
                    text: page.about
                    font.pixelSize: Theme.fontSizeExtraSmall
                    wrapMode: Text.WordWrap
                }

                Rectangle {
                    id: pictureContainer
                    opacity: 0
                    anchors.right: parent.right; anchors.rightMargin: Theme.paddingMedium
                    anchors.verticalCenter: coverImage.bottom
                    color: Theme.primaryColor
                    width: Theme.itemSizeSmall * 2 + Theme.paddingSmall
                    height: Theme.itemSizeSmall * 2 + Theme.paddingSmall

                    states: [
                        State {
                            name: "visible"; when: picture.status == Image.Ready
                            PropertyChanges {
                                target: pictureContainer
                                opacity: 1
                            }
                        }
                    ]
                    Behavior on opacity {
                        NumberAnimation {duration: Ui.ANIMATION_DURATION_NORMAL}
                    }

                    FacebookPicture {
                        id: picture
                        identifier: container.identifier
                        anchors.centerIn: parent
                        pictureWidth: Theme.itemSizeSmall * 2
                        pictureHeight: Theme.itemSizeSmall * 2
                    }
                }
            }
        }

        model: SocialNetworkModel {
            id: model
            socialNetwork: facebook
            filter: NewsFeedFilter {
                type: NewsFeedFilter.Feed
                identifier: container.identifier
            }
        }

        delegate: PostDelegate {
            post: model.contentItem
            to: model.contentItem.to.length > 0 ? model.contentItem.to[0] : null
            fancy: false
        }

        onAtYEndChanged: {
            if (atYEnd && model.hasNext) {
                if (model.status == SocialNetwork.Idle || model.status == SocialNetwork.Error) {
                    model.loadNext()
                }
            }
        }

        VerticalScrollDecorator {}

        ViewPlaceholder {
            enabled: model.status == SocialNetwork.Idle && model.count == 0
            text: qsTrId("friends_no_posts")
        }

        PullDownMenu {
            z: 1000
            busy: model.status == SocialNetwork.Busy

            MenuItem {
                enabled: (page.status == SocialNetwork.Idle) && page.canPost
                //: Action that allows the user to post something
                //% "Post something"
                text: qsTrId("friends_page_action_post")
                onClicked: pageStack.push(Qt.resolvedUrl("PostDialog.qml"), {"object": page})

                Connections {
                    target: page
                    onActionComplete: {
                        if (ok) {
                            model.loadPrevious()
                        }
                    }
                }
            }

            MenuItem {
                text: qsTrId("friends_action_refresh")
                onClicked: model.loadPrevious()
            }
        }
    }
}
