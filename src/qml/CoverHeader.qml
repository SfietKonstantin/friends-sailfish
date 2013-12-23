/****************************************************************************************
 * Copyright (C) 2012 Lucien XU <sfietkonstantin@free.fr>                               *
 *                                                                                      *
 * This program is free software; you can redistribute it and/or modify it under        *
 * the terms of the GNU General Public License as published by the Free Software        *
 * Foundation; either version 3 of the License, or (at your option) any later           *
 * version.                                                                             *
 *                                                                                      *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY      *
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A      *
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.             *
 *                                                                                      *
 * You should have received a copy of the GNU General Public License along with         *
 * this program.  If not, see <http://www.gnu.org/licenses/>.                           *
 ****************************************************************************************/

import QtQuick 2.0
import Sailfish.Silica 1.0
import "UiConstants.js" as Ui

Item {
    id: container
    property string name
    property string coverUrl
    height: 2 * Theme.itemSizeExtraLarge

    Rectangle {
        id: coverBackground
        anchors.fill: parent
        color: Theme.secondaryHighlightColor
        clip: true

        FacebookImage {
            id: image
            anchors.fill: parent
            fillMode: Image.PreserveAspectCrop
            url: coverUrl
        }

//        Rectangle {
//            anchors.bottom: parent.bottom
//            anchors.left: parent.left; anchors.right: parent.right
//            height: Ui.MARGIN_DEFAULT + Ui.FONT_SIZE_XXLARGE + Ui.MARGIN_DEFAULT
//            opacity: 0.8
//            gradient: Gradient {
//                GradientStop {position: 0; color: "#00000000"}
//                GradientStop {position: 1; color: "black"}
//            }
//        }

        Text {
            id: nameText
            anchors.left: parent.left; anchors.leftMargin: Theme.paddingMedium
            anchors.right: parent.right; anchors.rightMargin: Theme.paddingMedium
            anchors.bottom: parent.bottom; anchors.bottomMargin: Theme.paddingMedium
            color: Theme.primaryColor
            opacity: 0
            elide: Text.ElideRight
            wrapMode: Text.NoWrap
            font.pixelSize: Theme.fontSizeHuge
            states: [
                State {
                    name: "visible"; when: container.name != ""
                    PropertyChanges {
                        target: nameText
                        opacity: 1
                        text: container.name
                    }
                }
            ]
            Behavior on opacity {
                NumberAnimation {duration: Ui.ANIMATION_DURATION_NORMAL}
            }
        }
    }
}
