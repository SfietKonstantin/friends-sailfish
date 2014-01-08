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
import harbour.friends.social.extra 1.0

Page {
    id: container
    function load() {
        if (!view.ready) {
            view.queued = true
            return
        }
        if (model.status == SocialNetwork.Idle || model.status == SocialNetwork.Error) {
            model.load()
        }
    }

    Connections {
        target: me
        onStatusChanged: {
            if (me.status == SocialNetwork.Error) {
                pageStack.navigateForward()
            }
        }
    }

    onStatusChanged: {
        if (status == PageStatus.Active) {
            pageStack.pushAttached(menuPage)
            if (!settingsManager.welcomeDone) {
                settingsManager.welcomeDone = true
                pageStack.push(Qt.resolvedUrl("WelcomeDialog.qml"))
            }
        }
    }

    StateIndicator {
        busy: !view.ready
        model: model
    }

    SilicaListView {
        id: view
        anchors.fill: parent
        property bool ready: facebook.currentUserIdentifier != "me"
        property bool queued: false
        visible: (model.status == SocialNetwork.Idle && view.ready) || model.count > 0
        onReadyChanged: {
            if (ready && queued) {
                queued = false
                load()
            }
        }

        model: SocialNetworkModel {
            id: model
            socialNetwork: facebook
            filter: NewsFeedFilter {
                type: NewsFeedFilter.Home
            }
        }
        header: PageHeader {
            //: Title of the page showing the news feed (or the Home feed)
            //% "News"
            title: qsTrId("friends_news_title")
        }

        delegate: PostDelegate {
            post: model.contentItem
            from: model.contentItem.from
            to: model.contentItem.to.length > 0 ? model.contentItem.to[0] : null
            fancy: false
            onClicked: {
                var headerProperties = {"post": post}
                var page = pageStack.push(Qt.resolvedUrl("CommentsPage.qml"),
                                          {"identifier": post.identifier,
                                           "item": post,
                                           "headerComponent": postHeaderComponent,
                                           "headerProperties": headerProperties})
                page.load()
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
            //: Text shown on the placeholder, where there is no news to be displayed
            //% "No news"
            text: qsTrId("friends_news_placeholder")
        }

        PullDownMenu {
            MenuItem {
                id: postMenu
                property bool loading: false
                function posted() {
                    loading = true
                }

                //: Action that allows the user to post something
                //% "Post something"
                text: qsTrId("friends_news_action_post")
                onClicked: {
                    var dialog = pageStack.push(Qt.resolvedUrl("PostDialog.qml"))
                    dialog.accepted.connect(posted)
                }

                Connections {
                    target: me
                    onActionStatusChanged: {
                        if (me.actionStatus == SocialNetwork.Idle && postMenu.loading) {
                            postMenu.loading = false
                            model.loadPrevious()
                        }
                    }
                }
            }

            MenuItem {
                //: Action that allows to refresh a feed
                //% "Refresh"
                text: qsTrId("friends_action_refresh")
                onClicked: model.loadPrevious()
            }
        }
    }
}
