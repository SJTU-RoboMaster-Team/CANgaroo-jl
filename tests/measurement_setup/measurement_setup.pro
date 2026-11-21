TARGET = measurement_setup_test
include(../common.pri)

QT += xml

# MeasurementSetup::findDbMessage own tests: exact identifier matching for plain
# DBCs, PGN matching for J1939 databases, and interface rebinding.
HEADERS += $$SRC_DIR/core/MeasurementSetup.h

SOURCES += \
    MeasurementSetupTest.cpp \
    BackendStub.cpp \
    ../support/LogStub.cpp \
    $$SRC_DIR/core/MeasurementSetup.cpp \
    $$SRC_DIR/core/MeasurementNetwork.cpp \
    $$SRC_DIR/core/MeasurementInterface.cpp \
    $$SRC_DIR/db/dbc/DbcParser.cpp \
    $$SRC_DIR/db/dbc/DbcTokens.cpp \
    $$SRC_DIR/core/BusMessage.cpp \
    $$SRC_DIR/db/model/CanDb.cpp \
    $$SRC_DIR/db/model/CanDbMessage.cpp \
    $$SRC_DIR/db/model/CanDbNode.cpp \
    $$SRC_DIR/db/model/CanDbSignal.cpp
