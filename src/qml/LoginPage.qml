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
    signal connected()
    property bool busy: loginItem.status == SocialNetworkStatus.Busy
    property bool error: loginItem.status == SocialNetworkStatus.Error
    property bool loggedIn: tokenManager.token !== ""

    SocialContentItem {
        id: loginItem
        socialNetwork: facebook
        request: FacebookLoginRequest {
            email: email.text
            password: password.text
            deviceId: tokenManager.deviceId
            machineId: tokenManager.machineId
        }
        builder: FacebookLoginContentBuilder {}
        onFinished: {
            if (ok) {
                if (loginItem.object.machine_id !== null) {
                    tokenManager.machineId = loginItem.object.machine_id
                }
                tokenManager.userId = loginItem.object.uid
                tokenManager.token = loginItem.object.access_token
                container.connected()
            }
        }
    }

    PageHeader {
        id: header
        //: Title of the login page
        //% "Login"
        title: qsTrId("friends_login_title")
    }

    FLabel {
        id: label
        anchors.top: header.bottom; anchors.topMargin: Theme.paddingLarge
        anchors.left: parent.left; anchors.leftMargin: Theme.paddingLarge
        anchors.right: parent.right; anchors.rightMargin: Theme.paddingLarge
        //: Text indicating that this page asks credentials to connect to Facebook using Friends client
        //% "Connect to Facebook using Friends client"
        text: qsTrId("friends_login_indication")
    }

    TextField {
        id: email
        anchors.top: label.bottom; anchors.topMargin: Theme.paddingLarge
        anchors.left: parent.left; anchors.right: parent.right
        enabled: !container.busy && !loggedIn
        //: The email field used to connect to Facebook
        //% "Email"
        label: qsTrId("friends_login_email")
        placeholderText: qsTrId("friends_login_email")
    }
    TextField {
        id: password
        anchors.top: email.bottom
        anchors.left: parent.left; anchors.right: parent.right
        enabled: !container.busy && !loggedIn
        //: The password field used to connect to Facebook
        //% "Password"
        label: qsTrId("friends_login_password")
        placeholderText: qsTrId("friends_login_password")
        echoMode: TextInput.Password
    }

    Button {
        id: connectButton
        anchors.top: password.bottom; anchors.topMargin: Theme.paddingMedium
        anchors.horizontalCenter: parent.horizontalCenter
        enabled: !container.busy && email.text !== "" && password.text !== "" && !loggedIn
        //: The button used to connect to Facebook
        //% "Connect"
        text: qsTrId("friends_login_button_connect")
        onClicked: loginItem.load()
    }

    BusyIndicator {
        visible: container.busy
        running: visible
        size: BusyIndicatorSize.Small
        anchors.right: connectButton.left; anchors.rightMargin: Theme.paddingMedium
        anchors.verticalCenter: connectButton.verticalCenter
    }

    FLabel {
        id: errorLabel
        anchors.top: connectButton.bottom; anchors.topMargin: Theme.paddingLarge
        anchors.left: parent.left; anchors.leftMargin: Theme.paddingLarge
        anchors.right: parent.right; anchors.rightMargin: Theme.paddingLarge
        color: "red"
        visible: container.error
        text: {
            switch (loginItem.error) {
            case SocialNetworkError.Network:
                //: Label informing about a network error
                //% "Please check your network connection and try again"
                return qsTrId("friends_login_label_error_network")
            case SocialNetworkError.SocialNetwork:
                return loginItem.errorString
            case SocialNetworkError.No:
                return ""
            default:
                //: Label informing about an internal / unknown error
                //% "Friends has encountered an internal error. You might need to update Friends"
                return qsTrId("friends_login_label_error_internal")
            }
        }
    }

    FListButton {
        anchors.bottom: parent.bottom
        //: The button used to display more information about the client
        //% "More information"
        text: qsTrId("friends_login_button_info")
    }
}
