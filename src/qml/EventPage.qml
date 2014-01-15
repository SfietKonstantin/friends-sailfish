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
import "UiConstants.js" as Ui

Page {
    id: container
    property string identifier
    function load() {
        if (event.status == SocialNetwork.Idle || event.status == SocialNetwork.Error) {
            event.load()
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
        item: event
    }

    FacebookExtraEvent {
        id: event
        socialNetwork: facebook
        filter: EventFilter {
            identifier: container.identifier
        }
    }

    SilicaListView {
        id: view
        anchors.fill: parent
        visible: (model.status == SocialNetwork.Idle) || model.count > 0
        header: Item {
            width: view.width
            height: childrenRect.height
            Item {
                anchors.left: parent.left; anchors.right: parent.right
                height: 2 * Theme.itemSizeExtraLarge + Theme.itemSizeSmall + 0.5 * Theme.paddingSmall

                CoverHeader {
                    id: coverImage
                    anchors.left: parent.left; anchors.right: parent.right
                    height: 2 * Theme.itemSizeExtraLarge
                    coverUrl: event.cover.source
                    name: event.name
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
                model.loadNext()
            }
        }

        VerticalScrollDecorator {}

        PullDownMenu {
            z: 1000
            busy: model.status == SocialNetwork.Busy

            MenuItem {
                text: qsTrId("friends_action_refresh")
                onClicked: model.loadPrevious()
            }
        }
    }
}
