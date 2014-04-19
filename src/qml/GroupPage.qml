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
        if (group.status == SocialNetwork.Idle || group.status == SocialNetwork.Error) {
            group.load()
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
        item: group
    }

    FacebookGroup {
        id: group
        socialNetwork: facebook
        filter: FacebookItemFilter {
            identifier: container.identifier
            fields: "name,cover"
        }
    }

    SilicaListView {
        id: view
        anchors.fill: parent
        visible: (model.status == SocialNetwork.Idle) || model.count > 0
        header: CoverHeader {
            anchors.left: parent.left; anchors.right: parent.right
            name: group.name
            coverUrl: group.cover.source
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
                //: Action that shows the members in the group
                //% "Members"
                text: qsTrId("friends_group_action_members")
                onClicked: {
                    var page = pageStack.push(Qt.resolvedUrl("UsersPage.qml"),
                                              {"identifier": container.identifier,
                                               //: Title of the page showing the list of members in a group
                                               //% "Members"
                                               "title": qsTrId("friends_group_members_title"),
                                               "connection": Facebook.Members})
                    page.load()
                }
            }

            MenuItem {
                //: Action that shows the information of the group
                //% "Group information"
                text: qsTrId("friends_group_action_about")
                onClicked: {
                    var page = pageStack.push(Qt.resolvedUrl("GroupInfoPage.qml"),
                                              {"identifier": container.identifier})
                    page.load()
                }
            }

            MenuItem {
                enabled: group.status == SocialNetwork.Idle
                //: Action that allows the user to post something in a group
                //% "Post something"
                text: qsTrId("friends_group_action_post")
                onClicked: pageStack.push(Qt.resolvedUrl("PostDialog.qml"), {"object": group})

                Connections {
                    target: group
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
