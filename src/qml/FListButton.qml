import QtQuick 2.0
import Sailfish.Silica 1.0

BackgroundItem {
    id: container
    property alias icon: icon.url
    property real iconSize: Theme.iconSizeMedium
    property alias text: text.text
    anchors.left: parent.left; anchors.right: parent.right

    Rectangle {
        visible: icon.visible
        anchors.fill: icon
        color: Theme.rgba(Theme.highlightBackgroundColor, 0.2)
    }

    FacebookImage {
        id: icon
        visible: status !== Image.Null
        anchors.left: parent.left; anchors.leftMargin: Theme.paddingMedium
        anchors.verticalCenter: parent.verticalCenter
        width: container.iconSize
        height: container.iconSize
    }

    Label {
        id: text
        anchors.left: icon.right; anchors.leftMargin: icon.visible ? Theme.paddingMedium : 0
        anchors.right: parent.right; anchors.rightMargin: Theme.paddingMedium
        anchors.verticalCenter: parent.verticalCenter
        color: !container.pressed ? Theme.primaryColor : Theme.highlightColor
        wrapMode: Text.NoWrap
        truncationMode: TruncationMode.Fade
    }
}

