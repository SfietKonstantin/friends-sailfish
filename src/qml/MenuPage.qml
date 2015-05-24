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
import harbour.friends.microf 1.0

Page {
    id: container
    StateIndicator {
        item: me
    }

    function disconnect() {
        //: Message displayed in a remorse timer, when the disconnect from Facebook action has been triggered
        //% "Disconnecting"
        remorse.execute(qsTrId("friends_menu_remorse_disconnect"),
                        function() {
                            pageStack.clear()
                            pageStack.push(Qt.resolvedUrl("DisconnectPage.qml"))
                        })
    }
    RemorsePopup { id: remorse }

    SilicaListView {
        anchors.fill: parent
        visible: me.status === SocialNetworkStatus.Ready
        model: ListModel {
            ListElement {
                //: Menu entry to show the Home page, with the user's news feed
                //% "Home"
                text: QT_TRID_NOOP("friends_menu_home")
                page: "NewsPage.qml"
            }
            ListElement {
                //: Menu entry to show the "me" page, with the user's page
                //% "Me"
                text: QT_TRID_NOOP("friends_menu_me")
                page: ""
            }
            ListElement {
                //: Menu entry to show the friends page, with the user's friends
                //% "Friends"
                text: QT_TRID_NOOP("friends_menu_friends")
                page: "FriendsPage.qml"
            }
            ListElement {
                //: Menu entry to show the albums page, with the user's albums
                //% "Albums"
                text: QT_TRID_NOOP("friends_menu_albums")
                page: ""
            }
            ListElement {
                //: Menu entry to show the photos page, with the photos where the user has been tagged.
                //% "Photos"
                text: QT_TRID_NOOP("friends_menu_photos")
                page: ""
            }
            ListElement {
                //: Menu entry to show the events page, with the events where the user is invited.
                //% "Events"
                text: QT_TRID_NOOP("friends_menu_events")
                page: ""
            }
            ListElement {
                //: Menu entry to show the groups page, with list of groups where the user is.
                //% "Groups"
                text: QT_TRID_NOOP("friends_menu_groups")
                page: ""
            }
            ListElement {
                //: Menu entry to show the pages page, with list of pages the user liked.
                //% "Pages"
                text: QT_TRID_NOOP("friends_menu_pages")
                page: ""
            }
            ListElement {
                //: Menu entry to show the notifications page, where the user can read notifications.
                //% "Notifications"
                text: QT_TRID_NOOP("friends_menu_notifications")
                page: ""
            }
        }
        header: CoverHeader {
            anchors.left: parent.left; anchors.right: parent.right
            name: me.object.name !== null ? me.object.name : ""
            coverUrl: me.object.cover !== null ? me.object.cover : ""
        }

        delegate: BackgroundItem {
            enabled: model.page !== ""
            Label {
                anchors.left: parent.left; anchors.leftMargin: Theme.paddingMedium
                anchors.right: parent.right; anchors.rightMargin: Theme.paddingMedium
                anchors.verticalCenter: parent.verticalCenter
                text: qsTrId(model.text)
                color: model.page !== "" ? Theme.primaryColor : Theme.secondaryColor
            }
            onClicked: {
                if (model.page === "") {
                    return
                }

                // Give the illusion that we changed the root page, but that
                // we are still in the menu
                pageStack.clear()
                var properties = {"userId": tokenManager.userId}
                if (model.page === "FriendsPage.qml") {
                    //: Title of the page showing the list of friends
                    //% "Friends"
                    properties.title = qsTrId("friends_friends_title")
                }

                pageStack.push(Qt.resolvedUrl(model.page), properties)
                pageStack.navigateForward(PageStackAction.Immediate)
                pageStack.navigateBack()
            }
        }

        PullDownMenu {
            z: 1000
            MenuItem {
                //: Action that disconnects the user from Facebook
                //% "Logout"
                text: qsTrId("friends_menu_action_logout")
                onClicked: container.disconnect()
            }

            MenuItem {
                //: Action that shows the "About Friends" page
                //% "About Friends"
                text: qsTrId("friends_menu_action_about_friends")
                onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
            }
        }

        VerticalScrollDecorator {}
    }
}
