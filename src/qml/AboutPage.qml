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
    SilicaFlickable {
        anchors.fill: parent
        contentHeight: mainColumn.height + Theme.paddingLarge + buttonsColumn.height
        Column {
            id: mainColumn
            spacing: Theme.paddingMedium
            anchors.left: parent.left; anchors.leftMargin: Theme.paddingMedium
            anchors.right: parent.right; anchors.rightMargin: Theme.paddingMedium
            PageHeader {
                //: Title of the "About application" page
                //% "About"
                title: qsTrId("friends_about_title")
            }

            Image {
                anchors.horizontalCenter: parent.horizontalCenter
                source: "../data/logo.png"
            }

            Column {
                anchors.left: parent.left; anchors.right: parent.right
                spacing: Theme.paddingSmall
                Label {
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.pixelSize: Theme.fontSizeHuge
                    //: Name of the application. Should be "Friends"
                    //% "Friends"
                    text: qsTrId("friends_app_name")
                }

                Label {
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.pixelSize: Theme.fontSizeExtraSmall
                    color: Theme.secondaryColor
                    wrapMode: Text.WordWrap
                    //: Version of Friends. %1 will be replaced by the version. Please add "powered by Nemo Social"
                    //% "Version %1, powered by Nemo Social"
                    text: qsTrId("friends_version").arg(VERSION)
                }
            }

            Label {
                wrapMode: Text.WordWrap
                anchors.left: parent.left; anchors.right: parent.right
                font.pixelSize: Theme.fontSizeSmall
                //: Description of Friends
                //% "Friends is a simple and elegant Facebook client for Sailfish OS. It is an Open Source software, meaning that it can be modified and enhanced by anybody. If you like Friends, please consider a donation. It will help improving the software, while keeping it free and Open Source."
                text: qsTrId("friends_about_text")
            }
        }

        Column {
            id: buttonsColumn
            anchors.top: mainColumn.bottom; anchors.topMargin: Theme.paddingLarge
            anchors.left: parent.left; anchors.right: parent.right
            BackgroundItem {
                anchors.left: parent.left; anchors.right: parent.right
                onClicked: Qt.openUrlExternally(FACEBOOK_PAGE)
                Label {
                    anchors.left: parent.left; anchors.leftMargin: Theme.paddingMedium
                    anchors.right: parent.right; anchors.rightMargin: Theme.paddingMedium
                    anchors.verticalCenter: parent.verticalCenter
                    //: Caption of a button that shows the Facebook page of the application
                    //% "Facebook page"
                    text: qsTrId("friends_about_button_facebook_page")
                }
            }
            BackgroundItem {
                anchors.left: parent.left; anchors.right: parent.right
                onClicked: Qt.openUrlExternally(PAYPAL_DONATE)
                Label {
                    anchors.left: parent.left; anchors.leftMargin: Theme.paddingMedium
                    anchors.right: parent.right; anchors.rightMargin: Theme.paddingMedium
                    anchors.verticalCenter: parent.verticalCenter
                    //: Caption of a button that shows a Paypal donation page
                    //% "Donate"
                    text: qsTrId("friends_button_donate")
                }
            }
            BackgroundItem {
                anchors.left: parent.left; anchors.right: parent.right
                onClicked: pageStack.push(Qt.resolvedUrl("DevelopersPage.qml"))
                Label {
                    anchors.left: parent.left; anchors.leftMargin: Theme.paddingMedium
                    anchors.right: parent.right; anchors.rightMargin: Theme.paddingMedium
                    anchors.verticalCenter: parent.verticalCenter
                    //: Caption of a button that shows the list of developers
                    //% "Developers"
                    text: qsTrId("friends_about_button_developers")
                }
            }
            BackgroundItem {
                anchors.left: parent.left; anchors.right: parent.right
                onClicked: pageStack.push(Qt.resolvedUrl("ChangeLogPage.qml"))
                Label {
                    anchors.left: parent.left; anchors.leftMargin: Theme.paddingMedium
                    anchors.right: parent.right; anchors.rightMargin: Theme.paddingMedium
                    anchors.verticalCenter: parent.verticalCenter
                    //: Caption of a button that leads to the changelog
                    //% "ChangeLog"
                    text: qsTrId("friends_about_button_changelog")
                }
            }
        }

        VerticalScrollDecorator {}
    }
}
