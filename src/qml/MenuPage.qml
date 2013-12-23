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
    SilicaListView {
        anchors.fill: parent
        model: ListModel {
            ListElement {
                text: QT_TR_NOOP("Home")
                page: "NewsPage.qml"
            }
            ListElement {
                text: QT_TR_NOOP("Me")
                page: ""
            }
            ListElement {
                text: QT_TR_NOOP("Friends")
                page: "FriendsPage.qml"
            }
            ListElement {
                text: QT_TR_NOOP("Photos")
                page: ""
            }
            ListElement {
                text: QT_TR_NOOP("Events")
                page: ""
            }
            ListElement {
                text: QT_TR_NOOP("Messages")
                page: ""
            }
        }
        header: CoverHeader {
            anchors.left: parent.left; anchors.right: parent.right
            name: me.name
            coverUrl: me.cover != null ? me.cover.source : ""
        }

        delegate: BackgroundItem {
            Label {
                anchors.left: parent.left; anchors.leftMargin: Theme.paddingMedium
                anchors.right: parent.right; anchors.rightMargin: Theme.paddingMedium
                anchors.verticalCenter: parent.verticalCenter
                text: qsTr(model.text)
            }
            onClicked: {
                if (model.page == "") {
                    return
                }

                pageStack.clear()
                pageStack.push(Qt.resolvedUrl(model.page))
                pageStack.navigateForward(PageStackAction.Immediate)
                pageStack.navigateBack()
            }
        }
    }
}
