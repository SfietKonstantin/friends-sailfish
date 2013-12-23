/****************************************************************************************
 * Copyright (C) 2011 Lucien XU <sfietkonstantin@free.fr>                               *
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
import "UiConstants.js" as Ui
import harbour.friends 1.0

Image {
    id: image
    property string url
    onUrlChanged: ImageLoader.load(url)
    smooth: true
    asynchronous: true
    opacity: 0
    states: State {
        name: "visible"; when: image.status == Image.Ready
        PropertyChanges {
            target: image
            opacity: 1
        }
    }
    Behavior on opacity {
        enabled: image.visible
        NumberAnimation {duration: Ui.ANIMATION_DURATION_FAST}
    }

    Connections {
        target: ImageLoader
        onLoaded: {
            if (url == image.url) {
                image.source = path
            }
        }
    }
}
