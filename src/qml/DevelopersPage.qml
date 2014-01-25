
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

Page {
    id: container
    ListModel {
        id: model
        ListElement {
            //icon: "chriadam.jpg"
            icon: ""
            category: "Developers of Nemo Social"
            name: "Chris Adams"
            nickname: "chriadam"
            description: "Lead developer"
            twitter: ""
            website: "https://github.com/chriadam"
        }
        ListElement {
            icon: "../data/sfiet_konstantin.jpg"
            category: "Developers of Nemo Social"
            name: "Lucien Xu"
            nickname: "Sfiet_Konstantin"
            description: "Contributor"
            twitter: "https://twitter.com/SfietKonstantin"
            website: "https://github.com/SfietKonstantin"
        }
        ListElement {
            icon: "../data/sfiet_konstantin.jpg"
            category: "Developers of Friends"
            name: "Lucien Xu"
            nickname: "Sfiet_Konstantin"
            description: "Main developer"
            twitter: "https://twitter.com/SfietKonstantin"
            website: "https://github.com/SfietKonstantin"
        }
        ListElement {
            icon: "../data/morpog.jpeg"
            category: "Thanks to"
            name: "Stephan Beyerle"
            nickname: "Morpog"
            description: "Icons master"
            twitter: "https://twitter.com/Morpog"
            website: ""
        }
    }


    SilicaListView {
        id: view
        anchors.fill: parent
        model: model
        section.property: "category"
        section.delegate: SectionHeader {
            text: section
        }

        header: PageHeader {
            //: Title of the page showing the list of developers
            //% "Developers"
            title: qsTrId("friends_developers_title")
        }

        delegate: ListItem {
            id: background
            width: view.width
            contentHeight: column.height + 2 * Theme.paddingMedium
            menu: contextMenu
            onClicked: showMenu()

            Image {
                id: icon
                width: Theme.iconSizeMedium; height: Theme.iconSizeMedium
                anchors.left: parent.left; anchors.leftMargin: Theme.paddingMedium
                anchors.verticalCenter: parent.verticalCenter
                source: model.icon != "" ? model.icon
                                         : "image://theme/icon-cover-people"
            }

            Column {
                id: column
                anchors.top: parent.top; anchors.topMargin: Theme.paddingMedium
                anchors.left: icon.right; anchors.leftMargin: Theme.paddingMedium
                spacing: Theme.paddingSmall
                Label {
                    text: model.name + (model.nickname != "" ? " (" + model.nickname + ")" : "")
                    color: background.highlighted ? Theme.highlightColor : Theme.primaryColor
                }

                Label {
                    font.pixelSize: Theme.fontSizeSmall
                    text: model.description
                    color: background.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                }
            }

            Component {
                id: contextMenu
                ContextMenu {
                    MenuItem {
                        visible: model.website != ""
                        //: A menu item that opens a developer's webpage. %1 will be replaced by the developer's name
                        //% "%1's webpage"
                        text: qsTrId("friends_developers_webpage").arg(model.name)
                        onClicked: Qt.openUrlExternally(model.website)
                    }
                    MenuItem {
                        visible: model.twitter != ""
                        //: A menu item that opens a developer's Twitter page. %1 will be replaced by the developer's name
                        //% "%1's Twitter account"
                        text: qsTrId("friends_developers_twitter").arg(model.name)
                        onClicked: Qt.openUrlExternally(model.twitter)
                    }
                }
            }
        }
    }
}
