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
import "UiConstants.js" as Ui

Page {
    id: container
    property string identifier
    function load() {
        if (user.status == SocialNetwork.Idle || user.status == SocialNetwork.Error) {
            user.load()
        }
    }

    onStatusChanged: {
        if (status == PageStatus.Active) {
            pageStack.pushAttached(menuPage)
        }
    }

    Component.onCompleted: {
        console.debug(Theme.highlightColor)
        console.debug(Theme.secondaryHighlightColor)
    }

    UserInfoHelper {
        id: helper
        object: user
        highlightColor: Theme.highlightColor
        onGenderChanged: user.update("gender", gender)
        onInterestedInChanged: user.update("interested-in", interestedIn)
        onBirthdayChanged: user.update("birthday", birthday)
        onReligionChanged: user.update("religion", religion)
        onPoliticalChanged: user.update("political", political)
        onLocationChanged: user.update("location", location)
        onHometownChanged: user.update("hometown", hometown)
    }

    FacebookUser {
        id: user
        function update(field, text) {
            for (var i = 0; i < model.count; i++) {
                if (model.get(i).field == field) {
                    model.setProperty(i, "text", text)
                }
            }
        }

        socialNetwork: facebook
        filter: FacebookItemFilter {
            identifier: container.identifier
            fields: "name,bio,quotes,gender,interested_in,birthday,religion,political,location,hometown"
        }
        onLoaded: {
            if (ok) {
                model.clear()
                if (user.bio.length > 0) {
                    //: Category text for biography
                    //% "Biography"
                    model.append({"category": qsTrId("friends_user_info_category_bio"),
                                  "field": "bio",
                                  "text": user.bio})
                }

                if (user.quotes.length > 0) {
                    //: Category text for the user's quotes
                    //% "Quotes"
                    model.append({"category": qsTrId("friends_user_info_category_quotes"),
                                  "field": "quotes",
                                  "text": user.quotes})
                }

                if (user.gender != FacebookUser.UnknownGender) {
                    //: Category text for the general informations
                    //% "Informations"
                    model.append({"category": qsTrId("friends_user_info_category_info"),
                                  "field": "gender",
                                  "text": helper.gender})
                }

                if (user.interestedIn != 0) {
                    model.append({"category": qsTrId("friends_user_info_category_info"),
                                  "field": "interested-in",
                                  "text": helper.interestedIn})
                }

                if (user.birthday != "") {
                    model.append({"category": qsTrId("friends_user_info_category_info"),
                                  "field": "birthday",
                                  "text": helper.birthday})
                }

                if (user.religion != "") {
                    model.append({"category": qsTrId("friends_user_info_category_info"),
                                  "field": "religion",
                                  "text": helper.religion})
                }

                if (user.political != "") {
                    model.append({"category": qsTrId("friends_user_info_category_info"),
                                  "field": "political",
                                  "text": helper.political})
                }

                if (user.location.objectIdentifier != "") {
                    //: Category text for places where the user was / is
                    //% "Places"
                    model.append({"category": qsTrId("friends_user_info_category_places"),
                                  "field": "location",
                                  "text": helper.location})
                }

                if (user.hometown.objectIdentifier != "") {
                    model.append({"category": qsTrId("friends_user_info_category_places"),
                                  "field": "hometown",
                                  "text": helper.hometown})
                }
            }
        }
    }

    StateIndicator {
        item: user
    }

    ListModel {
        id: model
    }

    SilicaListView {
        id: view
        anchors.fill: parent
        visible: (user.status == SocialNetwork.Idle)
        header: PageHeader {
            //: Title of the page showing personnal information
            //% "Personal information"
            title: container.identifier == me.identifier ? qsTrId("friends_user_info_about_me")
            //: Title of the page showing personnal informations of a given user. %1 contains the name of the user.
            //% "About %1"
                                                         : qsTrId("friends_user_info_about").arg(user.name)
        }
        model: model
        spacing: Theme.paddingMedium
        delegate: Label {
            anchors.left: parent.left; anchors.leftMargin: Theme.paddingMedium
            anchors.right: parent.right; anchors.rightMargin: Theme.paddingMedium
            text: model.text
            wrapMode: Text.WordWrap
            textFormat: Text.RichText
            onLinkActivated: {
                var page = pageStack.push(Qt.resolvedUrl("TypeSolverPage.qml"),
                                          {"identifier": link})
                page.load()
            }
        }
        section.property: "category"
        section.delegate: SectionHeader {
            text: section
        }

        VerticalScrollDecorator {}

        ViewPlaceholder {
            enabled: user.status == SocialNetwork.Idle && model.count == 0
            //: Text shown on the placeholder, where there is no information about an user
            //% "There is no information to be displayed"
            text: qsTrId("friends_user_info_placeholder")
        }
    }
}
