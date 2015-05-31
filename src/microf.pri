# This PRI is used to get a static build of microf
include(../microf/src/config.pri)

INCLUDEPATH += ../microf/src/lib

HEADERS += \
    ../microf/src/lib/socialnetwork.h \
    ../microf/src/lib/socialnetwork_p.h \
    ../microf/src/lib/socialnetworkstatus.h \
    ../microf/src/lib/socialnetworkerror.h \
    ../microf/src/lib/socialrequest.h \
    ../microf/src/lib/socialrequest_p.h \
    ../microf/src/lib/isocialcontent.h \
    ../microf/src/lib/abstractsocialcontent_p.h \
    ../microf/src/lib/socialcontentitem.h \
    ../microf/src/lib/socialcontentitem_p.h \
    ../microf/src/lib/socialcontentitembuilder.h \
    ../microf/src/lib/socialcontentitembuilder_p.h \
    ../microf/src/lib/socialcontentmodel.h \
    ../microf/src/lib/socialcontentmodel_p.h \
    ../microf/src/lib/socialcontentmodelbuilder.h \
    ../microf/src/lib/socialcontentmodelbuilder_p.h \
    ../microf/src/lib/facebook/facebook.h \
    ../microf/src/lib/facebook/facebook_p.h \
    ../microf/src/lib/facebook/abstractfacebookrequest.h \
    ../microf/src/lib/facebook/facebookloginrequest.h \
    ../microf/src/lib/facebook/facebooklogincontentbuilder.h \
    ../microf/src/lib/facebook/facebooklogoutrequest.h \
    ../microf/src/lib/facebook/facebookconfirmationcontentbuilder.h \
    ../microf/src/lib/facebook/facebookproperty.h \
    ../microf/src/lib/facebook/facebookproperty_p.h \
    ../microf/src/lib/facebook/facebooklistproperty.h \
    ../microf/src/lib/facebook/facebookitembuilder.h \
    ../microf/src/lib/facebook/abstractfacebookmodelbuilder.h \
    ../microf/src/lib/facebook/facebookmodelbuilder.h \
    ../microf/src/lib/facebook/facebookfriendlistrequest.h \
    ../microf/src/lib/facebook/facebookusersummaryrequest.h \
    ../microf/src/lib/facebook/facebooknewsfeedrequest.h \
    ../microf/src/lib/facebook/facebooknewsfeedmodelbuilder.h \
    ../microf/src/lib/plugin.h

SOURCES += \
    ../microf/src/lib/socialnetwork.cpp \
    ../microf/src/lib/socialnetworkstatus.cpp \
    ../microf/src/lib/socialnetworkerror.cpp \
    ../microf/src/lib/socialrequest.cpp \
    ../microf/src/lib/socialcontentitem.cpp \
    ../microf/src/lib/socialcontentitembuilder.cpp \
    ../microf/src/lib/socialcontentmodel.cpp \
    ../microf/src/lib/socialcontentmodelbuilder.cpp \
    ../microf/src/lib/facebook/facebook.cpp \
    ../microf/src/lib/facebook/abstractfacebookrequest.cpp \
    ../microf/src/lib/facebook/facebookloginrequest.cpp \
    ../microf/src/lib/facebook/facebooklogincontentbuilder.cpp \
    ../microf/src/lib/facebook/facebooklogoutrequest.cpp \
    ../microf/src/lib/facebook/facebookconfirmationcontentbuilder.cpp \
    ../microf/src/lib/facebook/facebookproperty.cpp \
    ../microf/src/lib/facebook/facebooklistproperty.cpp \
    ../microf/src/lib/facebook/facebookitembuilder.cpp \
    ../microf/src/lib/facebook/abstractfacebookmodelbuilder.cpp \
    ../microf/src/lib/facebook/facebookmodelbuilder.cpp \
    ../microf/src/lib/facebook/facebookfriendlistrequest.cpp \
    ../microf/src/lib/facebook/facebookusersummaryrequest.cpp \
    ../microf/src/lib/facebook/facebooknewsfeedrequest.cpp \
    ../microf/src/lib/facebook/facebooknewsfeedmodelbuilder.cpp
