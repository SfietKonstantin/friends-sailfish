import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.friends.social 1.0
import harbour.friends.social.extra 1.0

Page {
    id: container
    onStatusChanged: {
        if (status == PageStatus.Active) {
            pageStack.pushAttached(menuPage)
        }
    }

    SilicaListView {
        anchors.fill: parent
        cacheBuffer: Theme.itemSizeLarge * 2
        model: SocialNetworkModel {
            socialNetwork: facebook
            filter: FacebookRelatedDataFilter {
                identifier: facebook.currentUserIdentifier
                connection: Facebook.Friends
                limit: 500
            }
            sorters: AlphabeticalSorterInterface {field: "name"}
            Component.onCompleted: {
                load()
            }
            onStatusChanged: {
                if (status == Facebook.Idle && hasNext) {
                    loadNext()
                }
            }
        }

        header: PageHeader {
            title: qsTr("Friends")
        }

        delegate: BackgroundItem {
            height: Theme.itemSizeLarge
            FacebookPicture {
                id: icon
                anchors.left: parent.left; anchors.leftMargin: Theme.paddingMedium
                anchors.verticalCenter: parent.verticalCenter
                identifier: model.contentItem.identifier
            }

            Label {
                anchors.left: icon.right; anchors.leftMargin: Theme.paddingMedium
                anchors.right: parent.right; anchors.rightMargin: Theme.paddingMedium
                anchors.verticalCenter: parent.verticalCenter
                text: qsTr(model.contentItem.name)
                truncationMode: TruncationMode.Fade
            }
        }
    }
}
