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

Item {
    id: container
    anchors.fill: parent
    property bool busy: false
    property var item: null
    property var model: null

    BusyIndicator {
        visible: flickable.busy
        running: visible
        size: BusyIndicatorSize.Large
        anchors.centerIn: parent
    }

    Connections {
        target: item
        onErrorMessageChanged: console.debug("==== Error ====\n" + item.errorMessage + "\n===============")
    }

    Connections {
        target: model
        onErrorMessageChanged: console.debug("==== Error ====\n" + model.errorMessage + "\n===============")
    }

    SilicaFlickable {
        id: flickable

        function testError() {
            switch (flickable.errorType) {
            case SocialNetwork.NetworkError:
                flickable._errorRecoveryMode = "reload_internet"
                break
            case SocialNetwork.SocialNetworkError:
                var errorMessage = flickable._modelError ? model.errorMessage : item.errorMessage
                var code = errorMessage.match(/Code: (\d+)/)[1];
                if (code == 190 || code == 102) {
                    flickable._errorRecoveryMode = "reconnect"
                } else {
                    flickable._errorRecoveryMode = "reload_facebook"
                }
                break
            case SocialNetwork.DataError:
                flickable._errorRecoveryMode = "bad"
                break
            case SocialNetwork.InternalError:
                flickable._errorRecoveryMode = "bad"
                break
            default:
                flickable._errorRecoveryMode = ""
                break
            }
        }

        property bool _itemBusy: item != null ? item.status == SocialNetwork.Busy : false
        property bool _modelBusy: model != null ? (model.status == SocialNetwork.Busy
                                                   && model.count == 0) : false
        property bool _itemError: item != null ? item.status == SocialNetwork.Error : false
        property bool _modelError: model != null ? (model.status == SocialNetwork.Error
                                                   && model.count == 0) : false
        property string _errorRecoveryMode: ""

        property bool busy: container.busy || _itemBusy || _modelBusy
        property bool error: !container.busy && (_itemError || _modelError)

        // Currently, the model have higher priority on error
        property int errorType: _modelError ? model.error : (_itemError ? item.error : SocialNetwork.NoError)
        onErrorTypeChanged: testError()

        anchors.fill: parent
        contentWidth: width
        contentHeight: height

        ViewPlaceholder {
            function getText() {
                switch (flickable._errorRecoveryMode) {
                case "reload_internet":
                    //: An error message that informs that there were a problem with the network. It should ask the user to try again.
                    //% "Something wrong happened. Check your Internet connection and try again."
                    return qsTrId("friends_error_network")
                case "reload_facebook":
                    //: An error message that informs that there were a problem with Facebook. It might be an operation that Facebook prohibits. It might ask the user to try again.
                    //% "Facebook has returned an error. The operation might not be permitted."
                    return qsTrId("friends_error_facebook")
                case "reconnect":
                    //: An error message that informs that the session has expired, and ask the user to reconnect to Facebook.
                    //% "Session expired. Please reconnect to Facebook."
                    return qsTrId("friends_error_session")
                case "bad":
                    //: An error message that informs that something bad happened, and is not handled. It might ask the user to get in touch with the developer.
                    //% "Something really bad happened. Please get in touch with the developer."
                    return qsTrId("friends_error_unknown")
                default:
                    return ""
                }
            }

            enabled: flickable.error
            text: getText()
        }
    }

    Button {
        function getText() {
            switch (flickable._errorRecoveryMode) {
            case "reload_internet":
                //: A button that allows to retry to load data from Facebook, because of an Internet failure, or because Facebook sent an error.
                //% "Retry"
                return qsTrId("friends_button_retry")
            case "reload_facebook":
                return qsTrId("friends_button_retry")
            case "reconnect":
                //: A button that allows to reconnect (perform login again) to Facebook, because the current session expired.
                //% "Reconnect"
                return qsTrId("friends_button_reconnect")
            default:
                return ""
            }
        }

        enabled: visible
        visible: flickable.error && text != ""
        anchors.bottom: parent.bottom; anchors.horizontalCenter: parent.horizontalCenter
        text: getText()
        onClicked: {
            if (flickable._errorRecoveryMode == "reload_internet"
                || flickable._errorRecoveryMode == "reload_facebook") {
                if (flickable._itemError) {
                    if (item.status == SocialNetwork.Idle || item.status == SocialNetwork.Error) {
                        item.load()
                    }
                }
                if (flickable._modelError) {
                    if (model.status == SocialNetwork.Idle || model.status == SocialNetwork.Error) {
                        model.load()
                    }
                }
            } else if (flickable._errorRecoveryMode == "reconnect") {
                app.performLogin()
            }
        }
    }
}


