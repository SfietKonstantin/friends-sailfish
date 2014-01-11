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
import "UiConstants.js" as Ui

Page {
    id: container
    property string identifier
    function load() {
        if (group.status == SocialNetwork.Idle || group.status == SocialNetwork.Error) {
            group.load()
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
        model: model
        item: group
    }

    FacebookGroup {
        id: group
        socialNetwork: facebook
        filter: FacebookItemFilter {
            identifier: container.identifier
            fields: "name,cover"
        }
    }

    SilicaListView {
        id: view
        anchors.fill: parent
        visible: (model.status == SocialNetwork.Idle) || model.count > 0
        header: Item {
            width: view.width
            height: childrenRect.height
            Item {
                anchors.left: parent.left; anchors.right: parent.right
                height: 2 * Theme.itemSizeExtraLarge + Theme.itemSizeSmall + 0.5 * Theme.paddingSmall

                Rectangle {
                    id: coverBackground
                    anchors.left: parent.left; anchors.right: parent.right
                    height: 2 * Theme.itemSizeExtraLarge
                    color: Theme.secondaryHighlightColor
                    clip: true

                    FacebookImage {
                        id: image
                        anchors.fill: parent
                        fillMode: Image.PreserveAspectCrop
                         url: group.cover.source
                    }

                    ShaderEffect {
                        id: gradient
                        property variant source: ShaderEffectSource {
                            hideSource: true
                            sourceItem: image
                        }

                        property real _boundary: 1 - (Theme.paddingLarge + Theme.fontSizeLarge + Theme.paddingMedium) / height;
                        anchors.fill: image

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
                        id: nameText
                        anchors.left: parent.left; anchors.leftMargin: Theme.paddingMedium
                        anchors.right: parent.right
                        anchors.rightMargin: Theme.paddingMedium + Theme.itemSizeSmall * 2
                        anchors.bottom: parent.bottom; anchors.bottomMargin: Theme.paddingMedium
                        opacity: 0
                        wrapMode: Text.WordWrap
                        font.pixelSize: Theme.fontSizeLarge
                        states: [
                            State {
                                name: "visible"; when: group.name != ""
                                PropertyChanges {
                                    target: nameText
                                    opacity: 1
                                    text: group.name
                                }
                                PropertyChanges {
                                    target: gradient
                                    opacity: 0.8
                                }
                            }
                        ]
                        Behavior on opacity {
                            NumberAnimation {duration: Ui.ANIMATION_DURATION_NORMAL}
                        }
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
            from: model.contentItem.from
            to: model.contentItem.to.length > 0 ? model.contentItem.to[0] : null
            fancy: false
        }

        onAtYEndChanged: {
            if (atYEnd && model.hasNext) {
                model.loadNext()
            }
        }

        VerticalScrollDecorator {}

        ViewPlaceholder {
            enabled: model.status == SocialNetwork.Idle && model.count == 0
            //: Text shown on the placeholder, where there is no posts from a group to be displayed
            //% "No posts"
            text: qsTrId("friends_group_placeholder")
        }

        PullDownMenu {
            z: 1000
            busy: model.status == SocialNetwork.Busy

            MenuItem {
                //: Action that shows the current user's personnal informations
                //% "Group information"
                text: qsTrId("friends_group_action_about")
                enabled: false
            }

            MenuItem {
                text: qsTrId("friends_action_refresh")
                onClicked: model.loadPrevious()
            }
        }
    }
}
