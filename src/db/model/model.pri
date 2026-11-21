# CAN description database model: the in-memory object graph that the DBC
# parser (../dbc) builds and the GUI reads.
#
# These headers include core headers (BusMessage.h) by bare name, so core.pri's
# INCLUDEPATH must be in effect as well — src.pro includes both.

INCLUDEPATH += $$PWD

SOURCES += \
    $$PWD/CanDb.cpp \
    $$PWD/CanDbMessage.cpp \
    $$PWD/CanDbNode.cpp \
    $$PWD/CanDbSignal.cpp

HEADERS += \
    $$PWD/CanDb.h \
    $$PWD/CanDbMessage.h \
    $$PWD/CanDbNode.h \
    $$PWD/CanDbSignal.h
