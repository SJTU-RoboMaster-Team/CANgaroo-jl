TARGET = motor_dbc_test
include(../common.pri)

# The generated database is parsed with DbcParser, whose CanDb::saveXML takes a
# QDomDocument even though this test never saves anything.
QT += xml

HEADERS += \
    $$SRC_DIR/decoders/motor/MotorDbc.h \
    $$SRC_DIR/decoders/motor/MotorDecoder.h \
    $$SRC_DIR/decoders/motor/MotorTypes.h \
    $$SRC_DIR/decoders/ProtocolManager.h

SOURCES += \
    MotorDbcTest.cpp \
    ../support/LogStub.cpp \
    $$SRC_DIR/decoders/motor/MotorDbc.cpp \
    $$SRC_DIR/decoders/motor/MotorDecoder.cpp \
    $$SRC_DIR/decoders/motor/MotorTypes.cpp \
    $$SRC_DIR/decoders/ProtocolManager.cpp \
    $$SRC_DIR/core/BusMessage.cpp \
    $$SRC_DIR/db/dbc/DbcParser.cpp \
    $$SRC_DIR/db/dbc/DbcTokens.cpp \
    $$SRC_DIR/db/model/CanDb.cpp \
    $$SRC_DIR/db/model/CanDbMessage.cpp \
    $$SRC_DIR/db/model/CanDbNode.cpp \
    $$SRC_DIR/db/model/CanDbSignal.cpp
