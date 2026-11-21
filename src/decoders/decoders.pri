# Generic protocol decoders. Each decoder implements IDecoder and is fed by
# ProtocolManager; protocol specific sources live in their own subdirectory.

INCLUDEPATH += $$PWD

include($$PWD/motor/motor.pri)

HEADERS += \
    $$PWD/IDecoder.h \
    $$PWD/ProtocolTypes.h \
    $$PWD/ProtocolManager.h

SOURCES += \
    $$PWD/ProtocolManager.cpp
