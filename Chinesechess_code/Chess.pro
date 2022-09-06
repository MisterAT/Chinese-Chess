HEADERS += \
    Board.h \
    Stone.h \
    Step.h \
    SingleGame.h \
    allpushbutton.h \
    start_interface.h

SOURCES += \
    Board.cpp \
    allpushbutton.cpp \
    main.cpp \
    Stone.cpp \
    Step.cpp \
    SingleGame.cpp \
    start_interface.cpp

QT += widgets gui network multimedia

RESOURCES += \
    RESOURCE.qrc

CONFIG+=resources_big

RC_FILE = favicon.rc
