lessThan(QT_MAJOR_VERSION, 6): error("requires Qt 6")

QT += core gui
QT += widgets
QT += xml
QT += charts
QT += serialport
QT += svg
QT += network

TARGET = cangaroo
TEMPLATE = app
CONFIG += warn_on
CONFIG += link_pkgconfig

VERSION = 0.7.2
DEFINES += VERSION_STRING=\\\"$${VERSION}\\\"

TRANSLATIONS = \
    translations/i18n_zh_cn.ts
RC_ICONS = cangaroo.ico

INCLUDEPATH += $$PWD

DESTDIR = ../bin
MOC_DIR = ../build/moc
RCC_DIR = ../build/rcc
UI_DIR = ../build/ui
unix:OBJECTS_DIR = ../build/o/unix
win32:OBJECTS_DIR = ../build/o/win32
macx:OBJECTS_DIR = ../build/o/mac


SOURCES += main.cpp \
    mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

RESOURCES = cangaroo.qrc

include($$PWD/core/core.pri)
include($$PWD/driver/driver.pri)
include($$PWD/db/db.pri)
include($$PWD/decoders/decoders.pri)
include($$PWD/window/window.pri)
include($$PWD/helpers/helpers.pri)


PKGCONFIG += python3-embed
unix:INCLUDEPATH += /usr/include/pybind11
win32 {
    # Escaped quotes: qmake strips plain ones, and the shell then splits the
    # Python snippet at its spaces ("import" alone, a SyntaxError).
    PYBIND11_INCLUDE = $$system("python3 -c \"import pybind11; print(pybind11.get_include())\"")
    isEmpty(PYBIND11_INCLUDE): warning("pybind11 include directory not found (python3 -c 'import pybind11')")
    INCLUDEPATH += $$PYBIND11_INCLUDE
}

unix:PKGCONFIG += libnl-3.0
unix:PKGCONFIG += libnl-route-3.0
unix:INCLUDEPATH += /usr/include/libnl3
unix:include($$PWD/driver/SocketCanDriver/SocketCanDriver.pri)

# Serial port seam for drivers that take a simulated device in tests (SLCAN).
# Included once here, not from the driver .pri files.
include($$PWD/driver/SerialTransport/SerialTransport.pri)
include($$PWD/driver/SLCANDriver/SLCANDriver.pri)

win32:include($$PWD/driver/CandleApiDriver/CandleApiDriver.pri)

# Pass CONFIG+=peakcan to qmake to enable the PEAK PCAN driver.
# Requires the PCAN-Basic SDK extracted to src/driver/PeakCanDriver/pcan-basic-api/.
# Download: https://www.peak-system.com/fileadmin/media/files/PCAN-Basic.zip
win32:peakcan {
    DEFINES += PEAKCAN_DRIVER
    include($$PWD/driver/PeakCanDriver/PeakCanDriver.pri)
}

# Pass CONFIG+=kvaser to qmake to enable the Kvaser CANlib driver.
# Requires the Kvaser CANlib SDK (canlib.h + libcanlib / canlib32.dll).
# On Windows also set CANLIB_DIR=<path to SDK> (see KvaserDriver.pri).
kvaser {
    DEFINES += KVASER_DRIVER
    include($$PWD/driver/KvaserDriver/KvaserDriver.pri)
}

# Vector CAN driver — always enabled via Qt serialbus.
# Requires the Vector XL Driver Library installed on the target machine at runtime.
include($$PWD/driver/VectorDriver/VectorDriver.pri)

DISTFILES += \
    assets/filter-symbolic.svg
