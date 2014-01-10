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

Page {
    id: container
    property string identifier
    property string postIdentifier
    function load() {
        if (identifier.length > 0) {
            if (item.status == SocialNetwork.Idle || item.status == SocialNetwork.Error) {
                item.load()
            }
            return
        }

        if (preloadPost.status == SocialNetwork.Idle || preloadPost.status == SocialNetwork.Error) {
            preloadPost.load()
        }
    }

    onStatusChanged: {
        if (status == PageStatus.Active) {
            if (item.source.length > 0) {
                item.pushPage(item.source, item.properties, item.needLoad, item.reparentedItems)
            }
        }
    }

    FacebookPost {
        // Used to translate attached id to graph id
        id: preloadPost
        socialNetwork: facebook
        filter: FacebookItemFilter {
            identifier: container.postIdentifier
            fields: "object_id"
        }
        onObjectIdentifierChanged: {
            if (objectIdentifier.length > 0) {
                container.identifier = objectIdentifier
                container.load()
            }
        }
    }

    TypeSolver {
        id: item
        property bool loading: false
        property string source
        property var properties
        property bool needLoad
        property list<QtObject> reparentedItems

        function pushPage(source, properties, needLoad, reparentedItems) {
            if (container.status == PageStatus.Active) {
                var page = pageStack.replace(Qt.resolvedUrl(source), properties,
                                             PageStackAction.Immediate)

                if (reparentedItems.length > 0) {
                    for (var i = 0; i < reparentedItems.length; i++) {
                        NotificationsHelper.reparentObject(reparentedItems[i], page)
                    }
                }

                if (needLoad) {
                    page.load()
                }
            } else {
                item.source = source
                item.properties = properties
                item.needLoad = needLoad
                if (reparentedItems !== null) {
                    item.reparentedItems = reparentedItems
                }
            }
        }

        function showUnsolvableObject() {
            console.debug("Unknown type: " + objectType + " and as string: " + objectTypeString)
            unsupported.enabled = true
        }

        function solveObjectType() {
            console.debug("Object Type:" + objectType + " " + objectTypeString)

            if (objectType == Facebook.Album) {
                item.pushPage("PhotosPage.qml", {"identifier": container.identifier}, true)
                return
            } else if (objectType == Facebook.Post) {
                if (objectTypeString == "post") {
                    post.filter.fields = "id,from,to,message,story,likes,comments,created_time,tags,link,picture,name,caption,description"
                } else if (objectTypeString == "location") {
                    post.filter.fields = "id,from,to,message,story,likes,comments,created_time,tags,"
                } else if (objectTypeString == "link") {
                    post.filter.fields = "id,from,to,message,story,likes,comments,created_time,link,picture,name,caption,description"
                } else if (objectTypeString == "status") {
                    post.filter.fields = "id,from,to,message,story,likes,comments,created_time"
                } else {
                    showUnsolvableObject()
                    return
                }
                indicator.item = post
                loading = true
                post.load()
            } else if (objectType == Facebook.User) {
                item.pushPage("UserPage.qml", {"identifier": container.identifier}, true)
                return
            } else if (objectType == Facebook.Photo) {
                indicator.item = photo
                loading = true
                photo.load()
            } else {
                showUnsolvableObject()
                return
            }
        }

        socialNetwork: facebook
        filter: TypeSolverFilter {
            identifier: container.identifier
        }
        onObjectTypeChanged: solveObjectType()
    }

    FacebookExtraPost {
        id: post
        socialNetwork: facebook
        filter: FacebookItemFilter {
            identifier: container.identifier
        }
        onStatusChanged: {
            if (status == SocialNetwork.Idle && item.loading) {
                var headerProperties = {"post": post}
                item.pushPage(Qt.resolvedUrl("CommentsPage.qml"),
                              {"identifier": post.identifier, "item": post,
                               "headerComponent": postHeaderComponent,
                               "headerProperties": headerProperties}, true, [post])
            }
        }
    }

    ListModel {
        id: photoModel
    }

    FacebookPhoto {
        id: photo
        socialNetwork: facebook
        filter: FacebookItemFilter {
            identifier: container.identifier
            fields: "id,name,updated_time,likes,comments"
        }

        onStatusChanged: {
            if (status == SocialNetwork.Idle && item.loading) {
                var headerProperties = {"post": post}
                photoModel.append({"contentItem": photo})

                item.pushPage(Qt.resolvedUrl("PhotoPage.qml"),
                              {"currentIndex": 0, "model": photoModel,
                               "isFacebookModel": false}, true, [photoModel, photo])
            }
        }
    }

    StateIndicator {
        id: indicator
        item: item
    }

    SilicaFlickable {
        anchors.fill: parent

        ViewPlaceholder {
            id: unsupported
            //: Describe that loading this item is not available yet
            //% "Friends cannot load this yet. This feature has not been implemented."
            text: qsTrId("friends_type_unsupported")
        }
    }
}

