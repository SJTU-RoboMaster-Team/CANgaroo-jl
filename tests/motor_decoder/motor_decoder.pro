TARGET = motor_decoder_test
include(../common.pri)

HEADERS += \
    $$SRC_DIR/decoders/motor/MotorDecoder.h \
    $$SRC_DIR/decoders/motor/MotorTypes.h \
    $$SRC_DIR/decoders/ProtocolManager.h

SOURCES += \
    MotorDecoderTest.cpp \
    $$SRC_DIR/decoders/motor/MotorDecoder.cpp \
    $$SRC_DIR/decoders/motor/MotorTypes.cpp \
    $$SRC_DIR/decoders/ProtocolManager.cpp \
    $$SRC_DIR/core/BusMessage.cpp \
    $$PWD/../support/LogStub.cpp
