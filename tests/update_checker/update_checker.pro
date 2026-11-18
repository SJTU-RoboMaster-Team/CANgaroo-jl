TARGET = update_checker_test
include(../common.pri)

QT += network

HEADERS += \
    $$SRC_DIR/core/UpdateChecker.h

SOURCES += \
    UpdateCheckerTest.cpp \
    $$SRC_DIR/core/UpdateChecker.cpp
