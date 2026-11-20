TARGET = trace_file_reader_test
include(../common.pri)

SOURCES += \
    TraceFileReaderTest.cpp \
    $$SRC_DIR/core/TraceFileReader.cpp \
    $$SRC_DIR/core/TraceFileWriter.cpp \
    $$SRC_DIR/core/TraceLineFormat.cpp \
    $$SRC_DIR/core/BusMessage.cpp
