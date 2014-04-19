system(qdbusxml2cpp harbour.friends.xml -i friendsdbusinterface.h -a adaptor)

HEADERS += $$PWD/friendsdbusinterface.h \
    $$PWD/adaptor.h

SOURCES += $$PWD/friendsdbusinterface.cpp \
    $$PWD/adaptor.cpp

OTHER_FILES += $$PWD/harbour.friends.xml
