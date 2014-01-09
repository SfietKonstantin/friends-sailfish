equals(QT_MAJOR_VERSION, 5): DEFINES += QT_VERSION_5

INCLUDEPATH += ../social/src/

HEADERS += $$PWD/alphabeticalsorterinterface.h \
    $$PWD/newsfeedfilterinterface.h \
    $$PWD/facebookextrapostinterface.h \
    $$PWD/typesolverinterface.h \
    socialextra/filterablefacebookrelateddatafilterinterface.h

SOURCES += $$PWD/alphabeticalsorterinterface.cpp \
    $$PWD/newsfeedfilterinterface.cpp \
    $$PWD/facebookextrapostinterface.cpp \
    $$PWD/typesolverinterface.cpp \
    socialextra/filterablefacebookrelateddatafilterinterface.cpp
