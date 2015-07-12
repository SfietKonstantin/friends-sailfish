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
import harbour.friends.microf 1.0
import "post"

Page {
    id: container
    onStatusChanged: {
        if (status === PageStatus.Active) {
            pageStack.pushAttached(menuPage)
        }
    }

    StateIndicator {
        model: model
    }

    SilicaListView {
        id: view
        anchors.fill: parent
        visible: model.status === SocialNetworkStatus.Ready || model.count > 0
        currentIndex: -1
        spacing: Theme.paddingMedium
        model: NewsFeedProxyModel {
            id: model
            filterAds: settingsManager.filterAds
            model: SocialContentModel {
                socialNetwork: facebook
                request: FacebookNewsFeedRequest {}
                builder: FacebookNewsFeedModelBuilder {
                    properties: [
                        FacebookProperty {
                            path: "node/id"
                            name: "id"
                        },
                        FacebookListProperty {
                            path: "node/actors"
                            name: "actors"
                            properties: [
                                FacebookProperty { path: "id"; name: "id" },
                                FacebookProperty { path: "name"; name: "name" },
                                FacebookProperty { path: "profile_picture/uri"; name: "profilePicture" }
                            ]
                        },
                        FacebookProperty {
                            path: "node/to/id"
                            name: "toId"
                        },
                        FacebookProperty {
                            path: "node/to/name"
                            name: "toName"
                        },
                        FacebookProperty {
                            path: "node/to/profile_picture/uri"
                            name: "toProfilePicture"
                        },
                        FacebookProperty {
                            path: "node/creation_time"
                            name: "timestamp"
                        },
                        FacebookProperty {
                            path: "node/application/name"
                            name: "application"
                        },
                        FacebookProperty {
                            path: "node/title/text"
                            name: "title"
                        },
                        FacebookProperty {
                            path: "node/message/text"
                            name: "message"
                        },
                        FacebookListProperty {
                            path: "node/with"
                            name: "with"
                            properties: [
                                FacebookProperty { path: "id"; name: "id" },
                                FacebookProperty { path: "name"; name: "name" },
                                FacebookProperty { path: "profile_picture/uri"; name: "profilePicture" }
                            ]
                        },
                        FacebookListProperty {
                            path: "node/attachments"
                            name: "attachments"
                            properties: [
                                FacebookProperty { path: "description/text"; name: "description" },
                                FacebookProperty { path: "media/__type__/name"; name: "type" },
                                FacebookProperty { path: "media/imageLargeAspect/uri"; name: "image" },
                                FacebookProperty { path: "media/imageLargeAspect/width"; name: "width" },
                                FacebookProperty { path: "media/imageLargeAspect/height"; name: "height" },
                                FacebookProperty { path: "title"; name: "title" },
                                FacebookProperty { path: "url"; name: "url" },
                                FacebookProperty { path: "source/text"; name: "source" },
                                FacebookProperty { path: "style_list"; name: "style" },
                                FacebookListProperty {
                                    path: "subattachments"
                                    name: "subattachments"
                                    properties: [
                                        FacebookProperty { path: "description/text"; name: "description" },
                                        FacebookProperty { path: "media/__type__/name"; name: "type" },
                                        FacebookProperty { path: "media/imageLargeAspect/uri"; name: "image" },
                                        FacebookProperty { path: "media/imageLargeAspect/width"; name: "width" },
                                        FacebookProperty { path: "media/imageLargeAspect/height"; name: "height" },
                                        FacebookProperty { path: "title"; name: "title" },
                                        FacebookProperty { path: "url"; name: "url" },
                                        FacebookProperty { path: "source/text"; name: "source" },
                                        FacebookProperty { path: "style_list"; name: "style" }
                                    ]
                                }
                            ]
                        },
                        FacebookListProperty {
                            path: "node/attached_story/actors"
                            name: "attachedStoryActors"
                            properties: [
                                FacebookProperty { path: "id"; name: "id" },
                                FacebookProperty { path: "name"; name: "name" },
                                FacebookProperty { path: "profile_picture/uri"; name: "profilePicture" }
                            ]
                        },
                        FacebookListProperty {
                            path: "node/attached_story/attachments"
                            name: "attachedStoryAttachments"
                            properties: [
                                FacebookProperty { path: "description/text"; name: "description" },
                                FacebookProperty { path: "media/__type__/name"; name: "type" },
                                FacebookProperty { path: "media/imageLargeAspect/uri"; name: "image" },
                                FacebookProperty { path: "media/imageLargeAspect/width"; name: "width" },
                                FacebookProperty { path: "media/imageLargeAspect/height"; name: "height" },
                                FacebookProperty { path: "title"; name: "title" },
                                FacebookProperty { path: "url"; name: "url" },
                                FacebookProperty { path: "source/text"; name: "source" },
                                FacebookProperty { path: "style_list"; name: "style" },
                                FacebookListProperty {
                                    path: "subattachments"
                                    name: "subattachments"
                                    properties: [
                                        FacebookProperty { path: "description/text"; name: "description" },
                                        FacebookProperty { path: "media/__type__/name"; name: "type" },
                                        FacebookProperty { path: "media/imageLargeAspect/uri"; name: "image" },
                                        FacebookProperty { path: "media/imageLargeAspect/width"; name: "width" },
                                        FacebookProperty { path: "media/imageLargeAspect/height"; name: "height" },
                                        FacebookProperty { path: "title"; name: "title" },
                                        FacebookProperty { path: "url"; name: "url" },
                                        FacebookProperty { path: "source/text"; name: "source" },
                                        FacebookProperty { path: "style_list"; name: "style" }
                                    ]
                                }
                            ]
                        },
                        FacebookProperty {
                            path: "node/attached_story/message/title"
                            name: "attachedStoryTitle"
                        },
                        FacebookProperty {
                            path: "node/attached_story/message/text"
                            name: "attachedStoryMessage"
                        },
                        FacebookListProperty {
                            path: "node/attached_story/with"
                            name: "attachedStoryWith"
                            properties: [
                                FacebookProperty { path: "id"; name: "id" },
                                FacebookProperty { path: "name"; name: "name" },
                                FacebookProperty { path: "profile_picture/uri"; name: "profilePicture" }
                            ]
                        },
                        FacebookProperty {
                            path: "node/feedback/likers/count"
                            name: "likes"
                        },
                        FacebookProperty {
                            path: "node/feedback/top_level_comments/count"
                            name: "comments"
                        },
                        FacebookListProperty {
                            path: "node/negative_feedback_actions/edges"
                            name: "negativeFeedback"
                            properties: [
                                FacebookProperty { path: "node/negative_feedback_action_type"; name: "type" }
                            ]
                        }
                    ]
                }
            }
            Component.onCompleted: load()
        }

        header: PageHeader {
            //: Title of the page showing the news feed (or the Home feed)
            //% "News"
            title: qsTrId("friends_news_title")
        }

        delegate: PostDelegate {
            object: model.object
            fancy: false
        }

        onAtYEndChanged: {
            if (atYEnd) {
                if (model.status == SocialNetworkStatus.Ready || model.status == SocialNetworkStatus.Error) {
                    model.loadNext()
                }
            }
        }

        VerticalScrollDecorator {}

        ViewPlaceholder {
            enabled: model.status === SocialNetworkStatus.Idle && model.count == 0
            //: Text shown on the news page placeholder, where there is no news to be displayed
            //% "No news"
            text: qsTrId("friends_news_placeholder")
        }
    }
}
