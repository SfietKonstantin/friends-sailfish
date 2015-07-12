QML_POST_FILES += $$PWD/PostDelegate.qml \
    $$PWD/PostHeader.qml \
    $$PWD/PostContent.qml \
    $$PWD/PostAttachment.qml \
    $$PWD/PostAttachedStory.qml

OTHER_FILES += $$QML_POST_FILES

qmlPost.files = $$QML_POST_FILES
qmlPost.path = $$DEPLOYMENT_PATH/qml/post

INSTALLS += qmlPost
