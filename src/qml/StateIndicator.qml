import QtQuick 2.0
import Sailfish.Silica 1.0

Item {
    id: container
    anchors.fill: parent
    signal reload()
    property bool busy: false
    property bool error: false

    BusyIndicator {
        visible: container.busy
        running: visible
        size: BusyIndicatorSize.Large
        anchors.centerIn: parent
    }

    SilicaFlickable {
        anchors.fill: parent
        contentWidth: width
        contentHeight: height

        ViewPlaceholder {
            anchors.centerIn: parent
            enabled: container.error
            text: qsTr("Something wrong happened. Check your Internet connection and try again")
        }
    }

    Button {
        visible: container.error
        anchors.bottom: parent.bottom; anchors.horizontalCenter: parent.horizontalCenter
        text: qsTr("Retry")
        onClicked: container.reload()
    }
}


