equals(QT_MAJOR_VERSION, 5): DEFINES += QT_VERSION_5

INCLUDEPATH += ../social/src/

HEADERS += $$PWD/alphabeticalsorterinterface.h \
    $$PWD/newsfeedfilterinterface.h \
    $$PWD/facebookextrapostinterface.h \
    $$PWD/typesolverinterface.h \
    socialextra/filterablefacebookrelateddatafilterinterface.h \
    socialextra/eventfilterinterface.h \
    socialextra/facebookextraeventinterface.h \
    socialextra/facebookextrainterface.h

SOURCES += $$PWD/alphabeticalsorterinterface.cpp \
    $$PWD/newsfeedfilterinterface.cpp \
    $$PWD/facebookextrapostinterface.cpp \
    $$PWD/typesolverinterface.cpp \
    socialextra/filterablefacebookrelateddatafilterinterface.cpp \
    socialextra/eventfilterinterface.cpp \
    socialextra/facebookextraeventinterface.cpp \
    socialextra/facebookextrainterface.cpp
