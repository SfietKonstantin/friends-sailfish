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
        id: stateIndicator
        model: model
        item: event
    }

    FacebookExtraEvent {
        id: event
        property bool isOrganizedByYou: facebook.currentUserIdentifier == owner.objectIdentifier
        socialNetwork: facebook
        filter: EventFilter {
            identifier: container.identifier
        }
        onLoaded: stateIndicator.visible = true

        onActionComplete: {
            if (ok) {
                stateIndicator.visible = false
                event.load()
            }
        }
    }

    SilicaListView {
        id: view
        anchors.fill: parent
        visible: (model.status == SocialNetwork.Idle) || model.count > 0
        header: Item {
            anchors.left: parent.left; anchors.right: parent.right
            height: childrenRect.height + 0.5 * Theme.paddingMedium

            Column {
                anchors.left: parent.left; anchors.right: parent.right
                spacing: Theme.paddingMedium
                CoverHeader {
                    id: coverImage
                    anchors.left: parent.left; anchors.right: parent.right
                    height: 2 * Theme.itemSizeExtraLarge
                    coverUrl: event.cover.source
                    name: event.name
                }

                Rectangle {
                    anchors.left: parent.left; anchors.leftMargin: Theme.paddingMedium
                    anchors.right: parent.right; anchors.rightMargin: Theme.paddingMedium
                    height: childrenRect.height + 2 * Theme.paddingMedium
                    color: Theme.rgba(Theme.highlightBackgroundColor, 0.2)
                    visible: event.description != ""

                    Label {
                        anchors.top: parent.top; anchors.topMargin: Theme.paddingMedium
                        anchors.left: parent.left; anchors.leftMargin: Theme.paddingMedium
                        anchors.right: parent.right; anchors.rightMargin: Theme.paddingMedium
                        text: event.description
                        font.pixelSize: Theme.fontSizeSmall
                        wrapMode: Text.WordWrap
                    }
                }

                Rectangle {
                    anchors.left: parent.left; anchors.leftMargin: Theme.paddingMedium
                    anchors.right: parent.right; anchors.rightMargin: Theme.paddingMedium
                    height: childrenRect.height + 2 * Theme.paddingMedium
                    color: Theme.rgba(Theme.highlightBackgroundColor, 0.2)

                    Column {
                        anchors.top: parent.top; anchors.topMargin: Theme.paddingMedium
                        anchors.left: parent.left; anchors.right: parent.right
                        spacing: Theme.paddingMedium
                        Label {
                            anchors.left: parent.left; anchors.leftMargin: Theme.paddingMedium
                            anchors.right: parent.right; anchors.rightMargin: Theme.paddingMedium
                            //: Text indicating that the user is organizing the event
                            //% "You are organizing this event"
                            text: event.isOrganizedByYou ? qsTrId("friends_event_organizer_me")
                            //: Text indicating the organizer. %1 is replaced by the name of the organizer.
                            //% "Organized by %1"
                                                         : qsTrId("friends_event_organizer").arg(event.owner.objectName)
                            font.pixelSize: Theme.fontSizeSmall
                        }
                        Label {
                            anchors.left: parent.left; anchors.leftMargin: Theme.paddingMedium
                            anchors.right: parent.right; anchors.rightMargin: Theme.paddingMedium
                            text: DateHelper.formatDateTime(event.startTime, Qt.DefaultLocaleShortDate)
                            font.pixelSize: Theme.fontSizeSmall
                        }
                        Label {
                            visible: event.endTime != ""
                            anchors.left: parent.left; anchors.leftMargin: Theme.paddingMedium
                            anchors.right: parent.right; anchors.rightMargin: Theme.paddingMedium
                            text: DateHelper.formatDateTime(event.endTime, Qt.DefaultLocaleShortDate)
                            color: Theme.secondaryColor
                            font.pixelSize: Theme.fontSizeSmall
                        }
                    }
                }

                ListItem {
                    id: rsvpButton
                    menu: rsvpMenu
                    showMenuOnPressAndHold: false
                    onClicked: !menuOpen ? showMenu() : hideMenu()
                    visible: !event.isOrganizedByYou
                    enabled: event.actionStatus != Facebook.Busy
                    function update() {
                        switch (event.rsvpStatus) {
                        case FacebookExtraEvent.NotReplied:
                            //: Displayed on the RSV button, asking for a reply from the user
                            //% "Will you attend this event ?"
                            rsvpButtonText.text = qsTrId("friends_event_rsvp_rsvp")
                            break
                        case FacebookExtraEvent.Attending:
                            //: Displayed on the RSV button, informing that the user is attending
                            //% "You are attending"
                            rsvpButtonText.text = qsTrId("friends_event_rsvp_attending")
                            break
                        case FacebookExtraEvent.Unsure:
                            //: Displayed on the RSV button, informing that the user is unsure
                            //% "You are unsure"
                            rsvpButtonText.text = qsTrId("friends_event_rsvp_unsure")
                            break
                        case FacebookExtraEvent.Declined:
                            //: Displayed on the RSV button, informing that the user declined
                            //% "You declined"
                            rsvpButtonText.text = qsTrId("friends_event_rsvp_declined")
                            break
                        default:
                            break
                        }
                    }

                    BusyIndicator {
                        id: rsvpButtonSpinner
                        visible: event.actionStatus == Facebook.Busy
                        running: visible
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left; anchors.leftMargin: Theme.paddingMedium
                    }

                    Label {
                        id: rsvpButtonText
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: rsvpButtonSpinner.right; anchors.leftMargin: Theme.paddingMedium
                        anchors.right: parent.right; anchors.rightMargin: Theme.paddingMedium
                    }

                    Component {
                        id: rsvpMenu
                        ContextMenu {
                            MenuItem {
                                //: A menu action to attend to an event
                                //% "Attend"
                                text: qsTrId("friend_event_action_attend")
                                onClicked: event.attend()
                            }
                            MenuItem {
                                //: A menu action to indicate that you are unsure to attend to an event
                                //% "Unsure"
                                text: qsTrId("friend_event_action_unsure")
                                onClicked: event.maybe()
                            }
                            MenuItem {
                                //: A menu action to decline an event
                                //% "Decline"
                                text: qsTrId("friend_event_action_decline")
                                onClicked: event.decline()
                            }
                        }
                    }

                    Connections {
                        target: event
                        onRsvpStatusChanged: rsvpButton.update()
                        onStatusChanged: rsvpButton.update()
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

        PullDownMenu {
            z: 1000
            busy: model.status == SocialNetwork.Busy

            MenuItem {
                //: Action that shows the invited people
                //% "Invited"
                text: qsTrId("friends_event_action_invited")
                onClicked: {
                    var page = pageStack.push(Qt.resolvedUrl("UsersPage.qml"),
                                              {"identifier": container.identifier,
                                               //: Title of the page showing the list of members in a group
                                               //% "Members"
                                               "title": qsTrId("friends_event_invited_title"),
                                               "connection": Facebook.Invited,
                                               "fields": "id,name,rsvp_status",
                                               "sectionField": "rsvp_status"})
                    page.load()
                }
            }

            MenuItem {
                text: qsTrId("friends_action_refresh")
                onClicked: model.loadPrevious()
            }
        }
    }
}
