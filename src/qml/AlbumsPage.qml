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

Page {
    id: container
    property string identifier
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
        model: model
    }

    SilicaListView {
        anchors.fill: parent
        visible: model.status == SocialNetwork.Idle || model.count > 0
        model: SocialNetworkModel {
            id: model
            socialNetwork: facebook
            filter: FacebookRelatedDataFilter {
                identifier: container.identifier
                connection: Facebook.Albums
                fields: "id,name,cover_photo"
                limit: 10
            }
        }

        header: PageHeader {
            //: Title of the page showing the list of albums
            //% "Albums"
            title: qsTrId("friends_albums_title")
        }

        delegate: Item {
            width: parent.width
            height: 2 * Theme.itemSizeExtraLarge + Theme.paddingMedium

            BackgroundItem {
                id: background
                anchors.left: parent.left; anchors.leftMargin: Theme.paddingMedium
                anchors.right: parent.right; anchors.rightMargin: Theme.paddingMedium
                anchors.top: parent.top; anchors.topMargin: Theme.paddingMedium / 2
                anchors.bottom: parent.bottom; anchors.bottomMargin: Theme.paddingMedium / 2
                onClicked: {
                    var page = pageStack.push(Qt.resolvedUrl("PhotosPage.qml"),
                                              {"identifier": model.contentItem.identifier})
                    page.load()
                }

                Item {
                    id: image
                    opacity: background.down ? Theme.highlightBackgroundOpacity : 1
                    anchors.left: parent.left; anchors.right: parent.right
                    height: childrenRect.height
                    Rectangle {
                        visible: !background.down
                        anchors.fill: parent
                        color: Theme.rgba(Theme.highlightBackgroundColor, 0.2)
                    }
                    FacebookPicture {
                        identifier: model.contentItem.coverPhoto
                        anchors.horizontalCenter: parent.horizontalCenter
                        pictureWidth: Screen.width - 2 * Theme.paddingMedium
                        pictureHeight: 2 * Theme.itemSizeExtraLarge
                    }
                }

            }

            ShaderEffect {
                id: gradient
                property variant source: ShaderEffectSource {
                    hideSource: true
                    sourceItem: background
                }

                property real _boundary: 1 - (Theme.paddingLarge + Theme.fontSizeMedium + Theme.paddingMedium) / height;
                anchors.fill: background

                fragmentShader: "
                varying highp vec2 qt_TexCoord0;
                uniform float qt_Opacity;
                uniform float _boundary;
                uniform sampler2D source;
                void main(void)
                {
                    lowp vec4 textureColor = texture2D(source, qt_TexCoord0.st);
                    gl_FragColor = (1. - smoothstep(_boundary, 1., qt_TexCoord0.y)) * textureColor * qt_Opacity;
                }
                "
            }

            Label {
                anchors.left: gradient.left; anchors.leftMargin: Theme.paddingMedium
                anchors.right: gradient.right; anchors.rightMargin: Theme.paddingMedium
                anchors.bottom: gradient.bottom; anchors.bottomMargin: Theme.paddingMedium
                text: model.contentItem.name
                truncationMode: TruncationMode.Fade
            }
        }

        onAtYEndChanged: {
            if (atYEnd && model.hasNext) {
                if (model.status == SocialNetwork.Idle || model.status == SocialNetwork.Error) {
                    model.loadNext()
                }
            }
        }

        VerticalScrollDecorator {}

        ViewPlaceholder {
            enabled: model.status == SocialNetwork.Idle && model.count == 0
            //: Text shown on the placeholder, where there is no albums to be displayed
            //% "No albums"
            text: qsTrId("friends_album_placeholder")
        }
    }
}
