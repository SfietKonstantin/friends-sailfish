equals(QT_MAJOR_VERSION, 5): DEFINES += QT_VERSION_5

INCLUDEPATH += ../social/src/

HEADERS += $$PWD/alphabeticalsorterinterface.h \
    socialextra/newsfeedfilterinterface.h

SOURCES += $$PWD/alphabeticalsorterinterface.cpp \
    socialextra/newsfeedfilterinterface.cpp
