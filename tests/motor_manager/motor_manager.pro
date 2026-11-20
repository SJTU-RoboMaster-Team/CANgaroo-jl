TARGET = motor_manager_test
include(../common.pri)

HEADERS += \
    $$SRC_DIR/core/MotorManager.h \
    $$SRC_DIR/decoders/motor/MotorDecoder.h \
    $$SRC_DIR/decoders/motor/MotorTypes.h \
    $$SRC_DIR/decoders/ProtocolManager.h

SOURCES += \
    MotorManagerTest.cpp \
    $$SRC_DIR/core/MotorManager.cpp \
    $$SRC_DIR/decoders/motor/MotorDecoder.cpp \
    $$SRC_DIR/decoders/motor/MotorTypes.cpp \
    $$SRC_DIR/decoders/ProtocolManager.cpp \
    $$SRC_DIR/core/BusMessage.cpp \
    $$PWD/../support/LogStub.cpp
