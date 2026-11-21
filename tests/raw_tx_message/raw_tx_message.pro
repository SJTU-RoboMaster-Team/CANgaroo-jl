TARGET = raw_tx_message_test
include(../common.pri)

SOURCES += \
    RawTxMessageBuilderTest.cpp \
    $$SRC_DIR/window/RawTxWindow/RawTxMessageBuilder.cpp \
    $$SRC_DIR/core/BusMessage.cpp
