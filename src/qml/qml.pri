QML_FILES += $$PWD/friends.qml \
    $$PWD/UiConstants.js \
    $$PWD/CoverHeader.qml \
    $$PWD/CoverPage.qml \
    $$PWD/MenuPage.qml \
    $$PWD/NewsPage.qml \
    $$PWD/LoginPage.qml \
    $$PWD/FacebookImage.qml \
    $$PWD/FacebookPicture.qml \
    $$PWD/StateIndicator.qml \
    $$PWD/AlbumsPage.qml \
    $$PWD/UserPage.qml \
    $$PWD/AboutPage.qml \
    $$PWD/DevelopersPage.qml \
    $$PWD/PhotosPage.qml \
    $$PWD/PhotoPage.qml \
    $$PWD/SplitSocialPanel.qml \
    $$PWD/CommentsPage.qml \
    $$PWD/PostCommentHeaderComponent.qml \
    $$PWD/WelcomeDialog.qml \
    $$PWD/SocialButtons.qml \
    $$PWD/PostDialog.qml \
    $$PWD/LikesPage.qml \
    $$PWD/NotificationsPage.qml \
    $$PWD/TypeSolverPage.qml \
    $$PWD/ChangeLogPage.qml \
    $$PWD/UpdatedDialog.qml \
    $$PWD/ChangeLogView.qml \
    $$PWD/GroupPage.qml \
    $$PWD/GroupsPage.qml \
    $$PWD/PagesPage.qml \
    $$PWD/PagePage.qml \
    $$PWD/CoverImage.qml \
    $$PWD/ThreadsPage.qml \
    $$PWD/EventsPage.qml \
    $$PWD/EventPage.qml \
    $$PWD/UserInfoPage.qml \
    $$PWD/GroupInfoPage.qml \
    $$PWD/UsersPage.qml \
    $$PWD/FLabel.qml \
    $$PWD/FListButton.qml \
    $$PWD/WaitingPage.qml \
    $$PWD/DisconnectPage.qml \
    $$PWD/FriendsPage.qml


OTHER_FILES += $$QML_FILES

qml.files = $$QML_FILES
qml.path = $$DEPLOYMENT_PATH/qml

INSTALLS += qml

include(post/post.pri)
