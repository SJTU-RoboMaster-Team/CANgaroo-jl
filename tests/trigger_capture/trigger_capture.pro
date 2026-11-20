TARGET = trigger_capture_test
include(../common.pri)

HEADERS += $$SRC_DIR/core/TriggerCaptureManager.h

SOURCES += \
    TriggerCaptureTest.cpp \
    $$SRC_DIR/core/TriggerCaptureManager.cpp \
    $$SRC_DIR/core/TraceFileWriter.cpp \
    $$SRC_DIR/core/TraceFileReader.cpp \
    $$SRC_DIR/core/TraceLineFormat.cpp \
    $$SRC_DIR/core/BusMessage.cpp \
    $$PWD/../support/LogStub.cpp
