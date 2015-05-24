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
import harbour.friends.microf 1.0

Page {
    id: container
    property string title
    property alias userId: request.userId

    onStatusChanged: {
        switch (status) {
        case PageStatus.Active:
            pageStack.pushAttached(menuPage)
            break
        }
    }

    StateIndicator {
        model: model
    }

    SilicaListView {
        id: view
        anchors.fill: parent
        visible: model.status === SocialNetworkStatus.Ready
        currentIndex: -1
        model: FriendsProxyModel {
            id: model
            model: SocialContentModel {
                socialNetwork: facebook
                request: FacebookFriendListRequest {
                    id: request
                    count: 200
                }
                builder: FacebookModelBuilder {
                    properties: [
                        FacebookProperty { path: "id"; name: "id" },
                        FacebookProperty { path: "name"; name: "name" },
                        FacebookProperty { path: "profile_picture/uri"; name: "profilePicture" }
                    ]
                }
            }
            Component.onCompleted: load()
        }

        header: Column {
            width: view.width
            PageHeader {
                title: container.title
            }
            SearchField {
                anchors.left: parent.left; anchors.right: parent.right
                onTextChanged: model.filter = text
            }
        }

        section.property: "section"
        section.criteria: ViewSection.FirstCharacter
        section.delegate: SectionHeader {
            text: section
        }

        delegate: FListButton {
            height: Theme.itemSizeLarge
            iconSize: Theme.iconSizeLarge
            icon: model.object.profilePicture
            text: model.object.name
        }

        VerticalScrollDecorator {}

        ViewPlaceholder {
            enabled: model.status === SocialNetworkStatus.Idle && model.count == 0
            //: Text shown on the placeholder, where there is no friends to be displayed
            //% "No Friends"
            text: qsTrId("friends_friends_placeholder")
        }
    }
}

