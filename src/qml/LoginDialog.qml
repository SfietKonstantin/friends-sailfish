import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.friends 1.0

Dialog {
    id: container
    onRejected: Qt.quit()
    onAccepted: Qt.quit()
    signal connected()

    DialogHeader {
        id: header
        cancelText: qsTr("Quit")
        acceptText: qsTr("Ignore")
    }

    LoginManager {
        id: loginManager
        uiType: LoginManager.Mobile
        clientId: CLIENT_ID
        friendsPermissions: LoginManager.FriendsAboutMe
                            + LoginManager.FriendsActivities
                            + LoginManager.FriendsBirthday
                            + LoginManager.FriendsEducationHistory
                            + LoginManager.FriendsEvents
                            + LoginManager.FriendsGroups
                            + LoginManager.FriendsHometowm
                            + LoginManager.FriendsInterests
                            + LoginManager.FriendsLikes
                            + LoginManager.FriendsLocation
                            + LoginManager.FriendsNotes
                            + LoginManager.FriendsPhotos
                            + LoginManager.FriendsQuestions
                            + LoginManager.FriendsRelationships
                            + LoginManager.FriendsRelationshipDetails
                            + LoginManager.FriendsReligionPolitics
                            + LoginManager.FriendsStatus
                            + LoginManager.FriendsSubscriptions
                            + LoginManager.FriendsVideos
                            + LoginManager.FriendsWebsite
                            + LoginManager.FriendsWorkHistory
        userPermissions: LoginManager.UserAboutMe
                         + LoginManager.UserActivities
                         + LoginManager.UserBirthday
                         + LoginManager.UserEducationHistory
                         + LoginManager.UserEvents
                         + LoginManager.UserGroups
                         + LoginManager.UserHometown
                         + LoginManager.UserInterests
                         + LoginManager.UserLikes
                         + LoginManager.UserLocation
                         + LoginManager.UserNotes
                         + LoginManager.UserPhotos
                         + LoginManager.UserQuestions
                         + LoginManager.UserRelationships
                         + LoginManager.UserRelationshipDetails
                         + LoginManager.UserReligionPolitics
                         + LoginManager.UserStatus
                         + LoginManager.UserSubscriptions
                         + LoginManager.UserVideos
                         + LoginManager.UserWebsite
                         + LoginManager.UserWorkHistory
                         + LoginManager.Email
        extendedPermissions: LoginManager.ReadFriendList
                             + LoginManager.ReadInsights
                             + LoginManager.ReadMailbox
                             + LoginManager.ReadRequests
                             + LoginManager.ReadStream
                             + LoginManager.XmppLogin
                             + LoginManager.AdsManagement
                             + LoginManager.CreateEvent
                             + LoginManager.ManageFriendList
                             + LoginManager.ManageNotifications
                             + LoginManager.UserOnlinePresence
                             + LoginManager.FriendsOnlinePresence
                             + LoginManager.PublishCheckins
                             + LoginManager.PublishStream
                             + LoginManager.RsvpEvent

        Component.onCompleted: login()
        onUrlRequested: webView.url = url
        onLoginSucceeded: {
            tokenManager.token = token
            container.connected()
        }
    }

    SilicaWebView {
        id: webView
        anchors.top: header.bottom; anchors.bottom: parent.bottom
        anchors.left: parent.left; anchors.right: parent.right
        onUrlChanged: loginManager.checkUrl(url)
    }
}
