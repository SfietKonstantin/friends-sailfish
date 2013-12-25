
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

Page {
    id: container
    property string identifier // Compatibility
    function load() {
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
        busy: model.status == SocialNetwork.Busy
        error: model.status == SocialNetwork.Error
        onReload: container.load()
    }

    SilicaListView {
        anchors.fill: parent
        visible: model.status == SocialNetwork.Idle
        model: SocialNetworkModel {
            id: model
            socialNetwork: facebook
            filter: FacebookRelatedDataFilter {
                identifier: facebook.currentUserIdentifier
                connection: Facebook.Friends
                limit: 500
            }
            sorters: AlphabeticalSorter {field: "name"}
            onStatusChanged: {
                if (status == Facebook.Idle && hasNext) {
                    loadNext()
                }
            }
        }

        header: PageHeader {
            title: qsTr("Friends")
        }

        // BUG: not working: criteria FirstCharacter is not taken in account
//        section {
//            criteria: ViewSection.FirstCharacter
//            property: "contentItem.name"
//            delegate: SectionHeader {
//                text: section
//                height: Theme.itemSizeExtraSmall
//            }
//        }

        delegate: BackgroundItem {
            height: Theme.itemSizeLarge
            Rectangle {
                anchors.fill: icon
                color: Theme.rgba(Theme.highlightBackgroundColor, 0.2)
            }

            FacebookPicture {
                id: icon
                anchors.left: parent.left; anchors.leftMargin: Theme.paddingMedium
                anchors.verticalCenter: parent.verticalCenter
                identifier: model.contentItem.identifier
            }

            Label {
                anchors.left: icon.right; anchors.leftMargin: Theme.paddingMedium
                anchors.right: parent.right; anchors.rightMargin: Theme.paddingMedium
                anchors.verticalCenter: parent.verticalCenter
                text: model.contentItem.name
                truncationMode: TruncationMode.Fade
            }

            onClicked: {
                var page = pageStack.push(Qt.resolvedUrl("UserPage.qml"),
                                          {"identifier": model.contentItem.identifier})
                page.load()
            }
        }

        VerticalScrollDecorator {}

        ViewPlaceholder {
            enabled: model.status == SocialNetwork.Idle && model.count == 0
            text: qsTr("No friends")
        }
    }
}
