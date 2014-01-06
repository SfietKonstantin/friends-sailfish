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

ApplicationWindow {
    id: app
    cover: Qt.resolvedUrl("CoverPage.qml")

    function performLogin() {
        me.loaded = false
        pageStack.push(loginDialogComponent)
    }

    MenuPage {
        id: menuPage
    }

    TokenManager {
        id: tokenManager
    }

    SettingsManager {
        id: settingsManager
    }

    Facebook {
        id: facebook
        accessToken: tokenManager.token
        onAccessTokenChanged: {
            me.loadMe()
        }
    }

    FacebookUser {
        id: me
        property bool loaded: false
        function loadMe() {
            if (!facebook.accessToken.length > 0) {
                return
            }

            if ((status == SocialNetwork.Idle || status == SocialNetwork.Error) && !loaded) {
                load()
                loaded = true
            }
        }
        onErrorMessageChanged: console.debug("==== Error ====\n" + errorMessage + "\n===============")
        socialNetwork: facebook
        filter: FacebookItemFilter {
            identifier: "me"
            fields: "name,first_name,cover"
        }

        onStatusChanged: {
            loadMe()
        }
    }

    Component {
        id: loginDialogComponent
        LoginDialog {
            onConnected: {
                pageStack.clear()
                var page = pageStack.push(Qt.resolvedUrl("NewsPage.qml"))
                page.load()
            }
        }
    }

    Component.onCompleted: {
        if (tokenManager.token.length > 0) {
            var page = pageStack.push(Qt.resolvedUrl("NewsPage.qml"))
            page.load()

        } else {
            app.performLogin()
        }
    }
}


